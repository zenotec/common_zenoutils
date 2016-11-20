//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zSerial.h
//    Description:
//
//*****************************************************************************

#ifndef __ZSERIAL_H__
#define __ZSERIAL_H__

namespace zUtils
{
namespace zSerial
{

//**********************************************************************
// Class: SerialConfiguration
//**********************************************************************

class SerialConfiguration : public zConfig::Configuration
{

public:

  static const std::string ConfigRoot;
  static const std::string ConfigTypePath;
  static const std::string ConfigTypeNone;
  static const std::string ConfigTypeTty;

  SerialConfiguration();

  SerialConfiguration(zData::Data &data_);

  SerialConfiguration(zConfig::Configuration &config_);

  virtual
  ~SerialConfiguration();

  zConfig::Configuration&
  GetConfig();

  std::string
  Type() const;

  bool
  Type(const std::string& type_);

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

  SerialPort(zConfig::Configuration& config_);

  virtual
  ~SerialPort();

  virtual bool
  Open();

  virtual bool
  Close();

  ssize_t
  RecvBuf(void *buf_, size_t len_, size_t timeout_ = 1000000 /* 1 sec */);

  bool
  RecvChar(char *c_, size_t timeout_ = 1000000 /* 1 sec */);

  bool
  RecvString(std::string &str_, size_t timeout_ = 1000000 /* 1 sec */);

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

  SerialConfiguration _config;
  zQueue<char> _rxq;
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
