//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: SerialPort.cpp
//    Description:
//
//*****************************************************************************

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include <list>
#include <mutex>

#include <zutils/zQueue.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>
#include <zutils/zSerial.h>

namespace zUtils
{
namespace zSerial
{

//**********************************************************************
// Class: SerialPort
//**********************************************************************

SerialPort::SerialPort() :
    zEvent::Event(zEvent::Event::TYPE_SERIAL)
{
}

SerialPort::SerialPort(zConfig::Configuration &config_) :
    zSerial::SerialConfiguration(config_), zEvent::Event(zEvent::Event::TYPE_SERIAL)
{
}

SerialPort::~SerialPort()
{
  this->Close();
}

bool
SerialPort::Open()
{
}

bool
SerialPort::Close()
{
}

ssize_t
SerialPort::RecvBuf(void *buf_, size_t len_, size_t timeout_)
{
  ssize_t bytes = 0;
  char *buf = (char *) buf_;
  ZLOG_DEBUG("Receiving");
  while (this->_rxq.TimedWait(timeout_) && len_--)
  {
    buf[bytes++] = this->_rxq.Front();
    this->_rxq.Pop();
  }
  ZLOG_DEBUG(std::string("Received Bytes: ") + zLog::IntStr(bytes));
  return (bytes);
}

bool
SerialPort::RecvChar(char *c_, size_t timeout_)
{
  bool status = false;
  ZLOG_DEBUG("Receiving");
  if (this->_rxq.TimedWait(timeout_))
  {
    *c_ = this->_rxq.Front();
    this->_rxq.Pop();
    status = true;
    ZLOG_DEBUG(std::string("Received character: ") + *c_);
  }
  return (status);
}

bool
SerialPort::RecvString(std::string &str_, size_t timeout_)
{
  bool status = false;
  ZLOG_DEBUG("Receiving");
  str_.clear();
  while (this->_rxq.TimedWait(timeout_))
  {
    char c = 0;
    c = this->_rxq.Front();
    this->_rxq.Pop();
    ZLOG_DEBUG(std::string("Received character: ") + c);
    if (iscntrl(c))
    {
      status = true;
      break;
    }
    str_ += c;
  }
  ZLOG_DEBUG(std::string("Received string: '") + str_ + "'");
  return (status);
}

ssize_t
SerialPort::SendBuf(const void *buf_, size_t len_)
{

  ssize_t bytes = 0;
  char *buf = (char *) buf_;
  ZLOG_DEBUG("Sending " + zLog::IntStr(len_) + " bytes");
  while (len_--)
  {
    this->_txq.Push(buf[bytes++]);
  }
  return (bytes);
}

bool
SerialPort::SendChar(const char c_)
{
  ZLOG_DEBUG(std::string("Sending '") + c_ + "'");
  this->_txq.Push(c_);
  return (true);
}

bool
SerialPort::SendString(const std::string &str_)
{
  ZLOG_DEBUG("Sending string '" + str_ + "'");
  return (this->SendBuf(str_.data(), str_.size()) == str_.size());
}

bool
SerialPort::rxchar(const char c_)
{
  ZLOG_DEBUG(std::string("Processing rxchar '") + c_ + "'");
  this->_rxq.Push(c_);
  SerialNotification notification(SerialNotification::ID_CHAR_RCVD, this);
  this->Notify(&notification);
  return (true);
}

bool
SerialPort::txchar(char *c_, size_t timeout_)
{
  bool status = false;
  ZLOG_DEBUG("Getting txchar");
  if (this->_txq.TimedWait(timeout_))
  {
    *c_ = this->_txq.Front();
    this->_txq.Pop();
    SerialNotification notification(SerialNotification::ID_CHAR_SENT, this);
    this->Notify(&notification);
    status = true;
    ZLOG_DEBUG(std::string("Got character: ") + *c_);
  }
  return (status);
}


}
}
