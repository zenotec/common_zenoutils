/*
 * Handler.cpp
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
// zMessage::Handler Class
//**********************************************************************

MessageHandler::MessageHandler(zMessage::Message::TYPE type_, zMessage::MessageSocket *sock_) :
    _sock(sock_), _type(type_),
        _rx_event(zEvent::Event::TYPE_MSG, zMessage::MessageEvent::EVENTID_MSG_RCVD),
        _tx_event(zEvent::Event::TYPE_MSG, zMessage::MessageEvent::EVENTID_MSG_SENT),
        _err_event(zEvent::Event::TYPE_MSG, zMessage::MessageEvent::EVENTID_MSG_ERR)

{
  ZLOG_DEBUG("Creating message handler: '" + zLog::PointerStr(this) + "'");
  this->RegisterEvent(&this->_rx_event);
  this->RegisterEvent(&this->_tx_event);
  this->RegisterEvent(&this->_err_event);
  this->_sock->RegisterObserver(this);
  this->_lock.Unlock();
}

MessageHandler::~MessageHandler()
{
  ZLOG_DEBUG("Destroying message socket: '" + zLog::PointerStr(this) + "'");
  this->UnregisterEvent(&this->_rx_event);
  this->UnregisterEvent(&this->_tx_event);
  this->UnregisterEvent(&this->_err_event);
  this->_sock->UnregisterObserver(this);
}

bool
MessageHandler::Send(zMessage::Message &msg_)
{
  return (this->_sock->Send(msg_));
}

bool
MessageHandler::EventHandler(zEvent::Event *event_, void *arg_)
{
  bool status = false;

  // Handle only socket events
  if (!event_ || !arg_ || (event_->GetType() != zEvent::Event::TYPE_MSG))
  {
    return (false);
  }

  ZLOG_DEBUG(
      "Handling message socket event: '" + zLog::PointerStr(this) + ", " + zLog::PointerStr(event_)
          + ", " + zLog::PointerStr(arg_) + "'");

  switch (event_->GetId())
  {
  case zMessage::MessageEvent::EVENTID_MSG_RCVD:
    {
    zMessage::Message *msg = static_cast<zMessage::Message *>(arg_);
    if (msg->GetType() == this->_type)
    {
      this->_rx_event.Notify(arg_);
      status = true;
    }
    break;
  }
  case zMessage::MessageEvent::EVENTID_MSG_SENT:
    {
//    if (msg->GetType() == this->_type)
//    {
      this->_tx_event.Notify(arg_);
      status = true;
//    }
    break;
  }
  case zMessage::MessageEvent::EVENTID_MSG_ERR:
    {
//    if (msg->GetType() == this->_type)
//    {
      this->_err_event.Notify(arg_);
      status = true;
//    }
    break;
  }
  default:
    break;
  }

  return (status);
}

}
}

