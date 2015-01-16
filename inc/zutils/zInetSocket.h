//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zInetSocket.h
//    Description:
//
//*****************************************************************************

#ifndef __ZINETSOCKET_H__
#define __ZINETSOCKET_H__

#include <stdint.h>
#include <netinet/in.h>

#include <string>

#include <zutils/zThread.h>
#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// InetAddress Class
//**********************************************************************

class InetAddress
{

public:

  InetAddress(const Address &addr_);
  InetAddress(const std::string &addr_ = "0.0.0.0:0");
  InetAddress(const struct sockaddr_in &addr_);

  virtual
  ~InetAddress();

  bool
  operator ==(const InetAddress &other_) const;
  bool
  operator !=(const InetAddress &other_) const;

  Address
  GetAddr() const;
  std::string
  GetAddrString() const;
  struct sockaddr_in
  GetAddrSockAddr() const;

  bool
  GetAddr(Address &addr_) const;
  bool
  GetAddr(std::string &addr_) const;
  bool
  GetAddr(struct sockaddr_in &addr_) const;

  bool
  SetAddr(const Address &addr_);
  bool
  SetAddr(const std::string &addr_);
  bool
  SetAddr(const struct sockaddr_in &addr_);

protected:

private:

  in_addr_t
  _getIpAddr() const;
  std::string
  _getIpAddrString() const;

  in_port_t
  _getPort() const;
  std::string
  _getPortString() const;

  struct sockaddr_in _ipaddr;

};

//**********************************************************************
// InetSocket Class
//**********************************************************************

class InetSocket : public Socket, private zThread::Function
{

public:

  InetSocket();
  InetSocket(const Address &addr_);

  virtual
  ~InetSocket();

protected:

  virtual void *
  ThreadFunction(void *arg_);

  virtual bool
  _open();

  virtual void
  _close();

  virtual bool
  _bind();

  virtual bool
  _connect();

  virtual ssize_t
  _send(const Address &addr_, Buffer &sb_);

private:

  virtual ssize_t
  _recv(Address &addr_, Buffer &sb_);

  zThread::Thread _thread;

  int _sock;
  InetAddress _addr;

};

}
}

#endif /* __ZINETSOCKET_H__ */
