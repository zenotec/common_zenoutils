//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Handler.cpp
//    Description:
//
//*****************************************************************************

#include <stdint.h>
#include <string.h>

#include <string>
#include <list>
#include <mutex>
#include <memory>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zQueue.h>
#include <zutils/zThread.h>
#include <zutils/zEvent.h>

#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

//*****************************************************************************
// zSocket::Socket Class
//*****************************************************************************
Socket::Socket(SocketType type_) :
    zEvent::Event(zEvent::Event::TYPE_SOCKET), _type(type_), _addr(NULL)
{
  ZLOG_DEBUG("Creating socket: '" + zLog::PointerStr(this) + "'");
}

Socket::~Socket()
{
  ZLOG_DEBUG("Destroying socket: '" + zLog::PointerStr(this) + "'");
}

const SocketType
Socket::Type() const
{
  return (this->_type);
}

const zSocket::SocketAddress*
Socket::Address() const
{
  return (this->_addr);
}

bool
Socket::Address(const zSocket::SocketAddress* addr_)
{
  this->_addr = addr_;
  return (true);
}

ssize_t
Socket::Send(SocketAddressBufferPair &pair_)
{
  ZLOG_DEBUG(
      "Sending packet: " + pair_.first->Address() + "(" + zLog::IntStr(pair_.second->Size()) + ")");
  this->_txq.Push(pair_);
  return (pair_.second->Size());
}

ssize_t
Socket::Send(const SocketAddress &addr_, SocketBuffer &sb_)
{
  std::shared_ptr<SocketAddress> addr(new SocketAddress(addr_));
  std::shared_ptr<SocketBuffer> sb(new SocketBuffer(sb_));
  SocketAddressBufferPair p(addr, sb);
  return (this->Send(p));
}

ssize_t
Socket::Send(const SocketAddress &addr_, const std::string &str_)
{
  SocketBuffer sb(str_);
  return (this->Send(addr_, sb));
}

bool
Socket::rxbuf(SocketAddressBufferPair &pair_)
{
  SocketNotification notification(this);
  notification.id(SocketNotification::ID_PKT_RCVD);
  notification.pkt(pair_);
  this->Notify(&notification);
  return (true);
}

bool
Socket::txbuf(SocketAddressBufferPair &pair_, size_t timeout_)
{
  bool status = false;
  if (this->_txq.TimedWait(timeout_))
  {
    pair_ = this->_txq.Front();
    this->_txq.Pop();
    SocketNotification notification(this);
    notification.id(SocketNotification::ID_PKT_SENT);
    notification.pkt(pair_);
    this->Notify(&notification);
    status = true;
  }
  return (status);
}

}
}
