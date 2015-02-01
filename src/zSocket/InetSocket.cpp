//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Inet.cpp
//    Description:
//
//*****************************************************************************

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#include <zutils/zLog.h>
#include <zutils/zInetSocket.h>

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// InetSocket Class
//**********************************************************************

InetSocket::InetSocket(const Address &addr_) :
    zSocket::Socket(addr_), _thread(this, this), _sock(0), _inetaddr(InetAddress(addr_))
{
  ZLOG_DEBUG("InetSocket::InetSocket: New socket: " + zLog::IntStr(this->_sock));
}

InetSocket::~InetSocket()
{
  ZLOG_DEBUG("InetSocket::~InetSocket: Destroying socket: " + zLog::IntStr(this->_sock));
  this->_close();
}

void *
InetSocket::ThreadFunction(void *arg_)
{
  InetSocket *self = (InetSocket *) arg_;
  ssize_t bytes = -1;

  if (!self->_sock)
  {
    ZLOG_CRIT(std::string("InetSocket::ThreadFunction: Socket not opened"));
    return ((void*) -1);
  }

  fd_set rxFds;
  struct timeval to;

  // Setup for select loop
  FD_ZERO(&rxFds);
  FD_SET(self->_sock, &rxFds);
  to.tv_sec = 0;
  to.tv_usec = 10000;

  // Select on socket
  int ret = select(self->_sock + 1, &rxFds, NULL, NULL, &to);
  if (ret > 0 && FD_ISSET(this->_sock, &rxFds))
  {
    Address addr;
    Buffer *sb = new Buffer;
    bytes = this->_recv(addr, *sb);
    if (bytes > 0)
    {
      sb->Put(bytes);
      this->Push(std::make_pair(addr, sb));
    }
  } // end if

  if (ret < 0)
  {
    ZLOG_ERR("InetSocket::ThreadFunction: Cannot select on socket: " + std::string(strerror(errno)));
  } // end if

  return (0);

}

bool
InetSocket::_open()
{
  ZLOG_DEBUG("InetSocket::_open: Opening socket: " + zLog::IntStr(this->_sock));

  // Create a AF_INET socket
  if (!this->_sock)
  {
    this->_sock = socket(AF_INET, (SOCK_DGRAM | SOCK_NONBLOCK), IPPROTO_UDP);
    if (this->_sock < 0)
    {
      ZLOG_CRIT("InetSocket::_open: Cannot create socket: " + std::string(strerror(errno)));
      return (false);
    } // end if
  } // end if
  else
  {
    ZLOG_WARN("InetSocket::_open: Socket already open");
  }
  return (true);
}

void
InetSocket::_close()
{
  ZLOG_DEBUG("InetSocket::_close: Closing socket: " + zLog::IntStr(this->_sock));
  // Close socket
  if (this->_sock && this->_thread.Join(1000))
  {
    ZLOG_DEBUG("InetSocket::_close: Socket Closed: " + zLog::IntStr(this->_sock));
    close(this->_sock);
    this->_sock = 0;
  } // end if
}

bool
InetSocket::_bind()
{
  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("InetSocket::_bind: Socket not opened"));
    return (false);
  }

  ZLOG_DEBUG("InetSocket::_bind: Bind on socket: " + zLog::IntStr(this->_sock));

  struct sockaddr_in addr = this->_inetaddr.GetAddrSockAddr();
  int ret = bind(this->_sock, (struct sockaddr*) &addr, sizeof(addr));
  if (ret < 0)
  {
    ZLOG_CRIT("InetSocket::_bind: Cannot bind socket: " + std::string(strerror(errno)));
    return (false);
  } // end if
  this->_thread.Run(1000);
  return (true);
}

bool
InetSocket::_connect()
{
  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("InetSocket::_connect: Socket not opened"));
    return (false);
  }

  ZLOG_DEBUG("InetSocket::_connect: Connect on socket: " + zLog::IntStr(this->_sock));

  struct sockaddr_in addr = this->_inetaddr.GetAddrSockAddr();
  int ret = connect(this->_sock, (struct sockaddr*) &addr, sizeof(addr));
  if (ret < 0)
  {
    ZLOG_CRIT("InetSocket::_connect: Cannot connect socket: " + std::string(strerror(errno)));
    return (false);
  } // end if
  this->_thread.Run(1000);
  return (true);
}

ssize_t
InetSocket::_send(const Address &addr_, Buffer &sb_)
{
  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("InetSocket::_send: Socket not opened"));
    return (false);
  }

  // Log info message about message being sent
  std::string logstr;
  logstr += "Sending on socket:\t";
  logstr += "To: " + addr_.GetAddress() + ";\t";
  logstr += "From: " + this->_addr.GetAddress() + ";\t";
  logstr += "Size: " + zLog::IntStr(sb_.Size()) + ";";
  ZLOG_INFO(logstr);

  if (addr_ == this->_addr)
  {
    Buffer *sb = new Buffer(sb_);
    this->Push(std::make_pair(addr_, sb));
    return (sb->Size());
  }
  else
  {
    InetAddress ipaddr(addr_);
    struct sockaddr_in src = ipaddr.GetAddrSockAddr();
    socklen_t slen = sizeof(struct sockaddr_in);
    return (sendto(this->_sock, sb_.Head(), sb_.Size(), 0, (struct sockaddr *) &src, slen));
  }

}

ssize_t
InetSocket::_recv(Address &addr_, Buffer &sb_)
{
  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("InetSocket::_recv: Socket not opened"));
    return (false);
  }

  struct sockaddr_in src = { 0 };
  socklen_t len = sizeof(src);

  int n = recvfrom(this->_sock, sb_.Head(), sb_.TotalSize(), 0, (struct sockaddr *) &src, &len);
  if (n > 0)
  {
    InetAddress ipaddr(src);
    addr_.SetType(Address::TYPE_INET);
    addr_.SetAddress(ipaddr.GetAddrString());

    std::string logstr;
    logstr += "Receiving on socket:\t";
    logstr += "To: " + this->_addr.GetAddress() + ";\t";
    logstr += "From: " + ipaddr.GetAddrString() + ";\t";
    logstr += "Size: " + zLog::IntStr(n) + ";";
    ZLOG_INFO(logstr);

  } // end if

  return (n);
}

}
}
