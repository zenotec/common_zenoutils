//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zInetSocket.h
//    Description:
//
//*****************************************************************************

#ifndef __ZINETSOCKET_H__
#define __ZINETSOCKET_H__

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// InetAddress Class
//**********************************************************************

class InetAddress : public SocketAddress
{

public:

  InetAddress();

  InetAddress(const std::string &ifname_);

  InetAddress(SocketAddress &other_);

  InetAddress(const SocketAddress &other_);

  InetAddress &
  operator=(SocketAddress &other_);

  InetAddress &
  operator=(const SocketAddress &other_);

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
  GetIp() const;

  std::string
  GetPort() const;

  std::string
  GetNetmask() const;

  std::string
  GetBroadcast() const;

  bool
  SetIp(const std::string &ip_);

  bool
  SetPort(const std::string &port_);

  bool
  SetNetmask(const std::string &netmask_);

  bool
  SetBroadcast(const std::string &bcast_);

protected:

private:

  std::string _ip;
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
  InetSocketRecv()
  {

  }
  virtual
  ~InetSocketRecv()
  {

  }
  virtual void *
  ThreadFunction(void *arg_);
};

//**********************************************************************
// zSocket::InetSocketSend Class
//**********************************************************************

class InetSocketSend : public zThread::Function
{
public:
  InetSocketSend()
  {

  }

  virtual
  ~InetSocketSend()
  {

  }

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

  InetSocket();

  virtual
  ~InetSocket();

  const zSocket::InetAddress &
  GetAddress();

  bool
  SetAddress(const zSocket::InetAddress &addr_);

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
  _recv(zSocket::InetAddress &src_, zSocket::SocketBuffer &sb_);

  virtual ssize_t
  _send(const zSocket::InetAddress &dst_, zSocket::SocketBuffer &sb_);

  virtual ssize_t
  _broadcast(zSocket::SocketBuffer &sb_);

private:

  zThread::Thread _rx_thread;
  InetSocketRecv _rx_func;
  zThread::Thread _tx_thread;
  InetSocketSend _tx_func;

  InetAddress _inetaddr;

};

}
}

#endif /* __ZINETSOCKET_H__ */
