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

class Port : public zEvent::Event
{

public:

  Port();

  virtual
  ~Port();

  virtual bool
  Open(const std::string &dev_);

  virtual void
  Close();

  ssize_t
  RecvBuf(void *buf_, size_t len_);

  bool
  RecvChar(char *c_);

  bool
  RecvString(std::string &str_);

  ssize_t
  SendBuf(const void *buf_, size_t len_);

  bool
  SendChar(const char c_);

  bool
  SendString(const std::string &str_);

protected:

  zQueue<char> rxq;
  zQueue<char> txq;

private:

};

}
}

#endif /* __ZCOM_H__ */
