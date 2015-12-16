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

class InetAddress : public Address
{

public:

  InetAddress();

  InetAddress(const std::string &ifname_);

  InetAddress(InetAddress &other_);

  InetAddress(const InetAddress &other_);

  InetAddress &
  operator=(InetAddress &other_);

  InetAddress &
  operator=(const InetAddress &other_);

  virtual
  ~InetAddress();

  bool
  operator ==(const InetAddress &other_) const;

  bool
  operator !=(const InetAddress &other_) const;

  bool
  operator <(const InetAddress &other_) const;

  bool
  operator >(const InetAddress &other_) const;

  virtual std::string
  GetAddress() const;

  virtual bool
  SetAddress(const std::string &addr_);

  std::string
  GetIpAddr() const;

  std::string
  GetPort() const;

  std::string
  GetNetmask() const;

  std::string
  GetBroadcast() const;

  bool
  SetIpAddr(const std::string &ipaddr_);

  bool
  SetPort(const std::string &port_);

  bool
  SetNetmask(const std::string &netmask_);

  bool
  SetBroadcast(const std::string &bcast_);

protected:

private:

  std::string _ipaddr;
  std::string _port;
  std::string _netmask;
  std::string _bcast;

};

//**********************************************************************
// zSocket::InetSocketRecv Class
//**********************************************************************

class InetSocketRecv : public zThread::Function
{
public:
  virtual void *
  ThreadFunction(void *arg_);
};

//**********************************************************************
// zSocket::InetSocketSend Class
//**********************************************************************

class InetSocketSend : public zThread::Function
{
public:
  virtual void *
  ThreadFunction(void *arg_);
};

//**********************************************************************
// zSocket::InetSocket Class
//**********************************************************************

class InetSocket : public Socket
{

  friend InetSocketRecv;
  friend InetSocketSend;

public:

  InetSocket(const InetAddress &addr_);

  virtual
  ~InetSocket();

  virtual const zSocket::Address *
  GetAddress();

  virtual bool
  Open();

  virtual void
  Close();

  virtual bool
  Bind();

  virtual bool
  Connect();

protected:

  int _sock;

  virtual ssize_t
  _recv(zSocket::Address *src_, zSocket::Buffer *sb_);

  virtual ssize_t
  _send(const zSocket::Address *dst_, zSocket::Buffer *sb_);

  virtual ssize_t
  _broadcast(zSocket::Buffer *sb_);

private:

  zThread::Thread _rx_thread;
  InetSocketRecv _rx_func;
  zThread::Thread _tx_thread;
  InetSocketSend _tx_func;

  const InetAddress _inetaddr;

};

}
}

#endif /* __ZINETSOCKET_H__ */
