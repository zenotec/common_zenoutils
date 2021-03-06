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
// Class: MessagePath
//**********************************************************************

class MessagePath : public zData::DataPath
{

public:

  static const std::string DataRoot;
  static const std::string IdDataPath;
  static const std::string TypeDataPath;
  static const std::string DstDataPath;
  static const std::string SrcDataPath;
  static const std::string DataDataPath;

  MessagePath(const std::string& root_ = std::string(""));

  MessagePath(const MessagePath& other_);

  MessagePath(const zData::DataPath& path_);

  virtual
  ~MessagePath();

private:

};

//**********************************************************************
// Class: Message
//**********************************************************************

class Message : public zData::Data
{

  static const std::string DataTypeNone;
  static const std::string DataTypeAuth;
  static const std::string DataTypeHello;
  static const std::string DataTypeAck;
  static const std::string DataTypeBye;
  static const std::string DataTypeNode;
  static const std::string DataTypeConfig;
  static const std::string DataTypeCommand;
  static const std::string DataTypeData;
  static const std::string DataTypeDefault;

public:

  enum TYPE
  {
    TYPE_ERR = -1,
    TYPE_NONE = 0,
    TYPE_DEFAULT = 0,
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

  Message(const zData::Data &data_);

  Message(const Message &other_);

  virtual
  ~Message();

  Message &
  operator=(const Message &other_);

  bool
  operator ==(const Message& other_) const;

  bool
  operator !=(const Message& other_) const;

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
  Create(const zMessage::Message::TYPE& type_);

  static zMessage::Message *
  Create(const std::string& json_);

  static zMessage::Message *
  Create(const zData::Data& data_);

  static zMessage::Message *
  Create(const zMessage::Message& msg_);

};

}
}

#endif /* __ZMESSAGE_H__ */

