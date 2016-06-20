/*
 * Factory.cpp
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
MessageFactory::Create(const Message::TYPE &type_)
{
  Message *msg = NULL;

  // Yes, this might look questionable, but it serves two purposes. The
  //   first is to validate the message type being passed in. The second
  //   is that it is a placeholder for future enhancements

    switch(type_)
    {
    case Message::TYPE_AUTH:
    case Message::TYPE_HELLO:
    case Message::TYPE_ACK:
    case Message::TYPE_BYE:
    case Message::TYPE_CFG:
    case Message::TYPE_CMD:
    case Message::TYPE_DATA:
    {
      msg = new Message;
      if (msg)
      {
        zUuid::Uuid uuid;
        msg->SetId(uuid());
        msg->SetType(type_);
      }
      break;
    }
    default:
      break;
    }
  return (msg);
}

}
}

