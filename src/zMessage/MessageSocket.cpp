/*
 * Socket.cpp
 *
 *  Created on: Jan 19, 2014
 *      Author: kmahoney
 */

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
#include <zutils/zData.h>
#include <zutils/zSocket.h>

#include <zutils/zMessage.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// zMessage::Socket Class
//**********************************************************************

MessageSocket::MessageSocket(zSocket::Socket *sock_) :
    _sock(sock_)
{
  ZLOG_DEBUG("Creating message socket: '" + zLog::PointerStr(this) + "'");
  this->_handler.RegisterObserver(this);
  this->_handler.Add(sock_);
}

MessageSocket::~MessageSocket()
{
  ZLOG_DEBUG("Destroying message socket: '" + zLog::PointerStr(this) + "'");
  this->_handler.UnregisterObserver(this);
  this->_handler.Remove(this->_sock);
}

const zSocket::SocketAddress &
MessageSocket::GetAddress()
{
  return(this->_sock->GetAddress());
}

bool
MessageSocket::SetAddress(const zSocket::SocketAddress &addr_)
{
  return(this->_sock->SetAddress(addr_));
}

bool
MessageSocket::Open()
{
  return(this->_sock->Open());
}

void
MessageSocket::Close()
{
  this->_sock->Close();
  return;
}

bool
MessageSocket::Bind()
{
  return(this->_sock->Bind());
}

bool
MessageSocket::Connect(const zSocket::SocketAddress &addr_)
{
  return(this->_sock->Connect(addr_));
}

bool
MessageSocket::Receive(zMessage::Message &msg_)
{
  bool status = false;
  zSocket::SocketAddressBufferPair p;
  if (zSocket::Socket::Receive(p) > 0)
  {
    msg_.SetJson(p.second->Str());
    status = true;
  }
  return(status);
}

bool
MessageSocket::Send(zMessage::Message &msg_)
{
  bool status = false;

  // Get destination address from message
  zSocket::SocketAddress addr;
  addr.SetAddress(msg_.GetDst());

  // Copy message contents to socket buffer
  std::shared_ptr<zSocket::SocketBuffer> sb(new zSocket::SocketBuffer);
  sb->Str(msg_.GetJson());

  // Create address / buffer pair
  zSocket::SocketAddressBufferPair p(addr, sb);

  // Send message
  if (this->_sock->Send(p) == sb->Size())
  {
    status = true;
  }

  // Return status
  return(status);
}

bool
MessageSocket::EventHandler(const zEvent::EventNotification* notification_)
{
  ZLOG_DEBUG("Handling socket event");

  bool status = false;
  if (notification_ && (notification_->Type() == zEvent::Event::TYPE_SOCKET))
  {
    zSocket::SocketNotification *n = (zSocket::SocketNotification *) notification_;
    switch (n->Id())
    {
    case zSocket::SocketNotification::ID_PKT_RCVD:
    {
      zSocket::SocketAddressBufferPair p;
      this->_sock->Receive(p);
      this->rxbuf(p);
      status = true;
      break;
    }
    case zSocket::SocketNotification::ID_PKT_SENT:
    {
      zSocket::SocketNotification notification(zSocket::SocketNotification::ID_PKT_SENT, this);
      this->Notify(&notification);
      status = true;
      break;
    }
    default:
      status = false;
      break;
    }
  }
  return (status);
}

}
}

