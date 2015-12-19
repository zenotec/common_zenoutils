//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zCom.h
//    Description:
//
//*****************************************************************************

#ifndef __ZCOM_H__
#define __ZCOM_H__

#include <string>
#include <list>

#include <zutils/zEvent.h>
#include <zutils/zQueue.h>

namespace zUtils
{
namespace zCom
{

class PortEvent : public zEvent::Event
{
public:

  enum EVENTID
  {
    EVENTID_ERR = -1,
    EVENTID_NONE = 0,
    EVENTID_CHAR_RCVD = 1,
    EVENTID_CHAR_SENT = 2,
    EVENTID_CHAR_ERR = 3,
    EVENTID_LAST
  };

  PortEvent(const PortEvent::EVENTID id_);

  virtual
  ~PortEvent();

protected:

private:

};

class Port : public zEvent::EventHandler
{

public:

  Port();

  virtual
  ~Port();

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

  zQueue<char> rxq;
  PortEvent rx_event;

  zQueue<char> txq;
  PortEvent tx_event;

private:

};

}
}

#endif /* __ZCOM_H__ */
