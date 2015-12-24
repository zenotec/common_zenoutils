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
        tx_event(zEvent::Event::TYPE_SOCKET, SocketEvent::EVENTID_PKT_SENT),
        err_event(zEvent::Event::TYPE_SOCKET, SocketEvent::EVENTID_PKT_ERR)
{
  ZLOG_DEBUG("Creating socket: '" + zLog::PointerStr(this) + "'");
  this->RegisterEvent(&this->rx_event);
  this->RegisterEvent(&this->tx_event);
  this->RegisterEvent(&this->err_event);
}

Socket::~Socket()
{
  ZLOG_DEBUG("Destroying socket: '" + zLog::PointerStr(this) + "'");
  this->UnregisterEvent(&this->rx_event);
  this->UnregisterEvent(&this->tx_event);
  this->UnregisterEvent(&this->err_event);
}

const zSocket::SocketAddress *
Socket::GetAddress()
{
  return (NULL);
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
Socket::Receive(SocketAddress *addr_, SocketBuffer *sb_)
{

  ssize_t bytes = -1;

  if (this->rxq.TimedWait(100000) && !this->rxq.Empty())
  {

    // Get first pair off queue and copy to caller's space */
    std::pair<zSocket::SocketAddress, zSocket::SocketBuffer> p(this->rxq.Front());
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
Socket::Receive(SocketAddress *addr_, std::string &str_)
{
  ssize_t bytes = 0;
  zSocket::SocketBuffer *sb = new zSocket::SocketBuffer;
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
  delete (sb);
  return (bytes);
}

ssize_t
Socket::Send(const SocketAddress *addr_, SocketBuffer *sb_)
{
  std::pair<zSocket::SocketAddress, zSocket::SocketBuffer> p(*addr_, *sb_);
  ZLOG_DEBUG("Sending packet: " + p.first.GetAddress() + "(" + zLog::IntStr(p.second.Size()) + ")");
  // Check for address match
  if (addr_->GetAddress() == this->GetAddress()->GetAddress())
  {
    this->rxq.Push(p);
    this->tx_event.Notify(this);
    this->rx_event.Notify(this);
  }
  else
  {
    this->txq.Push(p);
  }
  return (sb_->Size());
}

ssize_t
Socket::Send(const SocketAddress *addr_, const std::string &str_)
{
  ssize_t bytes = 0;
  SocketBuffer *sb = new SocketBuffer;
  memcpy(sb->Head(), str_.c_str(), str_.size());
  sb->Put(str_.size());
  bytes = Send(addr_, sb);
  return (bytes);
}

}
}
