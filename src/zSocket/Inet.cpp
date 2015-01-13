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
// InetAddress Class
//**********************************************************************

InetAddress::InetAddress(const Address &addr_)
{
  memset(&this->_ipaddr, 0, sizeof(this->_ipaddr));
  if ((addr_.GetType() != Address::TYPE_INET) || !this->SetAddr(addr_.GetAddr()))
  {
    ZLOG_CRIT("zSocket::InetAddress: Cannot set address");
  } // end if
}

InetAddress::InetAddress(const std::string& addr_)
{
  memset(&this->_ipaddr, 0, sizeof(this->_ipaddr));
  if (!this->SetAddr(addr_))
  {
    ZLOG_CRIT("zSocket::InetAddress: Cannot set address");
  } // end if
}

InetAddress::InetAddress(const struct sockaddr_in& addr_)
{
  memset(&this->_ipaddr, 0, sizeof(this->_ipaddr));
  if (!this->SetAddr(addr_))
  {
    ZLOG_CRIT("zSocket::InetAddress: Cannot set address");
  } // end if
}

InetAddress::~InetAddress()
{
}

bool
InetAddress::operator ==(const InetAddress &other_) const
{
  return ((this->GetAddrString() == other_.GetAddrString()));
}

bool
InetAddress::operator !=(const InetAddress &other_) const
{
  return ((this->GetAddrString() != other_.GetAddrString()));
}

std::string
InetAddress::GetAddrString() const
{
  std::string addr = this->_getIpAddrString() + ":" + this->_getPortString();
  return (addr);
}

struct sockaddr_in
InetAddress::GetAddrSockAddr()
{
  return (this->_ipaddr);
}

bool
InetAddress::SetAddr(const std::string& addr_)
{
  sockaddr_in ipaddr = { 0 };
  char ip[INET_ADDRSTRLEN + 1] = { 0 };
  int port = 0;
  int ret = 0;

  // Initialize address
  ipaddr.sin_family = AF_INET;
  ipaddr.sin_addr.s_addr = 0;
  ipaddr.sin_port = 0;

  // Parse address string
  ret = sscanf(addr_.c_str(), "%[^:]:%d", &ip[0], &port);
  if (ret < 1)
  {
    ZLOG_ERR("SocketAddr::SetAddr: Cannot parse address! " + ret);
    return (false);
  } // end if

  // Perform conversion
  if (ret == 2)
  {
    inet_pton(AF_INET, ip, &ipaddr.sin_addr);
    ipaddr.sin_port = htons(port);
    this->_ipaddr = ipaddr;
  } // end if

  // Return success
  return (ret == 2);
}

bool
InetAddress::SetAddr(const struct sockaddr_in &addr_)
{
  bool status = false;
  if (addr_.sin_family == AF_INET)
  {
    this->_ipaddr = addr_;
    status = true;
  }
  return (status);
}

in_addr_t
InetAddress::_getIpAddr() const
{
  return (ntohl(this->_ipaddr.sin_addr.s_addr));
}

std::string
InetAddress::_getIpAddrString() const
{
  char ip[INET_ADDRSTRLEN] = { 0 };
  inet_ntop(AF_INET, &this->_ipaddr.sin_addr, ip, INET_ADDRSTRLEN);
  return (std::string(ip));
}

in_port_t
InetAddress::_getPort() const
{
  return (ntohs(this->_ipaddr.sin_port));
}

std::string
InetAddress::_getPortString() const
{
  char port[10] = { 0 };
  snprintf(port, 10, "%d", this->_getPort());
  return (std::string(port));
}

//**********************************************************************
// InetSocket Class
//**********************************************************************

InetSocket::InetSocket(const Address &addr_) :
    _thread(this, this), _sock(0), _addr(addr_)
{
  ZLOG_DEBUG("InetSocket::InetSocket: New socket: " + zLog::IntStr(this->_sock));
}

InetSocket::~InetSocket()
{
  ZLOG_DEBUG("InetSocket::~InetSocket: Destroying socket: " + zLog::IntStr(this->_sock));
  this->_close();
}

bool
InetSocket::Bind()
{
  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("InetSocket::Bind: Socket not opened"));
    return (false);
  }

  ZLOG_DEBUG("InetSocket::Bind: Bind on socket: " + zLog::IntStr(this->_sock));

  struct sockaddr_in addr = this->_addr.GetAddrSockAddr();
  int ret = bind(this->_sock, (struct sockaddr*) &addr, sizeof(addr));
  if (ret < 0)
  {
    ZLOG_CRIT("InetSocket::Bind: Cannot bind socket: " + std::string(strerror(errno)));
    return (false);
  } // end if
  this->_thread.Run();
  return (true);
}

bool
InetSocket::Connect()
{
  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("InetSocket::Connect: Socket not opened"));
    return (false);
  }

  ZLOG_DEBUG("InetSocket::Connect: Connect on socket: " + zLog::IntStr(this->_sock));

  struct sockaddr_in addr = this->_addr.GetAddrSockAddr();
  int ret = connect(this->_sock, (struct sockaddr*) &addr, sizeof(addr));
  if (ret < 0)
  {
    ZLOG_CRIT("InetSocket::Connect: Cannot connect socket: " + std::string(strerror(errno)));
    return (false);
  } // end if
  this->_thread.Run();
  return (true);
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

  ZLOG_DEBUG("InetSocket::ThreadFunction: Select on socket: " + zLog::IntStr(self->_sock));

  // Select on socket
  int ret = select(self->_sock + 1, &rxFds, NULL, NULL, &to);
  if (ret > 0 && FD_ISSET(this->_sock, &rxFds))
  {
    ZLOG_DEBUG("InetSocket::ThreadFunction: Select successful: " + zLog::IntStr(self->_sock));
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
  logstr += "To: " + addr_.GetAddr() + ";\t";
  logstr += "From: " + this->_addr.GetAddrString() + ";\t";
  logstr += "Size: " + zLog::IntStr(sb_.Size()) + ";";
  ZLOG_INFO(logstr);

  if (InetAddress(addr_) == this->_addr)
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
    addr_.SetAddr(ipaddr.GetAddrString());

    std::string logstr;
    logstr += "Receiving on socket:\t";
    logstr += "To: " + this->_addr.GetAddrString() + ";\t";
    logstr += "From: " + ipaddr.GetAddrString() + ";\t";
    logstr += "Size: " + zLog::IntStr(n) + ";";
    ZLOG_INFO(logstr);

  } // end if

  return (n);
}

}
}
