//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Port.cpp
//    Description:
//
//*****************************************************************************

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include <iostream>

#include <zutils/zCom.h>

namespace zUtils
{
namespace zCom
{

//*****************************************************************************
// PortEvent Class
//*****************************************************************************
PortEvent::PortEvent(const PortEvent::EVENTID id_) :
    zEvent::Event(zEvent::Event::TYPE_COM, id_)
{
}

PortEvent::~PortEvent()
{
}

//*****************************************************************************
// Port Class
//*****************************************************************************
Port::Port() :
    rx_event(PortEvent::EVENTID_CHAR_RCVD), tx_event(PortEvent::EVENTID_CHAR_SENT)
{
  this->RegisterEvent(&this->rx_event);
  this->RegisterEvent(&this->tx_event);
}

Port::~Port()
{
  this->UnregisterEvent(&this->rx_event);
  this->UnregisterEvent(&this->tx_event);
}

ssize_t
Port::RecvBuf(void *buf_, size_t len_)
{
  ssize_t bytes = 0;
  char *buf = (char *) buf_;
  ZLOG_DEBUG("Receiving");
  while (!this->rxq.Empty() && len_--)
  {
    buf[bytes++] = this->rxq.Front();
    this->rxq.Pop();
  }
  ZLOG_DEBUG(std::string("Received Bytes: ") + zLog::IntStr(bytes));
  return (bytes);
}

bool
Port::RecvChar(char *c_)
{
  bool status = false;
  ZLOG_DEBUG("Receiving");
  if (!this->rxq.Empty())
  {
    *c_ = this->rxq.Front();
    this->rxq.Pop();
    status = true;
  }
  ZLOG_DEBUG(std::string("Received character: ") + *c_);
  return (status);
}

bool
Port::RecvString(std::string &str_)
{
  ZLOG_DEBUG("Receiving");
  while (!this->rxq.Empty())
  {
    char c = 0;
    c = this->rxq.Front();
    this->rxq.Pop();
    if (iscntrl(c))
    {
      break;
    }
    str_ += c;
  }
  ZLOG_DEBUG(std::string("Received string: '") + str_ + "'");
  return (!str_.empty());
}

ssize_t
Port::SendBuf(const void *buf_, size_t len_)
{
  ssize_t bytes = 0;
  char *buf = (char *) buf_;
  ZLOG_DEBUG("Sending " + zLog::IntStr(len_) + " bytes");
  while (len_--)
  {
    this->txq.Push(buf[bytes++]);
  }
  return (bytes);
}

bool
Port::SendChar(const char c_)
{
  ZLOG_DEBUG(std::string("Sending '") + c_ + "'");
  this->txq.Push(c_);
  return (true);
}

bool
Port::SendString(const std::string &str_)
{
  ZLOG_DEBUG("Sending string '" + str_ + "'");
  return (this->SendBuf(str_.data(), str_.size()) == str_.size());
}

}
}
