//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zCom.h
//    Description:
//
//*****************************************************************************

#ifndef __ZCOM_H__
#define __ZCOM_H__

#include <linux/tty.h>
#include <termios.h>

#include <string>
#include <fstream>
#include <list>
#include <vector>

#include <zutils/zThread.h>
#include <zutils/zSem.h>

namespace zUtils
{
namespace zCom
{

class Handler;

class Port
{
  friend class Handler;

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
    PARITY_ERR = -1, PARITY_DEF = 0, PARITY_NONE = 0, PARITY_ODD = 1, PARITY_EVEN = 2, PARITY_LAST
  };

  enum FLOWCNTL
  {
    FLOWCNTL_ERR = -1, FLOWCNTL_NONE = 0, FLOWCNTL_DEF = 0, FLOWCNTL_HARD = 1, FLOWCNTL_LAST
  };

  Port(const std::string& device_, Port::BAUD baud_ = Port::BAUD_DEF,
      Port::DATABITS dbits_ = Port::DATABITS_DEF, Port::STOPBITS sbits_ = Port::STOPBITS_DEF,
      Port::PARITY parity_ = Port::PARITY_DEF, Port::FLOWCNTL flowcntl_ = Port::FLOWCNTL_DEF);
  virtual
  ~Port();

  Port::BAUD
  GetBaud();
  bool
  SetBaud(Port::BAUD baud_);

  Port::DATABITS
  GetDataBits();
  bool
  SetDataBits(Port::DATABITS dbits_);

  Port::STOPBITS
  GetStopBits();
  bool
  SetStopBits(Port::STOPBITS sbits_);

  Port::PARITY
  GetParity();
  bool
  SetParity(Port::PARITY parity_);

  Port::FLOWCNTL
  GetFlowControl();
  bool
  SetFlowControl(Port::FLOWCNTL flowcntl_);

  ssize_t
  RecvBuf(void *buf_, uint32_t len_, uint32_t usecs_);
  bool
  RecvChar(char *c_, uint32_t usecs_ = 0);

  ssize_t
  SendBuf(const void *buf_, uint32_t len_, uint32_t usecs_);
  bool
  SendChar(const char c_, uint32_t usecs_ = 0);
  bool
  SendString(const std::string& str_, uint32_t usecs_ = 0);

protected:

  bool
  _open();
  bool
  _close();

  std::string _device;

private:
  struct termios _termios;
  struct termios _savedTermios;
  int _fd;

  Port::BAUD _baud;
  Port::DATABITS _dbits;
  Port::STOPBITS _sbits;
  Port::PARITY _parity;
  Port::FLOWCNTL _flowcntl;

  zSem::Mutex _mutex;

};

class ComReceiver
{
public:
  virtual bool
  RecvChar(Port* com_, char c_) = 0;

private:

};

class Handler: public zThread::Function
{

public:
  Handler();
  virtual
  ~Handler();

  bool
  Register(ComReceiver* rcvr_);
  void
  Unregister(ComReceiver* rcvr_);

  bool
  Open(Port* com_);
  void
  Close(Port* com_ = NULL);

  bool
  StartListenerThread(uint32_t usecs_);
  bool
  StopListenerThread();

  bool
  Listen(uint32_t usecs_);


protected:

    virtual void *
    ThreadFunction( void *arg_ );

private:

  void
  _notify(Port* com_, char c_);

  std::list<ComReceiver*> _rcvrList;

  std::list<Port *> _portList;
  zThread::Thread _thread;
  zSem::Mutex _mutex;
  bool _exit;

};

}
}

#endif /* __ZCOM_H__ */
