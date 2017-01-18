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

#include <zutils/zData.h>
#include <zutils/zMessage.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// Class: AckMessage
//**********************************************************************

class AckMessage : public zMessage::Message
{

public:

  static const std::string RespDataPath;

  AckMessage();

  AckMessage(const zData::Data &data_);

  AckMessage(const zMessage::Message& msg_);

  virtual
  ~AckMessage();

  const zMessage::Message&
  GetMessage() const;

  std::string
  GetResponse() const;

  bool
  SetResponse(const std::string& resp_);

protected:

private:

};

}
}

#endif /* __ZACKMESSAGE_H__ */
