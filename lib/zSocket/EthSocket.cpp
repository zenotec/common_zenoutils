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
_str2mac(const std::string& addr_, struct eth_addr& hwaddr_)
{
  return (sscanf(addr_.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
      &hwaddr_.addr[0], &hwaddr_.addr[1], &hwaddr_.addr[2],
      &hwaddr_.addr[3], &hwaddr_.addr[4], &hwaddr_.addr[5]) == ETH_ALEN);
}

static bool
_mac2str(const struct eth_addr& hwaddr_, std::string& addr_)
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

static bool
_get_ifindex(const std::string &name_, int& index_)
{
  bool status = false;
  int sock = -1;
  struct ifreq ifr = { 0 };

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(ifr.ifr_name, name_.c_str(), IFNAMSIZ);

    // Query interface index
    if (ioctl(sock, SIOCGIFINDEX, &ifr) == 0)
    {
      index_ = ifr.ifr_ifindex;
      status = true;
    }
    else
    {
      printf("Error: Cannot query interface index: %s\n", name_.c_str());
    }
    close(sock);
  }
  return (status);
}

static uint32_t
_get_flags(const std::string &name_)
{
  int sock = -1;
  struct ifreq ifr = { 0 };
  uint32_t flags = 0;

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(ifr.ifr_name, name_.c_str(), IFNAMSIZ);

    // Query interface flags
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0)
    {
      flags = ifr.ifr_flags;
    }
    close(sock);
  }
  return (flags);
}

static bool
_set_flags(const std::string &name_, const uint32_t flags_)
{

  bool status = false;
  int sock = -1;
  struct ifreq ifr = { 0 };
  uint32_t flags = 0;

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(ifr.ifr_name, name_.c_str(), IFNAMSIZ);

    // Query interface flags
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0)
    {
      flags = ifr.ifr_flags;
      printf("Getting flags: 0x%08x\n", flags);
      flags |= flags_;
      if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0)
      {
        printf("Setting flags: 0x%08x\n", flags);
        status = true;
      }
    }
    close(sock);
  }
  return (status);
}

static bool
_clr_flags(const std::string &name_, const uint32_t flags_)
{

  bool status = false;
  int sock = -1;
  struct ifreq ifr = { 0 };
  uint32_t flags = 0;

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(ifr.ifr_name, name_.c_str(), IFNAMSIZ);

    // Query interface flags
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0)
    {
      flags = ifr.ifr_flags;
      flags &= ~flags_;
      if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0)
      {
        status = true;
      }
    }
    close(sock);
  }
  return (status);
}

static bool
_get_mac(const std::string &name_, struct eth_addr& addr_)
{
  bool status = false;
  int sock = -1;
  struct ifreq ifr = { 0 };

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(ifr.ifr_name, name_.c_str(), IFNAMSIZ);

    // Query interface index
    if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0)
    {
      memcpy(&addr_, &ifr.ifr_hwaddr.sa_data, ETH_ALEN);
      status = true;
    }
    close(sock);
  }
  return (status);
}

static bool
_ethaddr2sock(const EthAddress& addr_, struct sockaddr_ll& sock_)
{
  bool status = false;
  struct eth_addr hwaddr = { 0 };
  memset(&sock_, 0, sizeof(struct sockaddr_ll));
  if (_str2mac(addr_.Address(), hwaddr))
  {
    sock_.sll_family = AF_PACKET;
    sock_.sll_ifindex = addr_.IfIndex();
    sock_.sll_halen = ETH_ALEN;
    memcpy(sock_.sll_addr, &hwaddr, ETH_ALEN);
    sock_.sll_protocol = htons(ETH_P_ALL);
    sock_.sll_pkttype = PACKET_HOST;
    status = true;
  }
  return (status);
}

static bool
_find_byname(const std::string& name_, struct eth_addr& addr_)
{
  return (_get_mac(name_, addr_));
}

static bool
_find_byhwaddr(const std::string& addr_, std::string& name_)
{
  bool status = false;
  struct ifaddrs *ifp, *tmp;

  getifaddrs(&ifp);
  tmp = ifp;

  while (tmp)
  {
    if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET)
    {

      struct eth_addr mac = { 0 };
      std::string addr;

      _get_mac(std::string(tmp->ifa_name), mac);
      _mac2str(mac, addr);

      if (addr_ == addr)
      {
        name_ = std::string(tmp->ifa_name);
        status = true;
        break;
      }
    }
    tmp = tmp->ifa_next;
  }
  freeifaddrs(ifp);
  return (status);
}

//**********************************************************************
// Class: zSocket::EthAddress
//**********************************************************************

EthAddress::EthAddress(const std::string &addr_) :
    SocketAddress(SocketType::TYPE_ETH, addr_)
{
  // Address is hardware address
  if (_str2mac(this->_addr, this->_mac))
  {
    _mac2str(this->_mac, this->_addr); // Ensure consistent case
    _find_byhwaddr(this->_addr, this->_name);
  }
  // Address is actually interface name
  else if (_find_byname(this->_addr, this->_mac))
  {
    this->_name = this->_addr;
    _mac2str(this->_mac, this->_addr);
  }
}

EthAddress::EthAddress(SocketAddress &addr_) :
    SocketAddress(addr_)
{
  // Address is hardware address
  if (_str2mac(this->_addr, this->_mac))
  {
    _mac2str(this->_mac, this->_addr); // Ensure consistent case
    _find_byhwaddr(this->_addr, this->_name);
  }
  // Address is actually interface name
  else if (_find_byname(this->_addr, this->_mac))
  {
    this->_name = this->_addr;
    _mac2str(this->_mac, this->_addr);
  }
}

EthAddress::EthAddress(const SocketAddress &addr_) :
    SocketAddress(addr_)
{
  // Address is hardware address
  if (_str2mac(this->_addr, this->_mac))
  {
    _mac2str(this->_mac, this->_addr); // Ensure consistent case
    _find_byhwaddr(this->_addr, this->_name);
  }
  // Address is actually interface name
  else if (_find_byname(this->_addr, this->_mac))
  {
    this->_name = this->_addr;
    _mac2str(this->_mac, this->_addr);
  }
}

EthAddress::~EthAddress()
{
}

std::string
EthAddress::IfName() const
{
  return (this->_name);
}

int
EthAddress::IfIndex() const
{
  int index = -1;
  _get_ifindex(this->_name, index);
  return (index);
}

struct eth_addr
EthAddress::Mac() const
{
  return (this->_mac);
}

bool
EthAddress::verify(const SocketType type_, const std::string &addr_)
{
  struct eth_addr hwaddr = { 0 };
  return ((type_ == SocketType::TYPE_ETH) && _str2mac(addr_, hwaddr));
}

//**********************************************************************
// zSocket::EthSocketRecv Class
//**********************************************************************

void
EthSocketRecv::Run(zThread::ThreadArg *arg_)
{
  EthSocket *sock = (EthSocket *) arg_;
  ssize_t bytes = -1;

  if (!sock || !sock->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return;
  }

  ZLOG_DEBUG("Waiting for RX data on ETH socket: " + zLog::IntStr(sock->_sock));

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = sock->_sock;
  fds[0].events = (POLLIN | POLLERR);

  while (!this->Exit())
  {
    // Poll for received data
    int ret = poll(fds, 1, 5000);
    if (ret > 0)
    {
      if (fds[0].revents == POLLIN)
      {
        SHARED_PTR(EthAddress)addr(new EthAddress);
        SHARED_PTR(SocketBuffer)sb(new SocketBuffer);
        bytes = sock->_recv(*addr, *sb);
        if (bytes > 0)
        {
          ZLOG_INFO("Received packet on socket: " + ZLOG_INT(sock->_sock));
          SocketAddressBufferPair p(addr, sb);
          sock->rxbuf(p);
        }
      }
      else if (fds[0].revents == POLLERR)
      {
        ZLOG_WARN("POLLERR");
      }
    } // end if

    if (ret < 0)
    {
      ZLOG_ERR("Error polling on socket: " + std::string(strerror(errno)));
    } // end if

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

  if (!sock || !sock->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return;
  }

  ZLOG_DEBUG("Waiting for TX data on ETH socket: " + zLog::IntStr(sock->_sock));

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = sock->_sock;
  fds[0].events = (POLLOUT | POLLERR);

  while (!this->Exit())
  {

    // Wait for data to send
    if (sock->txbuf(p, 100))
    {
      int ret = poll(fds, 1, 100);
      if (ret > 0 && (fds[0].revents == POLLOUT))
      {
        ZLOG_DEBUG("Sending packet: " + p.first->Address() +
            "(" + ZLOG_INT(p.second->Size()) + ")");
        if (sock->_send(*p.first, *p.second) != p.second->Size())
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
    Socket(SocketType::TYPE_ETH), _rx_thread(&this->_rx_func, this),
        _tx_thread(&this->_tx_func, this), _sock(0)
{
}

EthSocket::~EthSocket()
{
  this->Close();
}

bool
EthSocket::Open()
{

  if (!this->_sock)
  {
    int sockopt = 0;

    // Create a AF_ETH socket
    this->_sock = socket( PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (this->_sock < 0)
    {
      ZLOG_CRIT("Cannot create socket: " + std::string(strerror(errno)));
      return (false);
    } // end if

    ZLOG_DEBUG("Opening socket: " + ZLOG_INT(this->_sock));

    // Enable reuse of socket
    if (setsockopt(this->_sock, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) == -1)
    {
      ZLOG_CRIT("Cannot configure socket: " + std::string(strerror(errno)));
      return (false);
    }

  } // end if
  else
  {
    ZLOG_WARN("Socket already open");
  }
  return (true);
}

void
EthSocket::Close()
{
  ZLOG_DEBUG("Closing socket: " + ZLOG_INT(this->_sock));
  // Close socket
  if (this->_sock)
  {
    if (this->_rx_thread.Stop() && this->_tx_thread.Stop())
    {
      ZLOG_DEBUG("Socket Closed: " + ZLOG_INT(this->_sock));
      close(this->_sock);
      this->_sock = 0;
    } // end if
  }
  else
  {
    ZLOG_WARN("Socket not open");
  }
}

bool
EthSocket::_bind()
{

  EthAddress* addr = (EthAddress*) this->_addr;
  struct sockaddr_ll sockaddr = { 0 };

  ZLOG_DEBUG("Bind on socket: " + ZLOG_INT(this->_sock));

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (false);
  }

  if (this->Address().Type() != SocketType::TYPE_ETH)
  {
    ZLOG_CRIT(std::string("Invalid socket address type"));
    return (false);
  }

  // Convert address to socket address
  if (!_ethaddr2sock(*addr, sockaddr))
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

//  // Enable promiscuous mode
//  if (!_set_flags(addr->IfName(), IFF_PROMISC))
//  {
//    ZLOG_ERR("Error: Cannot enable promiscuous mode");
//    return (false);
//  }

  // Bind address to socket
  if (setsockopt(this->_sock, SOL_SOCKET, SO_BINDTODEVICE, addr->IfName().c_str(), IFNAMSIZ-1) == -1)
//  if (bind(this->_sock, (struct sockaddr*) &sockaddr, sizeof(struct sockaddr_ll)))
  {
    ZLOG_CRIT("Cannot bind socket: " + this->Address().Address() +
        ": " + std::string(strerror(errno)));
    return (false);
  } // end if

  // Start listener threads
  if (!this->_rx_thread.Start() || !this->_tx_thread.Start())
  {
    ZLOG_ERR("Error starting listening threads");
    return (false);
  }

  return (true);

}

ssize_t
EthSocket::_recv(zSocket::EthAddress & addr_, zSocket::SocketBuffer & sb_)
{

  ssize_t n = -1;
  struct sockaddr_ll src = { 0 };
  socklen_t srclen = sizeof(src);
  int flags = 0;

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (-1);
  }

  if (this->Address().Type() != SocketType::TYPE_ETH)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (-1);
  }

  n = recvfrom(this->_sock, sb_.Head(), sb_.TotalSize(), 0, (struct sockaddr *) &src, &srclen);
  if (n > 0)
  {
    sb_.Put(n);
    std::string addr;
    _mac2str(*(struct eth_addr*)&src.sll_addr, addr);
    addr_.Address(addr);

    uint8_t* p = sb_.Head();
    std::string logstr;
    logstr += "Receiving on socket:\t";
    logstr += "To:     " + this->Address().Address() + ";\t";
    logstr += "From:   " + addr_.Address() + ";\t";
    logstr += "Size:   " + ZLOG_INT(n) + ";\t";
    logstr += "Family: " + ZLOG_INT(src.sll_family) + ";\t";
    logstr += "Type:   " + ZLOG_INT(src.sll_pkttype) + ";\t";
    logstr += "Proto:  " + ZLOG_INT(src.sll_protocol) + ";\t";
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
EthSocket::_send(const zSocket::EthAddress &addr_, zSocket::SocketBuffer &sb_)
{

  ssize_t n = -1;
  struct sockaddr_ll dst = { 0 };

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (-1);
  }

  if (this->Address().Type() != SocketType::TYPE_ETH)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (-1);
  }

  if (addr_.Type() != SocketType::TYPE_ETH)
  {
    ZLOG_CRIT(std::string("Invalid destination address type"));
    return (-1);
  }

  // Convert destination address to socket address
  if (!_ethaddr2sock(addr_, dst))
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (-1);
  }

  // Log info message about message being sent
  std::string logstr;
  uint8_t* p = sb_.Head();
  logstr += "Sending on socket:\t";
  logstr += "To: " + addr_.Address() + ";\t";
  logstr += "From: " + this->Address().Address() + ";\t";
  logstr += "Size: " + ZLOG_INT(sb_.Size()) + ";\t";
  logstr += "Data: ";
  logstr += ZLOG_HEX(*p++)+ ":";
  logstr += ZLOG_HEX(*p++)+ ":";
  logstr += ZLOG_HEX(*p++)+ ":";
  logstr += ZLOG_HEX(*p++)+ ":";
  logstr += ZLOG_HEX(*p++)+ ":";
  logstr += ZLOG_HEX(*p++)+ ":";
  logstr += ZLOG_HEX(*p++)+ ":";
  logstr += ZLOG_HEX(*p++)+ ":";
  logstr += ";";
  ZLOG_INFO(logstr);

  // Send packet
  n = sendto(this->_sock, sb_.Head(), sb_.Size(), 0, (struct sockaddr *) &dst, sizeof(dst));
  if (n < 0)
  {
    ZLOG_ERR(std::string("Cannot send packet: " + std::string(strerror(errno))));
  }

  return (n);

}

}
}
