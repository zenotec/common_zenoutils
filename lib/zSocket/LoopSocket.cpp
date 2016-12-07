//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Loop.cpp
//    Description:
//
//*****************************************************************************

#include <string>
#include <list>
#include <mutex>
#include <memory>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>
#include <zutils/zSocket.h>

#include <zutils/zLoopSocket.h>

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// LoopAddress Class
//**********************************************************************

LoopAddress::LoopAddress(const std::string &name_) :
    SocketAddress(SocketType::TYPE_LOOP, name_)
{
}

LoopAddress::~LoopAddress()
{
}

bool
LoopAddress::verify(const SocketType type_, const std::string &addr_)
{
  return (type_ == SocketType::TYPE_LOOP);
}

//**********************************************************************
// zSocket::LoopSocket Class
//**********************************************************************

LoopSocket::LoopSocket() :
    Socket(SocketType::TYPE_LOOP), _thread(this, NULL),
        _opened(false), _bound(false), _connected(false)
{
  return;
}

LoopSocket::~LoopSocket()
{
  this->Close();
  return;
}

bool
LoopSocket::Open()
{
  bool status = false;

  if (!this->Address() || this->Address()->Type() != SocketType::TYPE_LOOP)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  if (!this->_opened)
  {
    this->_opened = true;
    status = true;
  }
  return (status);
}

void
LoopSocket::Close()
{
  // Close socket
  if (this->_thread.Stop())
  {
    this->_opened = false;
    ZLOG_DEBUG("Socket Closed");
  } // end if

  return;
}

bool
LoopSocket::Bind()
{
  bool status = false;

  if (!this->Address() || this->Address()->Type() != SocketType::TYPE_LOOP)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  if (this->_opened && !this->_bound && !this->_connected)
  {
    // Start listener threads
    if (this->_thread.Start())
    {
      this->_bound = true;
      status = true;
    }
    else
    {
      ZLOG_ERR("Error starting listening threads");
    }
  }

  return (status);
}

bool
LoopSocket::Connect(const SocketAddress* addr_)
{
  bool status = false;

  if (!this->Address() || this->Address()->Type() != SocketType::TYPE_LOOP)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  if (addr_->Type() != SocketType::TYPE_LOOP)
  {
    ZLOG_CRIT(std::string("Invalid socket address type"));
    return (false);
  }

  if (this->_opened && !this->_bound && !this->_connected)
  {
    // Start listener threads
    if (this->_thread.Start())
    {
      this->_connected = true;
      status = true;
    }
    else
    {
      ZLOG_ERR("Error starting listening threads");
    }
  }

  return (status);
}

void
LoopSocket::Run(zThread::ThreadArg *arg_)
{

  SocketAddressBufferPair p;

  while (!this->Exit())
  {

    if (this->txbuf(p, 100))
    {
      ZLOG_DEBUG("Sending packet: " + p.first->Address() +
          "(" + zLog::IntStr(p.second->Size()) + ")");
      if (!this->rxbuf(p))
      {
        ZLOG_ERR("Error sending packet");
      }
    }

  }

  return;

}

}
}
