//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zLoopSocket.h
//    Description:
//
//*****************************************************************************

#ifndef __ZLOOPSOCKET_H__
#define __ZLOOPSOCKET_H__

#include <string>

#include <zutils/zThread.h>
#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// LoopAddress Class
//**********************************************************************

class LoopAddress : public SocketAddress
{

public:

  LoopAddress(const std::string &name_ = std::string(""));

  virtual
  ~LoopAddress();

protected:

  virtual bool
  verify(const SocketType type_, const std::string &addr_);

private:

};

//**********************************************************************
// zSocket::LoopSocket Class
//**********************************************************************

class LoopSocket : public Socket, public zThread::ThreadFunction
{

public:

  LoopSocket();

  virtual
  ~LoopSocket();

  virtual bool
  Open();

  virtual void
  Close();

  virtual bool
  Bind();

  virtual bool
  Connect(const SocketAddress* addr_);

protected:

  virtual void
  Run(zThread::ThreadArg *arg_);

private:

  zThread::Thread _thread;
  bool _opened;
  bool _bound;
  bool _connected;

};

}
}

#endif /* __ZLOOPSOCKET_H__ */
