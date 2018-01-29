/*
 * Copyright (c) 2014-2018 ZenoTec LLC (http://www.zenotec.net)
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
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <stdint.h>
//#include <string.h>
//#include <unistd.h>
//#include <errno.h>
//#include <sys/time.h>
//#include <sys/types.h>
//#include <poll.h>
//#include <sys/ioctl.h>
//#include <sys/socket.h>
//#include <ifaddrs.h>
//#include <linux/ip.h>
//#include <linux/udp.h>
//#include <net/if.h>
//#include <netinet/in.h>
////#include <netinet/ether.h>
//
//#include <zutils/zLog.h>
//#include <zutils/zSocket.h>
//#include <zutils/zEthSocket.h>
//
//ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);
//
//namespace zUtils
//{
//namespace zSocket
//{
//

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <netinet/in.h>
#include <poll.h>

#include <zutils/zUtils.h>
#include <zutils/zLog.h>
#include <zutils/zSocket.h>
#include <zutils/zEthSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

static bool
_str2mac(const std::string& addr_, uint8_t* mac_)
{
  return (sscanf(addr_.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
      &mac_[0], &mac_[1], &mac_[2], &mac_[3], &mac_[4], &mac_[5]) == ETH_ALEN);
}

static bool
_mac2str(const uint8_t* mac_, std::string& addr_)
{
  bool status = false;
  char str[18] = { 0 };
  int n = sprintf(str, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
      mac_[0], mac_[1], mac_[2], mac_[3], mac_[4], mac_[5]);
  if (n == 17)
  {
    addr_ = std::string(str);
    status = true;
  }
  return (status);
}

static bool
_addr2ifindex(const std::string& addr_, int& index_)
{
  // TODO: Address is the interface index
  return (sscanf(addr_.c_str(), "%d", &index_) == 1);
}

static bool
_addr2mac(const std::string& addr_, uint8_t* mac_)
{
  // TODO: NOP
  return (true);
}

static bool
_addr2sa(const std::string &addr_, struct sockaddr_ll &sa_)
{
  bool status = true;
  sa_.sll_family = AF_PACKET;
  status &= _addr2ifindex(addr_, sa_.sll_ifindex);
  sa_.sll_halen = ETH_ALEN;
  status &= _addr2mac(addr_, sa_.sll_addr);
  sa_.sll_pkttype = PACKET_HOST;
  return (status);
}

static bool
_sa2addr(const struct sockaddr_ll &sa_, std::string &addr_)
{
  addr_ = zToStr(sa_.sll_ifindex);
  return (!addr_.empty());
}

//**********************************************************************
// Class: zSocket::EthAddress
//**********************************************************************

EthAddress::EthAddress(const std::string &addr_) :
    Address(SOCKET_TYPE::TYPE_ETH, addr_), sa { 0 }
{
  _addr2sa(addr_, this->sa);
}

EthAddress::EthAddress(const Address &addr_) :
    Address(SOCKET_TYPE::TYPE_ETH), sa { 0 }
{
  if (addr_.GetType() == this->GetType())
  {
    this->SetAddress(addr_.GetAddress());
  }
}

EthAddress::EthAddress(const struct sockaddr_ll& sa_) :
        Address(SOCKET_TYPE::TYPE_ETH), sa (sa_)
{
  this->SetAddress(this->GetAddress());
}

EthAddress::~EthAddress()
{
}

std::string
EthAddress::GetAddress() const
{
  std::string addr;
  _sa2addr(this->sa, addr);
  return (addr);
}

bool
EthAddress::SetAddress(const std::string& addr_)
{
  bool status = false;
  if (_addr2sa(addr_, this->sa))
  {
    status = Address::SetAddress(addr_);
  }
  return (status);
}

//**********************************************************************
// Class: zSocket::EthSocket
//**********************************************************************

EthSocket::EthSocket(const EthSocket::PROTO proto_) :
    Socket(SOCKET_TYPE::TYPE_ETH), _sock(0)
{
  // Create a AF_INET socket
  this->_sock = socket( PF_PACKET, SOCK_RAW, htons(proto_));
  if (this->_sock > 0)
  {
    ZLOG_INFO("Socket created: " + ZLOG_INT(this->_sock));
  }
  else
  {
    this->_sock = 0;
    ZLOG_ERR("Cannot create socket: " + std::string(strerror(errno)));
  }
}

EthSocket::~EthSocket()
{
  ZLOG_INFO("Closing socket: " + ZLOG_INT(this->_sock));
  // Close socket
  if (this->_sock)
  {
    close(this->_sock);
    this->_sock = 0;
  }
}

bool
EthSocket::Getopt(Socket::OPTIONS opt_)
{
  bool status = false;
  switch (opt_)
  {
  case Socket::OPTIONS_ALLOW_REUSE:
  {
    int optval = 0;
    socklen_t optlen = sizeof(optval);
    if (getsockopt(this->_sock, SOL_SOCKET, SO_REUSEADDR, &optval, &optlen) < 0)
    {
      ZLOG_ERR("Cannot get socket option: " + std::string(strerror(errno)));
    }
    else
    {
      std::cerr << "OPTIONS_ALLOW_REUSE: " << optval << std::endl;
      status = (optval == 0);
    }
    break;
  }
  default:
    break;
  }
  return (status);
}

bool
EthSocket::Setopt(Socket::OPTIONS opt_)
{
  bool status = false;
  switch (opt_)
  {
  case Socket::OPTIONS_ALLOW_REUSE:
  {
    // Enable reuse of socket
    int optval = 0;
    if (setsockopt(this->_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
      ZLOG_CRIT("Cannot set socket option: " + std::string(strerror(errno)));
    }
    break;
  }
  default:
    break;
  }
  return (status);
}

int
EthSocket::_get_fd()
{
  return (this->_sock);
}

bool
EthSocket::_bind()
{

  if (!this->_sock)
  {
    ZLOG_ERR(std::string("Socket not opened"));
    return (false);
  }

  if (this->GetAddress().GetType() != SOCKET_TYPE::TYPE_UNIX)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  this->_sa = EthAddress(this->GetAddress());

  // Bind address to socket
  int ret = bind(this->_sock, (struct sockaddr*) &this->_sa.sa, sizeof(this->_sa.sa));
  if (ret < 0)
  {
    ZLOG_CRIT("Cannot bind socket: " + this->GetAddress().GetAddress() + ": " +
        std::string(strerror(errno)));
    return (false);
  } // end if

  ZLOG_INFO("Bind on socket: " + ZLOG_INT(this->_sock));

  return (true);

}

ssize_t
EthSocket::_recv()
{

  int nbytes = 0;

  if (this->_sock)
  {
    // Query for the number of bytes ready to be read for use creating socket buffer
    ioctl(this->_sock, FIONREAD, &nbytes);
    if (nbytes)
    {
      EthAddress addr;
      socklen_t len = sizeof(addr.sa);
      Buffer sb(nbytes);
      nbytes = recvfrom(this->_sock, sb.Head(), sb.TotalSize(), 0, (struct sockaddr *) &addr.sa, &len);
      if (sb.Put(nbytes))
      {
        ZLOG_INFO("(" + ZLOG_INT(this->_sock) + ")" + "Sent " + ZLOG_INT(nbytes) +
            "bytes to: " + addr.GetAddress());
        if (!this->rxNotify(addr, sb))
        {
          nbytes = -1;
        }
      }
    }
  }

  return (nbytes);

}

ssize_t
EthSocket::_send(const Address& to_, const Buffer& sb_)
{

  ssize_t nbytes = -1;

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = this->_sock;
  fds[0].events = (POLLOUT | POLLERR);

  int ret = poll(fds, 1, 100);
  if (ret > 0 && (fds[0].revents == POLLOUT))
  {
    EthAddress addr(to_);
    nbytes = sendto(this->_sock, sb_.Head(), sb_.Size(), 0, (struct sockaddr *) &addr.sa, sizeof(addr.sa));
    if (nbytes > 0)
    {
      ZLOG_INFO("(" + ZLOG_INT(this->_sock) + ")" + "Sent " + ZLOG_INT(sb_.Length()) +
          "bytes to: " + addr.GetAddress());
      if (!this->txNotify(to_, sb_))
      {
        nbytes = -1;
      }
    }
    else
    {
      ZLOG_ERR(std::string("Cannot send packet: " + std::string(strerror(errno))));
    }
  }
  return (nbytes);
}


//
//bool
//EthSocket::_bind()
//{
//
//  if (!this->_iface)
//  {
//    ZLOG_ERR("Cannot directly call 'Bind()'; please specify an interface to bind to");
//    return (false);
//  }
//
//  struct sockaddr_ll sockaddr = { 0 };
//
//  sockaddr.sll_family = AF_PACKET;
//  sockaddr.sll_ifindex = this->_iface->GetIfIndex();
//  sockaddr.sll_halen = ETH_ALEN;
//  memset(sockaddr.sll_addr, 0, ETH_ALEN);
//  sockaddr.sll_pkttype = PACKET_HOST;
//
//  FOREACH(auto& fd, this->_sockfd)
//  {
//    sockaddr.sll_protocol = htons(fd.first);
//    ZLOG_DEBUG("Bind on socket: " + ZLOG_INT(fd.second));
//    if (_bind(fd.second, (struct sockaddr*) &sockaddr, sizeof(struct sockaddr_ll)))
//    {
//      ZLOG_ERR("Cannot bind socket: " + this->_iface->GetIfName() + ": " + std::string(strerror(errno)));
//    } // end if
//  }
//
//  // Start listener threads
//  if (!this->_rx_thread.Start() || !this->_tx_thread.Start())
//  {
//    ZLOG_ERR("Error starting listening threads");
//    return (false);
//  }
//
//  return (true);
//
//}
//
//ssize_t
//EthSocket::_recv(const int fd_, zSocket::EthAddress & addr_, zSocket::Buffer & sb_)
//{
//
//  ssize_t n = -1;
//  struct sockaddr_ll src = { 0 };
//  socklen_t srclen = sizeof(src);
//  int flags = 0;
//
//  if (!fd_)
//  {
//    ZLOG_CRIT(std::string("Socket not opened"));
//    return (-1);
//  }
//
//  n = recvfrom(fd_, sb_.Head(), sb_.TotalSize(), 0, (struct sockaddr *) &src, &srclen);
//  if (n > 0)
//  {
//    sb_.Put(n);
//    std::string addr;
//    _mac2str(*(struct hwaddr*)&src.sll_addr, addr);
//    addr_.SetAddress(addr);
//
//    uint8_t* p = sb_.Head();
//    std::string logstr;
//    logstr += "Receiving on socket:\t";
//    logstr += "To:     " + this->GetAddress().GetAddress() + ";\t";
//    logstr += "From:   " + addr_.GetAddress() + ";\t";
//    logstr += "Size:   " + ZLOG_INT(n) + ";\t";
//    logstr += "Family: " + ZLOG_INT(src.sll_family) + ";\t";
//    logstr += "Type:   " + ZLOG_INT(src.sll_pkttype) + ";\t";
//    int proto = htobe16(src.sll_protocol);
//    logstr += "Proto:  " + ZLOG_HEX(proto) + ";\t";
//    ZLOG_INFO(logstr);
//    logstr = "Data (0x00): ";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ";";
//    ZLOG_INFO(logstr);
//    logstr = "Data (0x08): ";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ";";
//    ZLOG_INFO(logstr);
//    logstr = "Data (0x10): ";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ";";
//    ZLOG_INFO(logstr);
//    logstr = "Data (0x18): ";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ";";
//    ZLOG_INFO(logstr);
//    logstr = "Data (0x20): ";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ";";
//    ZLOG_INFO(logstr);
//    logstr = "Data (0x28): ";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ":";
//    logstr += ZLOG_HEX(*p++) + ";";
//    ZLOG_INFO(logstr);
//
//  } // end if
//
//  return (n);
//}
//
//ssize_t
//EthSocket::_send(const int fd_, const zSocket::EthAddress &addr_, zSocket::Buffer &sb_)
//{
//
//  ssize_t n = -1;
//  struct sockaddr_ll src = { 0 };
//
//  if (!fd_)
//  {
//    ZLOG_ERR(std::string("Socket not opened"));
//    return (-1);
//  }
//
//  // Set socket address fields
//  src.sll_ifindex = this->_iface->GetIfIndex();
//
//  // Log info message about message being sent
//  std::string logstr;
//  uint8_t* p = sb_.Head();
//  logstr += "Sending on socket:\t";
//  logstr += "Iface: [" + ZLOG_INT(this->_iface->GetIfIndex()) + "] " + this->_iface->GetIfName() + ";\t";
//  logstr += "To: " + addr_.GetAddress() + ";\t";
////  logstr += "From: " + src->Address() + ";\t";
//  logstr += "Size: " + ZLOG_INT(sb_.Size()) + ";\t";
//  logstr += "Family: " + ZLOG_INT(src.sll_family) + ";\t";
//  logstr += "Type:   " + ZLOG_INT(src.sll_pkttype) + ";\t";
//  int proto = htobe16(src.sll_protocol);
//  logstr += "Proto:  " + ZLOG_HEX(proto) + ";\t";
//  ZLOG_INFO(logstr);
//  logstr = "Data (0x00): ";
//  logstr += ZLOG_HEX(*p++) + ":";
//  logstr += ZLOG_HEX(*p++) + ":";
//  logstr += ZLOG_HEX(*p++) + ":";
//  logstr += ZLOG_HEX(*p++) + ":";
//  logstr += ZLOG_HEX(*p++) + ":";
//  logstr += ZLOG_HEX(*p++) + ":";
//  logstr += ZLOG_HEX(*p++) + ":";
//  logstr += ZLOG_HEX(*p++) + ";";
//  ZLOG_INFO(logstr);
//  logstr = "Data (0x08): ";
//  logstr += ZLOG_HEX(*p++) + ":";
//  logstr += ZLOG_HEX(*p++) + ":";
//  logstr += ZLOG_HEX(*p++) + ":";
//  logstr += ZLOG_HEX(*p++) + ":";
//  logstr += ZLOG_HEX(*p++) + ":";
//  logstr += ZLOG_HEX(*p++) + ":";
//  logstr += ZLOG_HEX(*p++) + ":";
//  logstr += ZLOG_HEX(*p++) + ";";
//  ZLOG_INFO(logstr);
//
//  // Send packet
//  n = sendto(fd_, sb_.Head(), sb_.Size(), 0, (struct sockaddr *) &src, sizeof(src));
//  if (n < 0)
//  {
//    ZLOG_ERR(std::string("Cannot send packet: " + std::string(strerror(errno))));
//  }
//
//  return (n);
//
//}

}
}
