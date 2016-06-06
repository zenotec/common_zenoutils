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

class SerialPort : public SerialConfiguration, public zEvent::Event
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

  zQueue<char> rxq;
  zQueue<char> txq;

};

//**********************************************************************
// Class: SerialNotification
//**********************************************************************

class SerialNotification : public zEvent::EventNotification
{
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

  SerialNotification(const SerialNotification::ID id_, SerialPort* port_);

  virtual
  ~SerialNotification();

  SerialNotification::ID
  Id() const;

protected:

private:

  SerialNotification::ID _id;
  SerialPort* _port;

};

//**********************************************************************
// Class: SerialHandler
//**********************************************************************

class SerialHandler : public zEvent::EventHandler
{
public:

  SerialHandler();

  virtual
  ~SerialHandler();

  bool
  Add(SerialPort *port_);

  bool
  Remove(SerialPort *port_);

protected:

private:

  std::list<SerialPort *> _port_list;

};

//**********************************************************************
// Class: SerialManager
//**********************************************************************

class SerialManager : public SerialHandler
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
