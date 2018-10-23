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

//**********************************************************************
// Class: zSocket::UnixSocketRx
//**********************************************************************

void
UnixSocketRx::Run(zThread::ThreadArg *arg_)
{
  class UnixSocket* sock = (class UnixSocket*)arg_;

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = sock->fd;
  fds[0].events = (POLLIN | POLLERR);

  while (!this->Exit())
  {
    int ret = poll(fds, 1, 100);

    if (ret == 1)
    {
      if (fds[0].revents & POLLIN)
      {
        sock->rxq.Push(sock->recv());
      }
    }
    else if (ret == 0)
    {
      continue;
    }
    else
    {
      if (errno == -EINTR)
      {
        // A signal interrupted poll; exit flag should be set
        fprintf(stderr, "Poll interrupted by signal\n"); // TODO: Debug code, remove when no longer needed
        ZLOG_INFO("Socket poll interrupted by signal");
      }
      else
      {
        fprintf(stderr, "Error receiving on socket: %d\n", sock->GetFd()); // TODO: Debug code, remove when no longer needed
        fprintf(stderr, "\t%s", strerror(errno));
        ZLOG_INFO("Error receiving on socket: " + ZLOG_INT(sock->GetFd()));
      }
    }

  }
}

//**********************************************************************
// Class: zSocket::UnixSocketTx
//**********************************************************************

void
UnixSocketTx::Run(zThread::ThreadArg *arg_)
{
  class UnixSocket* sock = (class UnixSocket*)arg_;

  while (!this->Exit())
  {
    if (sock->txq.TimedWait(100))
    {
      sock->rxq.Push(sock->send());
    }
  }

}

//**********************************************************************
// Class: zSocket::UnixSocket
//**********************************************************************

UnixSocket::UnixSocket() :
    Socket(SOCKET_TYPE::TYPE_UNIX),
    _rxthread(&_rxfunc, this),
    _txthread(&_txfunc, this),
    fd(0)
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

  this->_rxthread.Stop();
  this->_txthread.Stop();

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
      unlink(this->_addr.GetSA().sun_path);
      close(this->fd);
      this->fd = 0;
    } // end if
  }
}

const Address&
UnixSocket::GetAddress() const
{
  return (this->_addr);
}

bool
UnixSocket::Bind(const Address& addr_)
{

  if (!this->fd)
  {
    ZLOG_ERR(std::string("Socket not opened"));
    return (false);
  }

  if (addr_.GetType() != Address::TYPE_UNIX)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  this->_addr = UnixAddress(addr_);

  // Bind address to socket
  struct sockaddr_un sa(this->_addr.GetSA());
  int ret = bind(this->fd, (struct sockaddr*) &sa, sizeof(sa));
  if (ret < 0)
  {
    ZLOG_CRIT("Cannot bind socket: " + this->_addr.GetAddress() + ": " + std::string(strerror(errno)));
    return (false);
  } // end if

  ZLOG_INFO("Bind on socket: " + ZLOG_INT(this->fd));

  this->_rxthread.Start();
  this->_txthread.Start();

  return (true);
}

SHARED_PTR(zSocket::Notification)
UnixSocket::recv()
{

  SHARED_PTR(zSocket::Notification) n(new zSocket::Notification(*this));
  ssize_t nbytes = 0;

  if (this->fd)
  {
    // Query for the number of bytes ready to be read for use creating socket buffer
    ioctl(this->fd, FIONREAD, &nbytes);
    if (nbytes)
    {

      struct sockaddr_un src;
      socklen_t len = sizeof(src);
      SHARED_PTR(Buffer) sb(new Buffer(nbytes));

      nbytes = recvfrom(this->fd, sb->Head(), sb->TotalSize(), 0, (struct sockaddr *) &src, &len);
      if ((nbytes > 0) && sb->Put(nbytes))
      {
        struct timespec ts = { 0 };
        ioctl(this->fd, SIOCGSTAMPNS, &ts);
        sb->Timestamp(ts);
        n->SetSubType(Notification::SUBTYPE_PKT_RCVD);
        n->SetDstAddress(SHARED_PTR(UnixAddress)(new UnixAddress(this->GetAddress())));
        n->SetSrcAddress(SHARED_PTR(UnixAddress)(new UnixAddress(src)));
        n->SetBuffer(sb);
        // NOTE: frame is initialized by optional adapter socket
        ZLOG_DEBUG("(" + ZLOG_INT(this->fd) + ") " + "Received " + ZLOG_INT(nbytes) +
            " bytes from: " + n->GetSrcAddress()->GetAddress());
      }
      else
      {
        n->SetSubType(Notification::SUBTYPE_PKT_ERR);
        ZLOG_ERR(std::string("Cannot receive packet: " + std::string(strerror(errno))));
      }
    }
  }

  return (n);

}

SHARED_PTR(zSocket::Notification)
UnixSocket::send()
{

  // Initialize notification
  SHARED_PTR(zSocket::Notification) n(this->txq.Front());
  this->txq.Pop();
  SHARED_PTR(UnixAddress) addr(new UnixAddress(*n->GetDstAddress()));
  zSocket::Buffer sb(*n->GetBuffer());

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = this->fd;
  fds[0].events = (POLLOUT | POLLERR);

  // Poll for transmit ready
  int ret = poll(fds, 1, 100);
  if (ret > 0 && (fds[0].revents == POLLOUT))
  {
    // Send
    struct sockaddr_un dst(addr->GetSA());
    ssize_t nbytes = sendto(this->fd, sb.Head(), sb.Size(), 0, (struct sockaddr *) &dst, sizeof(dst));
    if (nbytes > 0)
    {
      ZLOG_DEBUG("(" + ZLOG_INT(this->fd) + ") " + "Sent " + ZLOG_INT(sb.Length()) +
          " bytes to: " + addr->GetAddress());
      n->SetSubType(Notification::SUBTYPE_PKT_SENT);
    }
    else
    {
      n->SetSubType(Notification::SUBTYPE_PKT_ERR);
      ZLOG_ERR(std::string("Cannot send packet: " + std::string(strerror(errno))));
    }
  }
  else
  {
    n->SetSubType(Notification::SUBTYPE_PKT_ERR);
    ZLOG_ERR(std::string("Cannot send packet: " + std::string(strerror(errno))));
  }

  return (n);

}

}
}
