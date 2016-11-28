//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zLoopSocket.h
//    Description:
//
//*****************************************************************************

#ifndef __ZLOOPSOCKET_H__
#define __ZLOOPSOCKET_H__

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
  verify(SocketType type_, const std::string &addr_);

private:

};

//**********************************************************************
// zSocket::LoopSocketRecv Class
//**********************************************************************

class LoopSocketRecv : public zThread::Function
{
public:
  LoopSocketRecv()
  {

  }
  virtual
  ~LoopSocketRecv()
  {

  }
  virtual void *
  ThreadFunction(void *arg_);
};


//**********************************************************************
// zSocket::LoopSocket Class
//**********************************************************************

class LoopSocket : public Socket
{

  friend LoopSocketRecv;

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
  Connect();

protected:

private:

  zThread::Thread _rx_thread;
  LoopSocketRecv _rx_func;

};

}
}

#endif /* __ZLOOPSOCKET_H__ */
