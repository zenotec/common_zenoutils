//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zUnixSocket.h
//    Description:
//
//*****************************************************************************

#ifndef __ZUNIXSOCKET_H__
#define __ZUNIXSOCKET_H__

#include <string>

#include <zutils/zThread.h>
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

  UnixAddress(const std::string &addr_ = std::string(""));

  UnixAddress(SocketAddress &addr_);

  UnixAddress(const SocketAddress &addr_);

  virtual
  ~UnixAddress();

protected:

  virtual bool
  verify(const SocketType type_, const std::string &addr_);

private:

};

//**********************************************************************
// zSocket::UnixSocketRecv Class
//**********************************************************************

class UnixSocketRecv : public zThread::ThreadFunction
{
public:

  UnixSocketRecv()
  {
  }

  virtual
  ~UnixSocketRecv()
  {
  }

  virtual void
  Run(zThread::ThreadArg *arg_);

protected:

private:

};

//**********************************************************************
// zSocket::UnixSocketSend Class
//**********************************************************************

class UnixSocketSend : public zThread::ThreadFunction
{
public:

  UnixSocketSend()
  {
  }

  virtual
  ~UnixSocketSend()
  {

  }

  virtual void
  Run(zThread::ThreadArg *arg_);

protected:

private:

};

//**********************************************************************
// zSocket::UnixSocket Class
//**********************************************************************

class UnixSocket : public Socket, public zThread::ThreadArg
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
  Connect(const SocketAddress* addr_);

protected:

  int _sock;

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
