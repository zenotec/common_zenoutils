/*
 * Factory.cpp
 *
 *  Created on: Jan 19, 2014
 *      Author: kmahoney
 */

#include "zutils/zMessage.h"

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

  // Yes, this might look ridiculous, but it serves two purposes. The
  //   first is to validate the message type being passed in. The second
  //   is that it is a placeholder for future enhancements
  if (msg)
  {
    if (type_ == Message::TYPE_AUTH)
    {
      msg->SetType(type_);
    } // end if
    else if (type_ == Message::TYPE_HELLO)
    {
      msg->SetType(type_);
    } // end if
    else if (type_ == Message::TYPE_ACK)
    {
      msg->SetType(type_);
    } // end else if
    else if (type_ == Message::TYPE_BYE)
    {
      msg->SetType(type_);
    } // end else if
    else if (type_ == Message::TYPE_CFG)
    {
      msg->SetType(type_);
    } // end else if
    else if (type_ == Message::TYPE_CMD)
    {
      msg->SetType(type_);
    } // end else if
    else if (type_ == Message::TYPE_DATA)
    {
      msg->SetType(type_);
    } // end else if
    else
    {
      delete (msg);
      msg = NULL;
    } // end else
  }
  return (msg);
}

}
}

