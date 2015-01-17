/*
 * Handler.cpp
 *
 *  Created on: Jan 19, 2014
 *      Author: kmahoney
 */

#include <zutils/zMessage.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// zMessage::Handler Class
//**********************************************************************

Handler::Handler()
{

  // Register self with socket handler
  zSocket::Handler::Register(this);

}

Handler::~Handler()
{
  // Unregister with socket handler
  zSocket::Handler::Unregister(this);
}

bool
Handler::Register(zMessage::Message::TYPE type_, zMessage::Observer* obs_)
{
  std::list<zMessage::Observer *> obsList = this->_obsMap[type_];
  obsList.push_back(obs_);
  return (true);
}

bool
Handler::Unregister(zMessage::Message::TYPE type_, zMessage::Observer* obs_)
{
  std::list<zMessage::Observer *> obsList = this->_obsMap[type_];
  obsList.remove(obs_);
  return (true);
}

bool
Handler::Send(zMessage::Message &msg_)
{
  return (false);
}

bool
Handler::SocketRecv(zSocket::Socket *sock_, const zSocket::Address &addr_, zSocket::Buffer &buf_)
{
  bool status = false;

  // Convert from socket buffer to a message
  zMessage::Message *msg = new zMessage::Message;
  msg->SetJson(buf_.Str());

  // Switch on message type
  switch (msg->GetType())
  {
  case zMessage::Message::TYPE_AUTH:
    break;
  case zMessage::Message::TYPE_HELLO:
    // Update socket and socket address associated with this zNode
    this->_addrMap[msg->GetFrom()].first = sock_;
    this->_addrMap[msg->GetFrom()].second = addr_;
    break;
  case zMessage::Message::TYPE_ACK:
    break;
  case zMessage::Message::TYPE_BYE:
    // Remove socket and socket address associated with this zNode
    this->_addrMap[msg->GetFrom()].first = NULL;
    this->_addrMap[msg->GetFrom()].second = zSocket::Address();
    break;
  case zMessage::Message::TYPE_CFG:
    break;
  case zMessage::Message::TYPE_CMD:
    break;
  case zMessage::Message::TYPE_DATA:
    break;
  default:
    ZLOG_ERR("zMessage type not supported")
    ;
    break;
  }

  // Notify observers
  this->_notify(msg->GetType(), *msg);

  // Return status
  return (status);
}

void
Handler::_notify(zMessage::Message::TYPE type_, zMessage::Message &msg_)
{
  std::list<zMessage::Observer *> obsList = this->_obsMap[type_];
  std::list<zMessage::Observer *>::iterator it = obsList.begin();
  std::list<zMessage::Observer *>::iterator end = obsList.end();
  for (; it != end; ++it)
  {
    (*it)->RecvMsg(*this, msg_);
  }
}

}
}

