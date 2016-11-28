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
// zSocket::LoopSocketRecv Class
//**********************************************************************

void *
LoopSocketRecv::ThreadFunction(void *arg_)
{

  LoopSocket *sock = (LoopSocket *) arg_;
  SocketAddressBufferPair p;

  if (sock->txbuf(p, 100000))
  {
    ZLOG_DEBUG("Sending packet: " + p.first->Address() + "(" + zLog::IntStr(p.second->Size()) + ")");
    if (!sock->rxbuf(p))
    {
      ZLOG_ERR("Error sending packet");
    }
  }

  return (0);

}

//**********************************************************************
// zSocket::LoopSocket Class
//**********************************************************************

LoopSocket::LoopSocket() :
    Socket(SocketType::TYPE_LOOP), _rx_thread(&this->_rx_func, this),
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
  this->_opened = false;
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
    if (this->_rx_thread.Run())
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
    if (this->_rx_thread.Run())
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

}
}
