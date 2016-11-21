//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Handler.cpp
//    Description:
//
//*****************************************************************************

#include <stdint.h>
#include <netinet/in.h>
#include <string.h>

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

namespace zUtils
{
namespace zSocket
{

//*****************************************************************************
// zSocket::Socket Class
//*****************************************************************************
Socket::Socket() :
    _type(TYPE_NONE), zEvent::Event(zEvent::Event::TYPE_SOCKET)
{
  ZLOG_DEBUG("Creating socket: '" + zLog::PointerStr(this) + "'");
}

Socket::~Socket()
{
  ZLOG_DEBUG("Destroying socket: '" + zLog::PointerStr(this) + "'");
}

zSocket::Socket::TYPE
Socket::GetType()
{
  return(this->_type);
}

bool
Socket::SetType(zSocket::Socket::TYPE type_)
{
  this->_type = type_;
  return(true);
}

const zSocket::SocketAddress &
Socket::GetAddress()
{
  return (this->_addr);
}

bool
Socket::SetAddress(const zSocket::SocketAddress &addr_)
{
  return(this->_addr.SetAddress(addr_.GetAddress()));
}

bool
Socket::Open()
{
  return (false);
}

void
Socket::Close()
{
}

bool
Socket::Bind()
{
  return (false);
}

bool
Socket::Connect(const zSocket::SocketAddress &addr_)
{
  return (false);
}

ssize_t
Socket::Send(SocketAddressBufferPair &pair_)
{
  ZLOG_DEBUG("Sending packet: " + pair_.first.GetAddress() + "(" + zLog::IntStr(pair_.second->Size()) + ")");
  this->_txq.Push(pair_);
  return (pair_.second->Size());
}

ssize_t
Socket::Send(const SocketAddress &addr_, SocketBuffer &sb_)
{
  std::shared_ptr<SocketBuffer> sb(new SocketBuffer(sb_));
  SocketAddressBufferPair p(addr_, sb);
  return (this->Send(p));
}

ssize_t
Socket::Send(const SocketAddress &addr_, const std::string &str_)
{
  std::shared_ptr<SocketBuffer> sb(new SocketBuffer);
  sb->Str(str_);
  SocketAddressBufferPair p(addr_, sb);
  return (this->Send(p));
}

bool
Socket::rxbuf(SocketAddressBufferPair &pair_)
{
  SocketNotification notification(this);
  notification.id(SocketNotification::ID_PKT_RCVD);
  notification.pkt(pair_);
  this->Notify(&notification);
  return(true);
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
  return(status);
}

}
}
