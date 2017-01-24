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

#ifndef __ZBYEMESSAGE_H__
#define __ZBYEMESSAGE_H__

#include <zutils/zData.h>
#include <zutils/zMessage.h>
#include <zutils/zMessageSocket.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// Class: ByeMessage
//**********************************************************************

class ByeMessage : public zMessage::Message
{
public:

  ByeMessage();

  ByeMessage(const zData::Data &data_);

  ByeMessage(const zMessage::Message& msg_);

  virtual
  ~ByeMessage();

  const zMessage::Message&
  GetMessage() const;

protected:

private:

};

}
}

#endif /* __ZBYEMESSAGE_H__ */
