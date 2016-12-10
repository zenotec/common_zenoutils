/*
 * Handler.cpp
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
#include <zutils/zMessageSocket.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// Class: MessageNotification
//**********************************************************************

MessageNotification::MessageNotification(zMessage::MessageSocket *sock_) :
    _id(MessageNotification::ID_NONE), zEvent::EventNotification(sock_)
{
}

MessageNotification::~MessageNotification()
{
}

MessageNotification::ID
MessageNotification::Id() const
{
  return (this->_id);
}

bool
MessageNotification::id(MessageNotification::ID id_)
{
  this->_id = id_;
  return (true);
}

const zSocket::SocketAddress&
MessageNotification::SrcAddr() const
{
  return(this->_addr);
}

bool
MessageNotification::srcaddr(zSocket::SocketAddress& addr_)
{
  this->_addr = addr_;
  return(true);
}


zMessage::MessageSocket*
MessageNotification::Sock() const
{
  return (static_cast<zMessage::MessageSocket*>(this->GetEvent()));
}

zMessage::Message*
MessageNotification::Message() const
{
  return (this->_msg);
}

bool
MessageNotification::message(zMessage::Message* msg_)
{
  this->_msg = msg_;
  return (true);
}

}
}

