//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Handler.cpp
//    Description:
//
//*****************************************************************************

#include <string.h>

#include <zutils/zLog.h>
#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

//*****************************************************************************
// zSocket::SocketEvent Class
//*****************************************************************************

SocketEvent::SocketEvent(const SocketEvent::EVENTID id_) :
    zEvent::Event(zEvent::Event::TYPE_SOCKET, id_)
{
}

SocketEvent::~SocketEvent()
{
}

//*****************************************************************************
// zSocket::Socket Class
//*****************************************************************************
Socket::Socket() :
    _type(TYPE_NONE),
    rx_event(zEvent::Event::TYPE_SOCKET, SocketEvent::EVENTID_PKT_RCVD),
    tx_event(zEvent::Event::TYPE_SOCKET, SocketEvent::EVENTID_PKT_SENT)
{
  this->RegisterEvent(&this->rx_event);
  this->RegisterEvent(&this->tx_event);
}

Socket::~Socket()
{
  this->UnregisterEvent(&this->rx_event);
  this->UnregisterEvent(&this->tx_event);
}

const zSocket::Address *
Socket::GetAddress()
{
  return(NULL);
}

ssize_t
Socket::Receive(Address *addr_, Buffer *sb_)
{

  ssize_t bytes = -1;

  if (!this->rxq.Empty())
  {

    // Get first pair off queue and copy to caller's space */
    std::pair<zSocket::Address, zSocket::Buffer> p(this->rxq.Front());
    this->rxq.Pop();

    // Copy address */
    addr_->SetAddress(p.first.GetAddress());

    // Copy to caller's socket buffer */
    *sb_ = p.second;

    // Update number of bytes received
    bytes = sb_->Size();

  }

  // Return number of bytes received
  return (bytes);

}

ssize_t
Socket::Receive(Address *addr_, std::string &str_)
{
  ssize_t bytes = 0;
  zSocket::Buffer *sb = new zSocket::Buffer;
  if (!sb)
  {
    std::string errMsg = "Error allocating memory for socket buffer";
    ZLOG_CRIT(errMsg);
    throw errMsg;
  }
  bytes = Receive(addr_, sb);
  if (bytes > 0)
  {
    str_ = sb->Str();
  }
  delete(sb);
  return (bytes);
}

ssize_t
Socket::Send(const Address *addr_, Buffer *sb_)
{
  std::pair<zSocket::Address, zSocket::Buffer> p(*addr_, *sb_);
  ZLOG_DEBUG("Sending packet: " + p.first.GetAddress() + "(" + zLog::IntStr(p.second.Size()) + ")");
  this->txq.Push(p);
  return (sb_->Size());
}

ssize_t
Socket::Send(const Address *addr_, const std::string &str_)
{
  ssize_t bytes = 0;
  Buffer *sb = new Buffer;
  memcpy(sb->Head(), str_.c_str(), str_.size());
  sb->Put(str_.size());
  bytes = Send(addr_, sb);
  return (bytes);
}

ssize_t
Socket::Broadcast(Buffer *sb_)
{
//  return (this->_broadcast(sb_));
  return(0);
}

ssize_t
Socket::Broadcast(const std::string &str_)
{
  ssize_t bytes = 0;
  Buffer *sb = new Buffer;
  memcpy(sb->Head(), str_.c_str(), str_.size());
  sb->Put(str_.size());
  bytes = Broadcast(sb);
  delete(sb);
  return (bytes);
}

}
}
