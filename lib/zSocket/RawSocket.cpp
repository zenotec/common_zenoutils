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

// libc includes
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <net/if.h>
#include <netinet/in.h>
#include <poll.h>

// libc++ includes

// libzutils includes

#include <zutils/zUtils.h>
#include <zutils/zLog.h>
#include <zutils/zThread.h>
#include <zutils/zSocket.h>
#include <zutils/zRawAddress.h>
#include <zutils/zRawSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// Class: zSocket::RawSocketRx
//**********************************************************************

void
RawSocketRx::Run(zThread::ThreadArg *arg_)
{
  class RawSocket* sock = (class RawSocket*)arg_;

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
        fprintf(stderr, "Error receiving on socket: %d\n", sock->fd); // TODO: Debug code, remove when no longer needed
        fprintf(stderr, "\t%s", strerror(errno));
        ZLOG_INFO("Error receiving on socket: " + ZLOG_INT(sock->fd));
      }
    }

  }
}

//**********************************************************************
// Class: zSocket::RawSocketTx
//**********************************************************************

void
RawSocketTx::Run(zThread::ThreadArg *arg_)
{
  class RawSocket* sock = (class RawSocket*)arg_;

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = sock->fd;
  fds[0].events = (POLLOUT | POLLERR);

  while (!this->Exit())
  {
    if (sock->txq.TimedWait(100))
    {
      int retry = 5;
      SHARED_PTR(zSocket::Notification) n(sock->txq.Front());
      sock->txq.Pop();
      while (!this->Exit() && --retry)
      {
        int ret = poll(fds, 1, 100);

        if ((ret == 1) && (fds[0].revents & POLLOUT))
        {
          sock->rxq.Push(sock->send(n));
          break;
        }
        else
        {
          fprintf(stderr, "BUG: Timed out waiting to send frame...trying again: %d\n", retry);
        }
      }
    }
  }

}

//**********************************************************************
// Class: zSocket::RawSocket
//**********************************************************************

RawSocket::RawSocket(const RawSocket::PROTO proto_, const RawSocket::PACKETTYPE ptype_) :
    Socket(SOCKET_TYPE::TYPE_RAW),
    _rxthread(&_rxfunc, this),
    _txthread(&_txfunc, this),
    fd(0), _proto(proto_), _ptype(ptype_)
{
  // Create a AF_INET socket
  this->fd = socket( PF_PACKET, (SOCK_RAW | SOCK_NONBLOCK), htons(proto_));
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

RawSocket::~RawSocket()
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
    ZLOG_INFO("Closing socket: " + ZLOG_INT(this->fd));
    if (this->fd)
    {
      close(this->fd);
      this->fd = 0;
    } // end if
  }
}

bool
RawSocket::Getopt(Socket::OPTIONS opt_)
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
      status = (optval == 0);
    }
    break;
  }
  case Socket::OPTIONS_PROMISC:
  {
    RawAddress raddr(this->GetAddress());
    struct packet_mreq mr = { 0 };
    break;
  }
  default:
    break;
  }
  return (status);
}

bool
RawSocket::Setopt(Socket::OPTIONS opt_)
{
  bool status = false;
  switch (opt_)
  {
  case Socket::OPTIONS_ALLOW_REUSE:
  {
    // Enable reuse of socket
    int optval = 0;
    if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == 0)
    {
      status = true;
    }
    else
    {
      ZLOG_ERR("Cannot set socket option: " + std::string(strerror(errno)));
    }
    break;
  }
  case Socket::OPTIONS_PROMISC:
  {
    RawAddress raddr(this->GetAddress());
    struct packet_mreq mr = { 0 };
    mr.mr_ifindex = raddr.GetSA().sll_ifindex;
    mr.mr_type = PACKET_MR_PROMISC;
    if (setsockopt(this->fd, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == 0)
    {
      status = true;
    }
    else
    {
      ZLOG_ERR("Cannot set socket option: " + std::string(strerror(errno)));
    }
    break;
  }
  default:
    break;
  }
  return (status);
}

bool
RawSocket::Bind(const Address& addr_)
{

  bool status = false;

  if (!this->fd)
  {
    ZLOG_ERR("Socket not opened");
    return (false);
  }

  if (addr_.GetType() != Address::TYPE_RAW)
  {
    ZLOG_CRIT("Invalid socket address");
    return (false);
  }

  // Initialize socket address of interface to bind to
  RawAddress addr(addr_);
  struct sockaddr_ll sa(addr.GetSA());
  sa.sll_family = AF_PACKET;
  sa.sll_protocol = htons(this->_proto);
  sa.sll_pkttype = this->_ptype;

  // Bind address to socket
  int ret = bind(this->fd, (struct sockaddr*) &sa, sizeof(sa));
  if (ret < 0)
  {
    ZLOG_ERR("Cannot bind socket: " + addr_.GetAddress() + ": " + std::string(strerror(errno)));
    return (false);
  } // end if

  ZLOG_INFO("Bind on socket: [" + ZLOG_INT(this->fd) + "]" + addr_.GetAddress());

  this->_rxthread.Start();
  this->_txthread.Start();

  return (this->SetAddress(addr_) && this->_addr.SetSA(sa));

}

SHARED_PTR(zSocket::Notification)
RawSocket::recv()
{

  SHARED_PTR(zSocket::Notification) n(new zSocket::Notification(*this));
  ssize_t nbytes = 0;

  if (this->fd)
  {
    // Query for the number of bytes ready to be read for use creating socket buffer
    ioctl(this->fd, FIONREAD, &nbytes);
    if (nbytes)
    {

      struct sockaddr_ll src;
      socklen_t len = sizeof(src);
      SHARED_PTR(Buffer) sb(new Buffer(nbytes));

      nbytes = recvfrom(this->fd, sb->Head(), sb->TotalSize(), 0, (struct sockaddr *) &src, &len);
      if ((nbytes > 0) && sb->Put(nbytes))
      {
        struct timespec ts = { 0 };
        ioctl(this->fd, SIOCGSTAMPNS, &ts);
        sb->Timestamp(ts);
        n->SetSubType(Notification::SUBTYPE_PKT_RCVD);
        n->SetDstAddress(SHARED_PTR(RawAddress)(new RawAddress(this->GetAddress())));
        n->SetSrcAddress(SHARED_PTR(RawAddress)(new RawAddress(src)));
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
RawSocket::send(SHARED_PTR(zSocket::Notification) n_)
{

  // Initialize notification
  zSocket::Address addr(*n_->GetDstAddress());
  zSocket::Buffer sb(*n_->GetBuffer());

  struct sockaddr_ll sa(this->_addr.GetSA());
  sa.sll_family = AF_PACKET;
  sa.sll_protocol = htons(this->_proto);
  sa.sll_pkttype = this->_ptype;

  ssize_t nbytes = sendto(this->fd, sb.Head(), sb.Size(), 0, (struct sockaddr *) &sa, sizeof(sa));
  if (nbytes == sb.Size())
  {
    ZLOG_DEBUG("(" + ZLOG_INT(this->GetFd()) + ") " + "Sent " + ZLOG_INT(sb.Length()) +
        " bytes to: " + addr.GetAddress());
    n_->SetSubType(Notification::SUBTYPE_PKT_SENT);
  }
  else
  {
    ZLOG_ERR(std::string("Cannot send packet: " + std::string(strerror(errno))));
    n_->SetSubType(Notification::SUBTYPE_PKT_ERR);
  }

  return (n_);

}

}
}
