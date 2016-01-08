/*
 * Socket.cpp
 *
 *  Created on: Jan 19, 2014
 *      Author: kmahoney
 */

#include <stddef.h>

#include <zutils/zMessage.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// zMessage::Socket Class
//**********************************************************************

MessageSocket::MessageSocket(zSocket::Socket *sock_) :
    _sock(sock_),
        _rx_event(zEvent::Event::TYPE_MSG, zMessage::MessageEvent::EVENTID_MSG_RCVD),
        _tx_event(zEvent::Event::TYPE_MSG, zMessage::MessageEvent::EVENTID_MSG_SENT),
        _err_event(zEvent::Event::TYPE_MSG, zMessage::MessageEvent::EVENTID_MSG_ERR)
{
  ZLOG_DEBUG("Creating message socket: '" + zLog::PointerStr(this) + "'");
  this->RegisterEvent(&this->_rx_event);
  this->RegisterEvent(&this->_tx_event);
  this->RegisterEvent(&this->_err_event);
  this->_sock->RegisterObserver(this);
}

MessageSocket::~MessageSocket()
{
  ZLOG_DEBUG("Destroying message socket: '" + zLog::PointerStr(this) + "'");
  this->UnregisterEvent(&this->_rx_event);
  this->UnregisterEvent(&this->_tx_event);
  this->UnregisterEvent(&this->_err_event);
  this->_sock->UnregisterObserver(this);
}

bool
MessageSocket::Send(zMessage::Message &msg_)
{
  bool status = true;

  msg_.SetSrc(this->_sock->GetAddress().GetAddress());
  zSocket::SocketAddress dstAddr;
  dstAddr.SetAddress(msg_.GetDst());

  ZLOG_INFO("Sending message: " + msg_.GetId());
  ZLOG_DEBUG(msg_.GetJson());

  ssize_t size = this->_sock->Send(dstAddr, msg_.GetJson());
  if (size != msg_.GetJson().size())
  {
    ZLOG_ERR("Failed to send message: bytes sent: " + zLog::IntStr(size));
    status = false;
  }

  // Return status
  return (status);

}

bool
MessageSocket::EventHandler(zEvent::Event *event_, void *arg_)
{
  bool status = false;

  // Handle only socket events
  if (!event_ || !arg_ || (event_->GetType() != zEvent::Event::TYPE_SOCKET))
  {
    return (false);
  }

  ZLOG_DEBUG("Handling socket event: '" + zLog::PointerStr(this) + ", " + zLog::PointerStr(event_) + ", " + zLog::PointerStr(arg_) + "'");

  if (event_->GetId() == zSocket::SocketEvent::EVENTID_PKT_RCVD)
  {
    zSocket::Socket *sock = static_cast<zSocket::Socket *>(arg_);
    zSocket::SocketAddress addr;
    std::string str;

    // Receive message string
    if (sock->Receive(addr, str) <= 0)
    {
      ZLOG_ERR("Error receiving message string");
      this->_err_event.Notify(this);
      return (false);
    }

    // Convert from socket buffer to a message
    zMessage::Message *message = new zMessage::Message;
    if (!message || !message->SetJson(str))
    {
      ZLOG_ERR("Error converting string to zMessage");
      this->_err_event.Notify(this);
      return (false);
    }

    ZLOG_INFO("Received message: (" + zLog::PointerStr(message) + ")->Type(" + zLog::IntStr(message->GetType()) + ")");
    ZLOG_DEBUG(message->GetJson());

    // Notify observers
    this->_rx_event.Notify(message);
    delete(message);

  }
  if (event_->GetId() == zSocket::SocketEvent::EVENTID_PKT_SENT)
  {
    this->_tx_event.Notify(this);
  }

  return (status);
}

}
}

