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

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <netinet/in.h>
#include <net/if.h>
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
  char c = 0;
  if (sscanf(addr_.c_str(), "%d%c", &index_, &c) != 1)
  {
    index_ = if_nametoindex(addr_.c_str());
  }
  return (!!index_);
}

static bool
_addr2mac(const std::string& addr_, uint8_t* mac_)
{
  uint8_t mac[ETH_ALEN] = { 0 };
  bool status = _str2mac(addr_, mac);
  if (status)
  {
    memcpy(mac_, mac, ETH_ALEN);
  }
  return (status);
}

static bool
_addr2sa(const std::string &addr_, struct sockaddr_ll &sa_)
{
  bool status = false;

  // Initialize socket address
  memset(&sa_, 0, sizeof(sa_));
  sa_.sll_family = AF_PACKET;
  sa_.sll_pkttype = PACKET_HOST;
  sa_.sll_halen = ETH_ALEN;

  if (!(status = _addr2mac(addr_, sa_.sll_addr)))
  {
    status = _addr2ifindex(addr_, sa_.sll_ifindex);
  }
  return (status);
}

static bool
_sa2addr(const struct sockaddr_ll &sa_, std::string &addr_)
{
  char str[256] = { 0 };
  if ((sa_.sll_ifindex) && if_indextoname(sa_.sll_ifindex, str))
  {
    addr_ = std::string(str);
  }
  else
  {
    _mac2str(sa_.sll_addr, addr_);
  }
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
    Socket(SOCKET_TYPE::TYPE_ETH)
{
  // Create a AF_INET socket
  this->fd = socket( PF_PACKET, SOCK_RAW, htons(proto_));
  if (this->fd > 0)
  {
    ZLOG_INFO("Socket created: " + ZLOG_INT(this->fd));
  }
  else
  {
    this->fd = 0;
    ZLOG_ERR("Cannot create socket: " + std::string(strerror(errno)));
  }
}

EthSocket::~EthSocket()
{
  // Make sure the socket is unregistered from all handlers
  if (!this->_handler_list.empty())
  {
    fprintf(stderr, "BUG: Socket registered with handler, not closing FD\n");
  }
  else
  {
    // Close socket
    ZLOG_INFO("Closing socket: " + ZLOG_INT(this->fd));
    if (this->fd)
    {
      close(this->fd);
      this->fd = 0;
    } // end if
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
    if (getsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &optval, &optlen) < 0)
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
    if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
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

bool
EthSocket::_bind()
{

  if (!this->fd)
  {
    ZLOG_ERR(std::string("Socket not opened"));
    return (false);
  }

  if (this->GetAddress().GetType() != SOCKET_TYPE::TYPE_ETH)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  this->_sa = EthAddress(this->GetAddress());

  // Bind address to socket
  int ret = bind(this->fd, (struct sockaddr*) &this->_sa.sa, sizeof(this->_sa.sa));
  if (ret < 0)
  {
    ZLOG_CRIT("Cannot bind socket: " + this->_sa.GetAddress() + ": " + std::string(strerror(errno)));
    return (false);
  } // end if

  ZLOG_INFO("Bind on socket: " + ZLOG_INT(this->fd));

  return (true);

}

ssize_t
EthSocket::_recv()
{

  int nbytes = 0;

  if (this->fd)
  {
    // Query for the number of bytes ready to be read for use creating socket buffer
    ioctl(this->fd, FIONREAD, &nbytes);
    if (nbytes)
    {
      struct sockaddr_ll src;
      socklen_t len = sizeof(src);
      Buffer sb(nbytes);
      nbytes = recvfrom(this->fd, sb.Head(), sb.TotalSize(), 0, (struct sockaddr *) &src, &len);
      if ((nbytes > 0) && sb.Put(nbytes))
      {
        EthAddress addr(src);
        ZLOG_INFO("(" + ZLOG_INT(this->fd) + ") " + "Received " + ZLOG_INT(nbytes) +
            " bytes from: " + addr.GetAddress());
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
  fds[0].fd = this->fd;
  fds[0].events = (POLLOUT | POLLERR);

  int ret = poll(fds, 1, 100);
  if (ret > 0 && (fds[0].revents == POLLOUT))
  {
    EthAddress addr(to_);
    nbytes = sendto(this->fd, sb_.Head(), sb_.Size(), 0, (struct sockaddr *) &addr.sa, sizeof(addr.sa));
    if (nbytes > 0)
    {
      ZLOG_INFO("(" + ZLOG_INT(this->fd) + ") " + "Sent " + ZLOG_INT(sb_.Length()) +
          " bytes to: " + addr.GetAddress());
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

}
}
