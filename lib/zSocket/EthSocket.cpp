/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <net/if.h>
#include <netinet/in.h>
//#include <netinet/ether.h>

#include <zutils/zLog.h>
#include <zutils/zSocket.h>
#include <zutils/zEthSocket.h>

namespace zUtils
{
namespace zSocket
{

static bool
_str2mac(const std::string& addr_, struct hwaddr& hwaddr_)
{
  return (sscanf(addr_.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
      &hwaddr_.addr[0], &hwaddr_.addr[1], &hwaddr_.addr[2],
      &hwaddr_.addr[3], &hwaddr_.addr[4], &hwaddr_.addr[5]) == ETH_ALEN);
}

static bool
_mac2str(const struct hwaddr& hwaddr_, std::string& addr_)
{
  bool status = false;
  char str[256] = { 0 };
  if (sprintf(str, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
      hwaddr_.addr[0], hwaddr_.addr[1], hwaddr_.addr[2],
      hwaddr_.addr[3], hwaddr_.addr[4], hwaddr_.addr[5]) > 0)
  {
    addr_ = std::string(str);
    status = true;
  }
  return (status);
}

//**********************************************************************
// Class: zSocket::EthAddress
//**********************************************************************

EthAddress::EthAddress(const std::string &addr_) :
    SocketAddress(SocketType::TYPE_ETH, addr_)
{
}

EthAddress::EthAddress(SocketAddress &addr_) :
    SocketAddress(addr_)
{
}

EthAddress::EthAddress(const SocketAddress &addr_) :
    SocketAddress(addr_)
{
}

EthAddress::~EthAddress()
{
}

struct hwaddr
EthAddress::HwAddress() const
{
  struct hwaddr addr = { 0 };
  _str2mac(this->_addr, addr);
  return (addr);
}

bool
EthAddress::HwAddress(const struct hwaddr addr_)
{
  return (_mac2str(addr_, this->_addr));
}

bool
EthAddress::verify(const SocketType type_, const std::string &addr_)
{
  bool status = false;
  struct hwaddr hwaddr = { 0 };
  if (type_ == SocketType::TYPE_ETH)
  {
    status = _str2mac(addr_, hwaddr);
  }
  return (status);
}

//**********************************************************************
// zSocket::EthSocketRecv Class
//**********************************************************************

void
EthSocketRecv::Run(zThread::ThreadArg *arg_)
{
  EthSocket *sock = (EthSocket *) arg_;
  ssize_t bytes = -1;

  // Check if any sockets exist
  if (sock->_sockfd.empty())
  {
    ZLOG_CRIT(std::string("No sockets not opened"));
    return;
  }

  ZLOG_DEBUG("Waiting for RX data on ETH socket");

  // Setup for poll loop
  int cnt = 0;
  struct pollfd fds[sock->_sockfd.size()] = { 0 };
  FOREACH(auto& fd, sock->_sockfd)
  {
    fds[cnt].fd = fd.second;
    fds[cnt].events = (POLLIN | POLLERR);
    cnt++;
  }

  while (!this->Exit())
  {
    // Poll for received data
    int ret = poll(fds, 1, 1000);

    // Check for timeout
    if (ret == 0)
    {
      continue;
    } // end if

    // Check for errors
    if (ret < 0)
    {
      ZLOG_ERR("Error polling on socket: " + std::string(strerror(errno)));
      continue;
    } // end if

    // Loop through all sockets
    for (int i = 0; i < cnt; i++)
    {
      if (fds[i].revents == POLLIN)
      {
        SHARED_PTR(EthAddress)addr(new EthAddress);
        SHARED_PTR(SocketBuffer)sb(new SocketBuffer);
        bytes = sock->_recv(fds[i].fd, *addr, *sb);
        if (bytes > 0)
        {
          ZLOG_INFO("Received packet on socket: " + ZLOG_INT(fds[i].fd));
          SocketAddressBufferPair p(addr, sb);
          sock->rxbuf(p);
        }
      }
      else if (fds[i].revents == POLLERR)
      {
        ZLOG_WARN(ZLOG_INT(fds[i].fd) + ": POLLERR");
      }
      else
      {
        ZLOG_ERR(ZLOG_INT(fds[i].fd) + ": Unhandled revent: 0x" + ZLOG_HEX(fds[i].revents));
      }
    }

  }

  return;

}

//**********************************************************************
// zSocket::EthSocketSend Class
//**********************************************************************

void
EthSocketSend::Run(zThread::ThreadArg *arg_)
{
  EthSocket *sock = (EthSocket *) arg_;
  SocketAddressBufferPair p;

  if (sock->_sockfd.empty())
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return;
  }

  ZLOG_DEBUG("Waiting for TX data on ETH socket");

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = sock->_sockfd.begin()->second;
  fds[0].events = (POLLOUT | POLLERR);

  while (!this->Exit())
  {

    // Test for data to send
    if (sock->txbuf(p, 100))
    {
      // Poll for available TX buffer
      int ret = poll(fds, 1, 100);
      if (ret > 0 && (fds[0].revents == POLLOUT))
      {
        ZLOG_DEBUG("Sending packet: " + p.first->Address() + "(" + ZLOG_INT(p.second->Size()) + ")");
        if (sock->_send(fds[0].fd, *p.first, *p.second) != p.second->Size())
        {
          ZLOG_ERR("Error sending packet");
        }
      }
    }

  }

  return;

}

//**********************************************************************
// zSocket::EthSocket Class
//**********************************************************************

EthSocket::EthSocket() :
    Socket(SocketType::TYPE_ETH), _iface(NULL),
    _rx_thread(&this->_rx_func, this), _tx_thread(&this->_tx_func, this)
{
}

EthSocket::~EthSocket()
{
  this->Close();
}

bool
EthSocket::Open(const PROTO proto_)
{

  // Check if socket has already been created
  if (this->_sockfd.count(proto_))
  {
    return (true);
  }

  // Create the raw socket
  int fd = socket( PF_PACKET, SOCK_RAW, htons(proto_));
  if (fd < 0)
  {
    ZLOG_CRIT("Cannot create socket: " + std::string(strerror(errno)));
    return (false);
  } // end if

  // Save the new socket file descriptor
  this->_sockfd[proto_] = fd;

  ZLOG_DEBUG("Opening socket: " + ZLOG_INT(fd));

  // Enable reuse of socket
  int sockopt = 0;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) == -1)
  {
    ZLOG_CRIT("Cannot configure socket: " + std::string(strerror(errno)));
    return (false);
  }

  // Return success
  return (true);

}

bool
EthSocket::Open()
{
  return (this->Open(PROTO_ALL));
}

bool
EthSocket::Close(const PROTO proto_)
{

  // Check if the socket has been created
  if (!this->_sockfd.count(proto_))
  {
    return (true);
  }

  // TODO: For now just stop all threads regardless if there are still any open
  if (this->_rx_thread.Stop() && this->_tx_thread.Stop())
  {
    int fd = this->_sockfd[proto_];
    this->_sockfd.erase(proto_);
    close(fd);
  }

  return (true);

}

void
EthSocket::Close()
{
  FOREACH(auto& fd, this->_sockfd)
  {
    this->Close(fd.first);
  }
}

bool
EthSocket::Bind(const zInterface::Interface& iface_)
{
  bool status = false;
  if (this->_iface)
  {
    delete (this->_iface);
    this->_iface = NULL;
  }
  this->_iface = new zInterface::Interface(iface_.GetIfName());
  if (this->_iface && this->_iface->Refresh())
  {
    EthAddress addr(this->_iface->GetHwAddress());
    status = Socket::Bind(addr);
  }
  return (status);
}

bool
EthSocket::_bind()
{

  if (!this->_iface)
  {
    ZLOG_ERR("Cannot directly call 'Bind()'; please specify an interface to bind to");
    return (false);
  }

  struct sockaddr_ll sockaddr = { 0 };

  sockaddr.sll_family = AF_PACKET;
  sockaddr.sll_ifindex = this->_iface->GetIfIndex();
  sockaddr.sll_halen = ETH_ALEN;
  memset(sockaddr.sll_addr, 0, ETH_ALEN);
  sockaddr.sll_pkttype = PACKET_HOST;

  FOREACH(auto& fd, this->_sockfd)
  {
    sockaddr.sll_protocol = htons(fd.first);
    ZLOG_DEBUG("Bind on socket: " + ZLOG_INT(fd.second));
    if (bind(fd.second, (struct sockaddr*) &sockaddr, sizeof(struct sockaddr_ll)))
    {
      ZLOG_ERR("Cannot bind socket: " + this->_iface->GetIfName() + ": " + std::string(strerror(errno)));
    } // end if
  }

  // Start listener threads
  if (!this->_rx_thread.Start() || !this->_tx_thread.Start())
  {
    ZLOG_ERR("Error starting listening threads");
    return (false);
  }

  return (true);

}

ssize_t
EthSocket::_recv(const int fd_, zSocket::EthAddress & addr_, zSocket::SocketBuffer & sb_)
{

  ssize_t n = -1;
  struct sockaddr_ll src = { 0 };
  socklen_t srclen = sizeof(src);
  int flags = 0;

  if (!fd_)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (-1);
  }

  n = recvfrom(fd_, sb_.Head(), sb_.TotalSize(), 0, (struct sockaddr *) &src, &srclen);
  if (n > 0)
  {
    sb_.Put(n);
    std::string addr;
    _mac2str(*(struct hwaddr*)&src.sll_addr, addr);
    addr_.Address(addr);

    uint8_t* p = sb_.Head();
    std::string logstr;
    logstr += "Receiving on socket:\t";
    logstr += "To:     " + this->Address().Address() + ";\t";
    logstr += "From:   " + addr_.Address() + ";\t";
    logstr += "Size:   " + ZLOG_INT(n) + ";\t";
    logstr += "Family: " + ZLOG_INT(src.sll_family) + ";\t";
    logstr += "Type:   " + ZLOG_INT(src.sll_pkttype) + ";\t";
    int proto = htobe16(src.sll_protocol);
    logstr += "Proto:  " + ZLOG_HEX(proto) + ";\t";
    ZLOG_INFO(logstr);
    logstr = "Data (0x00): ";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ";";
    ZLOG_INFO(logstr);
    logstr = "Data (0x08): ";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ";";
    ZLOG_INFO(logstr);
    logstr = "Data (0x10): ";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ";";
    ZLOG_INFO(logstr);
    logstr = "Data (0x18): ";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ";";
    ZLOG_INFO(logstr);
    logstr = "Data (0x20): ";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ";";
    ZLOG_INFO(logstr);
    logstr = "Data (0x28): ";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ":";
    logstr += ZLOG_HEX(*p++) + ";";
    ZLOG_INFO(logstr);

  } // end if

  return (n);
}

ssize_t
EthSocket::_send(const int fd_, const zSocket::EthAddress &addr_, zSocket::SocketBuffer &sb_)
{

  ssize_t n = -1;
  struct sockaddr_ll src = { 0 };

  if (!fd_)
  {
    ZLOG_ERR(std::string("Socket not opened"));
    return (-1);
  }

  // Set socket address fields
  src.sll_ifindex = this->_iface->GetIfIndex();

  // Log info message about message being sent
  std::string logstr;
  uint8_t* p = sb_.Head();
  logstr += "Sending on socket:\t";
  logstr += "Iface: [" + ZLOG_INT(this->_iface->GetIfIndex()) + "] " + this->_iface->GetIfName() + ";\t";
  logstr += "To: " + addr_.Address() + ";\t";
//  logstr += "From: " + src->Address() + ";\t";
  logstr += "Size: " + ZLOG_INT(sb_.Size()) + ";\t";
  logstr += "Family: " + ZLOG_INT(src.sll_family) + ";\t";
  logstr += "Type:   " + ZLOG_INT(src.sll_pkttype) + ";\t";
  int proto = htobe16(src.sll_protocol);
  logstr += "Proto:  " + ZLOG_HEX(proto) + ";\t";
  ZLOG_INFO(logstr);
  logstr = "Data (0x00): ";
  logstr += ZLOG_HEX(*p++) + ":";
  logstr += ZLOG_HEX(*p++) + ":";
  logstr += ZLOG_HEX(*p++) + ":";
  logstr += ZLOG_HEX(*p++) + ":";
  logstr += ZLOG_HEX(*p++) + ":";
  logstr += ZLOG_HEX(*p++) + ":";
  logstr += ZLOG_HEX(*p++) + ":";
  logstr += ZLOG_HEX(*p++) + ";";
  ZLOG_INFO(logstr);
  logstr = "Data (0x08): ";
  logstr += ZLOG_HEX(*p++) + ":";
  logstr += ZLOG_HEX(*p++) + ":";
  logstr += ZLOG_HEX(*p++) + ":";
  logstr += ZLOG_HEX(*p++) + ":";
  logstr += ZLOG_HEX(*p++) + ":";
  logstr += ZLOG_HEX(*p++) + ":";
  logstr += ZLOG_HEX(*p++) + ":";
  logstr += ZLOG_HEX(*p++) + ";";
  ZLOG_INFO(logstr);

  // Send packet
  n = sendto(fd_, sb_.Head(), sb_.Size(), 0, (struct sockaddr *) &src, sizeof(src));
  if (n < 0)
  {
    ZLOG_ERR(std::string("Cannot send packet: " + std::string(strerror(errno))));
  }

  return (n);

}

bool
EthSocket::EventHandler(zEvent::EventNotification* notification_)
{
  return (false);
}

}
}
