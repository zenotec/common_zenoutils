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

#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zThread.h>
#include <zutils/zData.h>
#include <zutils/zConfig.h>
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

  static const std::string ConfigDevicePath;

  static const std::string ConfigBaudPath;
  static const std::string ConfigBaud9600;
  static const std::string ConfigBaud19200;
  static const std::string ConfigBaud38400;
  static const std::string ConfigBaud57600;
  static const std::string ConfigBaud115200;
  static const std::string ConfigBaudDefault;

  static const std::string ConfigDatabitsPath;
  static const std::string ConfigDatabits5;
  static const std::string ConfigDatabits6;
  static const std::string ConfigDatabits7;
  static const std::string ConfigDatabits8;
  static const std::string ConfigDatabitsDefault;

  static const std::string ConfigStopbitsPath;
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

  std::string
  GetDevice() const;

  bool
  SetDevice(const std::string& dev_);

  std::string
  GetBaud() const;

  bool
  SetBaud(const std::string& baud_);

  std::string
  GetDataBits() const;

  bool
  SetDataBits(const std::string& dbits_);

  std::string
  GetStopBits() const;

  bool
  SetStopBits(const std::string& sbits_);

  std::string
  GetParity() const;

  bool
  SetParity(const std::string parity_);

  std::string
  GetFlowControl() const;

  bool
  SetFlowControl(const std::string& flowcntl_);

};

//**********************************************************************
// Class: TtyPortRecv
//**********************************************************************

class TtyPortRecv : public zThread::ThreadFunction
{
public:
  TtyPortRecv()
  {

  }
  virtual
  ~TtyPortRecv()
  {
  }

  virtual void
  Run(zThread::ThreadArg *arg_);

protected:
private:
};

//**********************************************************************
// Class: TtyPortSend
//**********************************************************************

class TtyPortSend : public zThread::ThreadFunction
{
public:
  TtyPortSend()
  {

  }
  virtual
  ~TtyPortSend()
  {
  }

  virtual void
  Run(zThread::ThreadArg *arg_);

protected:
private:
};

//**********************************************************************
// Class: TtyPort
//**********************************************************************

class TtySerialPort :
    public zSerial::SerialPort, public zThread::ThreadArg
{

  friend TtyPortRecv;
  friend TtyPortSend;

public:

  enum BAUD
  {
    BAUD_ERR = -1,
    BAUD_NONE = 0,
    BAUD_300 = 1,
    BAUD_600 = 2,
    BAUD_1200 = 3,
    BAUD_1800 = 4,
    BAUD_2400 = 5,
    BAUD_4800 = 6,
    BAUD_9600 = 7,
    BAUD_19200 = 8,
    BAUD_38400 = 9,
    BAUD_57600 = 10,
    BAUD_115200 = 11,
    BAUD_DEF = BAUD_115200,
    BAUD_230400 = 12,
    BAUD_LAST
  };

  enum DATABITS
  {
    DATABITS_ERR = -1,
    DATABITS_NONE = 0,
    DATABITS_5 = 1,
    DATABITS_6 = 2,
    DATABITS_7 = 3,
    DATABITS_8 = 4,
    DATABITS_DEF = DATABITS_8,
    DATABITS_LAST
  };

  enum STOPBITS
  {
    STOPBITS_ERR = -1,
    STOPBITS_NONE = 0,
    STOPBITS_1 = 1,
    STOPBITS_DEF = STOPBITS_1,
    STOPBITS_2 = 2,
    STOPBITS_LAST
  };

  enum PARITY
  {
    PARITY_ERR = -1,
    PARITY_NONE = 0,
    PARITY_DEF = PARITY_NONE,
    PARITY_ODD = 1,
    PARITY_EVEN = 2,
    PARITY_LAST
  };

  enum FLOWCNTL
  {
    FLOWCNTL_ERR = -1,
    FLOWCNTL_NONE = 0,
    FLOWCNTL_DEF = FLOWCNTL_NONE,
    FLOWCNTL_HARD = 1,
    FLOWCNTL_LAST
  };

  TtySerialPort();

  TtySerialPort(const TtySerialConfiguration& config_);

  virtual
  ~TtySerialPort();

  virtual bool
  Open();

  virtual bool
  Close();

  std::string
  GetDevice() const;

  bool
  SetDevice(const std::string &dev_);

  TtySerialPort::BAUD
  GetBaud() const;

  bool
  SetBaud(TtySerialPort::BAUD baud_);

  TtySerialPort::DATABITS
  GetDataBits() const;

  bool
  SetDataBits(TtySerialPort::DATABITS dbits_);

  TtySerialPort::STOPBITS
  GetStopBits() const;

  bool
  SetStopBits(TtySerialPort::STOPBITS sbits_);

  TtySerialPort::PARITY
  GetParity() const;

  bool
  SetParity(TtySerialPort::PARITY parity_);

  TtySerialPort::FLOWCNTL
  GetFlowControl() const;

  bool
  SetFlowControl(TtySerialPort::FLOWCNTL flowcntl_);

  bool
  GetBlocking() const;

  bool
  SetBlocking(bool blocking_);

protected:

  int _fd;

private:

  TtySerialConfiguration _config;

  struct termios _termios;
  struct termios _savedTermios;
  int _options;

  zThread::Thread _rx_thread;
  TtyPortRecv _rx_func;
  zThread::Thread _tx_thread;
  TtyPortSend _tx_func;

};

}
}

#endif /* __ZTTY_H__ */
