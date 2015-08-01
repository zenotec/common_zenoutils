/*
 * Factory.cpp
 *
 *  Created on: Jan 19, 2014
 *      Author: kmahoney
 */

#include <zutils/zUuid.h>
#include <zutils/zMessage.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// MessageFactory
//**********************************************************************

Message *
Factory::Create(const Message::TYPE &type_)
{
  Message *msg = new Message;

  // Yes, this might look questionable, but it serves two purposes. The
  //   first is to validate the message type being passed in. The second
  //   is that it is a placeholder for future enhancements
  if (msg)
  {
    switch(type_)
    {
    case Message::TYPE_AUTH:
    case Message::TYPE_HELLO:
    case Message::TYPE_ACK:
    case Message::TYPE_BYE:
    case Message::TYPE_CFG:
    case Message::TYPE_CMD:
    case Message::TYPE_DATA:
      zUuid::Uuid uuid;
      msg->SetId(uuid());
      msg->SetType(type_);
      break;
    default:
      delete (msg);
      msg = NULL;
      break;
    }
  }
  return (msg);
}

}
}

