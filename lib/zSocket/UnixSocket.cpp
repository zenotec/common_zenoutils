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
#include <sys/un.h>
#include <netinet/in.h>
#include <poll.h>

#include <zutils/zLog.h>
#include <zutils/zSocket.h>
#include <zutils/zUnixSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

static bool
_addr2sa(const std::string &addr_, struct sockaddr_un &sa_)
{
  bool status = false;
  if (!addr_.empty())
  {
    size_t len = (sizeof(sa_.sun_path) - 1); // subtract one for null terminator
    sa_.sun_family = AF_UNIX;
    status = (sa_.sun_path == strncpy(sa_.sun_path, addr_.c_str(), len));
  }
  return (status);
}

static bool
_sa2addr(const struct sockaddr_un &sa_, std::string &addr_)
{
  addr_ = std::string(sa_.sun_path);
  return (!addr_.empty());
}

//**********************************************************************
// Class: zSocket::UnixAddress
//**********************************************************************

UnixAddress::UnixAddress(const std::string &addr_) :
    Address(SOCKET_TYPE::TYPE_UNIX, addr_), sa { 0 }
{
  _addr2sa(addr_, this->sa);
}

UnixAddress::UnixAddress(const Address &addr_) :
    Address(SOCKET_TYPE::TYPE_UNIX), sa { 0 }
{
  if (addr_.GetType() == this->GetType())
  {
    this->SetAddress(addr_.GetAddress());
  }
}

UnixAddress::UnixAddress(const struct sockaddr_un& sa_) :
        Address(SOCKET_TYPE::TYPE_UNIX), sa (sa_)
{
  this->SetAddress(this->GetAddress());
}

UnixAddress::~UnixAddress()
{
}

std::string
UnixAddress::GetAddress() const
{
  std::string addr;
  _sa2addr(this->sa, addr);
  return (addr);
}

bool
UnixAddress::SetAddress(const std::string& addr_)
{
  bool status = false;
  if (_addr2sa(addr_, this->sa))
  {
    status = Address::SetAddress(addr_);
  }
  return (status);
}

void
UnixAddress::Display() const
{
  Address::Display();
}

//**********************************************************************
// Class: zSocket::UnixSocket
//**********************************************************************

UnixSocket::UnixSocket() :
    Socket(SOCKET_TYPE::TYPE_UNIX)
{
  // Create a AF_INET socket
  this->fd = socket( AF_UNIX, (SOCK_DGRAM | SOCK_NONBLOCK), 0);
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

UnixSocket::~UnixSocket()
{
  // Make sure the socket is unregistered from all handlers
  if (!this->_handler_list.empty())
  {
    fprintf(stderr, "BUG: Socket registered with handler, not closing FD\n");
  }
  else
  {
    // Close socket
    if (this->fd)
    {
      ZLOG_INFO("Closing socket: " + ZLOG_INT(this->fd));
      unlink(this->_sa.sa.sun_path);
      close(this->fd);
      this->fd = 0;
    } // end if
  }
}

bool
UnixSocket::_bind()
{

  if (!this->fd)
  {
    ZLOG_ERR(std::string("Socket not opened"));
    return (false);
  }

  if (this->GetAddress().GetType() != SOCKET_TYPE::TYPE_UNIX)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  this->_sa = UnixAddress(this->GetAddress());

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
UnixSocket::_recv()
{

  int nbytes = 0;

  if (this->fd)
  {
    // Query for the number of bytes ready to be read for use creating socket buffer
    ioctl(this->fd, FIONREAD, &nbytes);
    if (nbytes)
    {
      struct sockaddr_un src;
      socklen_t len = sizeof(src);
      Buffer sb(nbytes);
      nbytes = recvfrom(this->fd, sb.Head(), sb.TotalSize(), 0, (struct sockaddr *) &src, &len);
      if ((nbytes > 0) && sb.Put(nbytes))
      {
        UnixAddress addr(src);
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
UnixSocket::_send(const Address& to_, const Buffer& sb_)
{

  ssize_t nbytes = -1;

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = this->fd;
  fds[0].events = (POLLOUT | POLLERR);

  int ret = poll(fds, 1, 100);
  if (ret > 0 && (fds[0].revents == POLLOUT))
  {
    UnixAddress addr(to_);
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
