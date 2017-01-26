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

#ifndef __ZACKMESSAGE_H__
#define __ZACKMESSAGE_H__

#include <stddef.h>

#include <string>
#include <map>

#include <zutils/zQueue.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zMessage.h>

namespace zUtils
{
namespace zMessage
{

class MessageNotification;

//**********************************************************************
// Class: AckMessage
//**********************************************************************

class AckMessage : public zMessage::Message
{

public:

  static const std::string StatusDataPath;
  static const std::string InfoDataPath;

  enum STATUS
  {
    STATUS_ERR = -1,
    STATUS_NONE = 0,
    STATUS_PASS = 1,
    STATUS_FAIL = 2,
    STATUS_LAST
  };

  AckMessage();

  AckMessage(const zData::Data &data_);

  AckMessage(const zMessage::Message& msg_);

  virtual
  ~AckMessage();

  const zMessage::Message&
  GetMessage() const;

  AckMessage::STATUS
  GetStatus() const;

  bool
  SetStatus(const AckMessage::STATUS status_);

  std::string
  GetInfo() const;

  bool
  SetInfo(const std::string& resp_);

protected:

private:

};

//**********************************************************************
// Typedef: AckMessageTable
//**********************************************************************

typedef std::map<std::string, zQueue<AckMessage> > AckMessageTable;

//**********************************************************************
// Class: AckObserver
//**********************************************************************

class AckObserver : public zEvent::EventObserver
{

public:

  AckObserver();

  virtual
  ~AckObserver();

  bool
  RegisterForAck(const std::string& msg_id_);

  bool
  UnregisterForAck(const std::string& msg_id_);

  bool
  WaitForAck(const std::string& msg_id_, AckMessage& ack_, uint32_t ms_);

protected:

  bool
  EventHandler(zEvent::EventNotification* notification_);

private:

  AckMessageTable _ack_table;

  bool
  EventHandler(zMessage::MessageNotification* notification_);

};

}
}

#endif /* __ZACKMESSAGE_H__ */
