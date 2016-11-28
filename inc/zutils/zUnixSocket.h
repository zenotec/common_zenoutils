//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zUnixSocket.h
//    Description:
//
//*****************************************************************************

#ifndef __ZUNIXSOCKET_H__
#define __ZUNIXSOCKET_H__

#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// UnixAddress Class
//**********************************************************************

class UnixAddress : public SocketAddress
{

public:

  UnixAddress();

  UnixAddress(SocketAddress &addr_);

  UnixAddress(const SocketAddress &addr_);

  virtual
  ~UnixAddress();

protected:

  virtual bool
  verify(SocketType type_, const std::string &addr_);

private:

};

//**********************************************************************
// zSocket::UnixSocketRecv Class
//**********************************************************************

class UnixSocketRecv : public zThread::Function
{
public:

  UnixSocketRecv()
  {
  }

  virtual
  ~UnixSocketRecv()
  {
  }

  virtual void *
  ThreadFunction(void *arg_);

protected:

private:

};

//**********************************************************************
// zSocket::UnixSocketSend Class
//**********************************************************************

class UnixSocketSend : public zThread::Function
{
public:

  UnixSocketSend()
  {
  }

  virtual
  ~UnixSocketSend()
  {

  }

  virtual void *
  ThreadFunction(void *arg_);

protected:

private:

};

//**********************************************************************
// zSocket::UnixSocket Class
//**********************************************************************

class UnixSocket : public Socket
{

  friend UnixSocketRecv;
  friend UnixSocketSend;

public:

  UnixSocket();

  virtual
  ~UnixSocket();

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

  virtual bool
  verify(SocketType type_, const std::string &addr_);

  virtual ssize_t
  _recv(zSocket::UnixAddress &src_, zSocket::SocketBuffer &sb_);

  virtual ssize_t
  _send(const zSocket::UnixAddress &dst_, zSocket::SocketBuffer &sb_);

private:

  zThread::Thread _rx_thread;
  UnixSocketRecv _rx_func;
  zThread::Thread _tx_thread;
  UnixSocketSend _tx_func;

};

}
}

#endif /* __ZUNIXSOCKET_H__ */
