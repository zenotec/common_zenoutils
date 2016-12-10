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

#ifndef __ZMESSAGE_H__
#define __ZMESSAGE_H__

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// Class: Message
//**********************************************************************

class Message : public zData::Data
{

  static const std::string STR_ROOT;
  static const std::string STR_TYPE;
  static const std::string STR_TYPE_AUTH;
  static const std::string STR_TYPE_HELLO;
  static const std::string STR_TYPE_ACK;
  static const std::string STR_TYPE_BYE;
  static const std::string STR_TYPE_NODE;
  static const std::string STR_TYPE_CFG;
  static const std::string STR_TYPE_CMD;
  static const std::string STR_TYPE_DATA;
  static const std::string STR_ID;
  static const std::string STR_DST;
  static const std::string STR_SRC;
  static const std::string STR_DATA;

public:

  enum TYPE
  {
    TYPE_ERR = -1,
    TYPE_NONE = 0,
    TYPE_AUTH = 1,
    TYPE_HELLO = 2,
    TYPE_ACK = 3,
    TYPE_BYE = 4,
    TYPE_NODE = 5,
    TYPE_CFG = 6,
    TYPE_CMD = 7,
    TYPE_DATA = 8,
    TYPE_LAST
  };

  Message();

  Message(const zData::Data &msg_);

  Message(const std::string &json_);

  Message(Message &other_) :
      zData::Data(other_)
  {
  }

  Message(const Message &other_) :
      zData::Data(other_)
  {
  }

  virtual
  ~Message();

  Message &
  operator=(Message &other_)
  {
    zData::Data::operator =(other_);
    return (*this);
  }

  Message &
  operator=(const Message &other_)
  {
    zData::Data::operator =(other_);
    return (*this);
  }

  Message::TYPE
  GetType() const;

  bool
  SetType(const Message::TYPE &type_);

  std::string
  GetId() const;

  bool
  SetId(const std::string &id_);

  std::string
  GetDst() const;

  bool
  SetDst(const std::string &dst_);

  std::string
  GetSrc() const;

  bool
  SetSrc(const std::string &src_);

  zData::Data
  GetData() const;

  bool
  SetData(const zData::Data &data_);

protected:

private:

};

//**********************************************************************
// Class: MessageFactory
//**********************************************************************

class MessageFactory
{
public:
  static zMessage::Message *
  Create(const zMessage::Message::TYPE &type_);
};

}
}

#endif /* __ZMESSAGE_H__ */

