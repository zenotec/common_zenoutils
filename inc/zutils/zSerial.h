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

#ifndef __ZSERIAL_H__
#define __ZSERIAL_H__

#include <stdio.h>

#include <string>

#include <zutils/zQueue.h>
#include <zutils/zData.h>
#include <zutils/zConfig.h>

namespace zUtils
{
namespace zSerial
{

//**********************************************************************
// Class: SerialConfigPath
//**********************************************************************

class SerialConfigPath : public zConfig::ConfigPath
{

public:

  static const std::string ConfigRoot;
  static const std::string ConfigTypePath;

  SerialConfigPath();

  virtual
  ~SerialConfigPath();

  zConfig::ConfigPath
  Type();

protected:

private:

};

//**********************************************************************
// Class: SerialConfigData
//**********************************************************************

class SerialConfigData : public zConfig::ConfigData
{

public:

  static const std::string ConfigTypeNone;
  static const std::string ConfigTypeEcho;
  static const std::string ConfigTypeTty;

  SerialConfigData();

  SerialConfigData(const zData::Data& data_);

  SerialConfigData(const zConfig::ConfigData& config_);

  SerialConfigData(const SerialConfigData& other_);

  virtual
  ~SerialConfigData();

  std::string
  GetType() const;

  bool
  SetType(const std::string& type_);

protected:

private:

};

//**********************************************************************
// Class: SerialPort
//**********************************************************************

class SerialPort : public zEvent::Event
{

public:

  enum TYPE
  {
    TYPE_ERR = -1,
    TYPE_NONE = 0,
    TYPE_TTY,
    TYPE_LAST
  };

  SerialPort();

  virtual
  ~SerialPort();

  virtual bool
  Open() = 0;

  virtual bool
  Close() = 0;

  ssize_t
  SendBuf(const void *buf_, size_t len_);

  bool
  SendChar(const char c_);

  bool
  SendString(const std::string &str_);

protected:

  // Called to process a received char
  bool
  rxchar(const char c_);

  // Called to get character to send
  bool
  txchar(char *c_, size_t timeout_ = 1000000 /* 1 sec */);

private:

  zQueue<char> _txq;

};

//**********************************************************************
// Class: SerialNotification
//**********************************************************************

class SerialNotification : public zEvent::EventNotification
{

  friend SerialPort;

public:

  enum ID
  {
    ID_ERR = -1,
    ID_NONE = 0,
    ID_CHAR_RCVD,
    ID_CHAR_SENT,
    ID_CHAR_ERR,
    ID_LAST
  };

  SerialNotification(SerialPort* port_);

  virtual
  ~SerialNotification();

  SerialNotification::ID
  Id() const;

  SerialPort*
  Port();

  char
  Data() const;

protected:

  void
  id(SerialNotification::ID id_);

  bool
  data(const char c_);

private:

  SerialNotification::ID _id;
  char _c;

};

//**********************************************************************
// Class: SerialManager
//**********************************************************************

class SerialManager : public zEvent::EventHandler
{
public:

  static SerialManager&
  Instance()
  {
    static SerialManager instance;
    return instance;
  }

protected:

private:

  SerialManager()
  {
  }

  SerialManager(SerialManager const&);

  void
  operator=(SerialManager const&);

};

}
}

#endif /* __ZSERIAL_H__ */
