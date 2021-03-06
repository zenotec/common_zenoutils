/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
// Class: MessageObserver
//**********************************************************************

MessageObserver::MessageObserver(zMessage::Message::TYPE type_) :
    _type(type_)
{
}

MessageObserver::~MessageObserver()
{
}

bool
MessageObserver::EventHandler(const zEvent::EventNotification* notification_)
{
  ZLOG_DEBUG("Handling socket event");

  bool status = false;
  if (notification_ && (notification_->Type() == zEvent::Event::TYPE_SOCKET))
  {
    zSocket::SocketNotification *n = (zSocket::SocketNotification *) notification_;
    switch (n->Id())
    {
    case zSocket::SocketNotification::ID_PKT_RCVD:
      zMessage::MessageNotification notification(zMessage::MessageNotification::ID_MSG_RCVD);
      this->MessageHandler(notification);
      status = true;
      break;
    case zSocket::SocketNotification::ID_PKT_SENT:
      this->TxSem.Post();
      status = true;
      break;
    default:
      this->ErrSem.Post();
      status = false;
      break;
    }
  }
  return (status);
}

}
}

