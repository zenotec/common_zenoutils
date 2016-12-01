//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zInetSocket.h
//    Description:
//
//*****************************************************************************

#ifndef __ZINETSOCKET_H__
#define __ZINETSOCKET_H__

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

class InetAddress : public SocketAddress
{

public:

  InetAddress(const std::string &addr_ = std::string("0.0.0.0:0"));

  InetAddress(SocketAddress &addr_);

  InetAddress(const SocketAddress &addr_);

  virtual
  ~InetAddress();

protected:

private:

  virtual bool
  verify(const SocketType type_, const std::string &addr_);

};

//**********************************************************************
// zSocket::InetSocketRecv Class
//**********************************************************************

class InetSocketRecv : public zThread::ThreadFunction
{

public:

  InetSocketRecv()
  {
  }

  virtual
  ~InetSocketRecv()
  {
  }

  virtual void
  Run(zThread::ThreadArg *arg_);

protected:

private:

};

//**********************************************************************
// zSocket::InetSocketSend Class
//**********************************************************************

class InetSocketSend : public zThread::ThreadFunction
{

public:

  InetSocketSend()
  {
  }

  virtual
  ~InetSocketSend()
  {
  }

  virtual void
  Run(zThread::ThreadArg *arg_);

protected:

private:

};

//**********************************************************************
// zSocket::InetSocket Class
//**********************************************************************

class InetSocket : public Socket, public zThread::ThreadArg
{

  friend InetSocketRecv;
  friend InetSocketSend;

public:

  InetSocket();

  virtual
  ~InetSocket();

  virtual bool
  Open();

  virtual void
  Close();

  virtual bool
  Bind();

  virtual bool
  Connect(const SocketAddress* addr_);

protected:

  int _sock;

  virtual ssize_t
  _recv(zSocket::InetAddress &src_, zSocket::SocketBuffer &sb_);

  virtual ssize_t
  _send(const zSocket::InetAddress &dst_, zSocket::SocketBuffer &sb_);

private:

  zThread::Thread _rx_thread;
  InetSocketRecv _rx_func;
  zThread::Thread _tx_thread;
  InetSocketSend _tx_func;

};

}
}

#endif /* __ZINETSOCKET_H__ */
