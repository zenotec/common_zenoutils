//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: ZTTY.h
//    Description:
//
//*****************************************************************************

#ifndef __ZTTY_H__
#define __ZTTY_H__

#include <linux/tty.h>
#include <termios.h>

#include <string>
#include <fstream>
#include <list>
#include <vector>

#include <zutils/zThread.h>
#include <zutils/zSerial.h>

namespace zUtils
{
namespace zSerial
{

//**********************************************************************
// Class: SerialConfiguration
//**********************************************************************

class TtySerialConfiguration : public SerialConfiguration
{

public:

  static const std::string ConfigBaudPath;
  static const std::string ConfigBaudNone;
  static const std::string ConfigBaud9600;
  static const std::string ConfigBaud19200;
  static const std::string ConfigBaud38400;
  static const std::string ConfigBaud57600;
  static const std::string ConfigBaud115200;
  static const std::string ConfigBaudDefault;

  static const std::string ConfigDatabitsPath;
  static const std::string ConfigDatabitsNone;
  static const std::string ConfigDatabits5;
  static const std::string ConfigDatabits6;
  static const std::string ConfigDatabits7;
  static const std::string ConfigDatabits8;
  static const std::string ConfigDatabitsDefault;

  static const std::string ConfigStopbitsPath;
  static const std::string ConfigStopbitsNone;
  static const std::string ConfigStopbits1;
  static const std::string ConfigStopbits2;
  static const std::string ConfigStopbitsDefault;

  static const std::string ConfigParityPath;
  static const std::string ConfigParityNone;
  static const std::string ConfigParityOdd;
  static const std::string ConfigParityEven;
  static const std::string ConfigParityDefault;

  static const std::string ConfigFlowPath;
  static const std::string ConfigFlowNone;
  static const std::string ConfigFlowHard;
  static const std::string ConfigFlowDefault;

  TtySerialConfiguration();

  TtySerialConfiguration(zData::Data &data_);

  TtySerialConfiguration(zConfig::Configuration &config_);

  virtual
  ~TtySerialConfiguration();

};

//**********************************************************************
// Class: TtyPortRecv
//**********************************************************************

class TtyPortRecv : public zThread::Function
{
public:
  TtyPortRecv()
  {

  }
  virtual
  ~TtyPortRecv()
  {
  }

  virtual void *
  ThreadFunction(void *arg_);

protected:
private:
};

//**********************************************************************
// Class: TtyPortSend
//**********************************************************************

class TtyPortSend : public zThread::Function
{
public:
  TtyPortSend()
  {

  }
  virtual
  ~TtyPortSend()
  {
  }

  virtual void *
  ThreadFunction(void *arg_);

protected:
private:
};

//**********************************************************************
// Class: TtyPort
//**********************************************************************

class TtyPort : public zSerial::SerialPort
{

  friend TtyPortRecv;
  friend TtyPortSend;

public:

  enum BAUD
  {
    BAUD_ERR = -1,
    BAUD_NONE = 0,
    BAUD_300 = B300,
    BAUD_600 = B600,
    BAUD_1200 = B1200,
    BAUD_1800 = B1800,
    BAUD_2400 = B2400,
    BAUD_4800 = B4800,
    BAUD_9600 = B9600,
    BAUD_19200 = B19200,
    BAUD_38400 = B38400,
    BAUD_57600 = B57600,
    BAUD_115200 = B115200,
    BAUD_DEF = B115200,
    BAUD_230400 = B230400,
    BAUD_LAST
  };

  enum DATABITS
  {
    DATABITS_ERR = -1,
    DATABITS_NONE = 0,
    DATABITS_5 = 5,
    DATABITS_6 = 6,
    DATABITS_7 = 7,
    DATABITS_8 = 8,
    DATABITS_DEF = 8,
    DATABITS_LAST
  };

  enum STOPBITS
  {
    STOPBITS_ERR = -1,
    STOPBITS_NONE = 0,
    STOPBITS_1 = 1,
    STOPBITS_DEF = 1,
    STOPBITS_2 = 2,
    STOPBITS_LAST
  };

  enum PARITY
  {
    PARITY_ERR = -1,
    PARITY_DEF = 0,
    PARITY_NONE = 0,
    PARITY_ODD = 1,
    PARITY_EVEN = 2,
    PARITY_LAST
  };

  enum FLOWCNTL
  {
    FLOWCNTL_ERR = -1,
    FLOWCNTL_NONE = 0,
    FLOWCNTL_DEF = 0,
    FLOWCNTL_HARD = 1,
    FLOWCNTL_LAST
  };

  TtyPort(TtyPort::BAUD baud_ = TtyPort::BAUD_DEF,
      TtyPort::DATABITS dbits_ = TtyPort::DATABITS_DEF,
      TtyPort::STOPBITS sbits_ = TtyPort::STOPBITS_DEF,
      TtyPort::PARITY parity_ = TtyPort::PARITY_DEF,
      TtyPort::FLOWCNTL flowcntl_ = TtyPort::FLOWCNTL_DEF,
      bool blocking_ = false);

  virtual
  ~TtyPort();

  virtual bool
  Open(const std::string &dev_);

  virtual bool
  Close();

  TtyPort::BAUD
  GetBaud();
  bool
  SetBaud(TtyPort::BAUD baud_);

  TtyPort::DATABITS
  GetDataBits();
  bool
  SetDataBits(TtyPort::DATABITS dbits_);

  TtyPort::STOPBITS
  GetStopBits();
  bool
  SetStopBits(TtyPort::STOPBITS sbits_);

  TtyPort::PARITY
  GetParity();
  bool
  SetParity(TtyPort::PARITY parity_);

  TtyPort::FLOWCNTL
  GetFlowControl();
  bool
  SetFlowControl(TtyPort::FLOWCNTL flowcntl_);

  bool
  GetBlocking();
  bool
  SetBlocking(bool blocking_);

protected:
  int _fd;

private:

  std::string _device;

  struct termios _termios;
  struct termios _savedTermios;

  TtyPort::BAUD _baud;
  TtyPort::DATABITS _dbits;
  TtyPort::STOPBITS _sbits;
  TtyPort::PARITY _parity;
  TtyPort::FLOWCNTL _flowcntl;
  int _options;

  zThread::Thread _rx_thread;
  TtyPortRecv _rx_func;
  zThread::Thread _tx_thread;
  TtyPortSend _tx_func;

};

}
}

#endif /* __ZTTY_H__ */
