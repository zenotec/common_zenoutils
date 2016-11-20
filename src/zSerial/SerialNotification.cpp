//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: SerialNotification.cpp
//    Description:
//
//*****************************************************************************

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
// Class: SerialNotification
//**********************************************************************

SerialNotification::SerialNotification(SerialPort* port_) :
    _id(SerialNotification::ID_NONE), zEvent::EventNotification(port_)
{
}

SerialNotification::~SerialNotification()
{
}

SerialNotification::ID
SerialNotification::Id() const
{
  return (this->_id);
}

void
SerialNotification::id(SerialNotification::ID id_)
{
  this->_id = id_;
  return;
}

SerialPort*
SerialNotification::Port()
{
  return (static_cast<SerialPort*>(this->GetEvent()));
}

char
SerialNotification::Data() const
{
  return (this->_c);
}

bool
SerialNotification::data(const char c_)
{
  this->_c = c_;
  return (true);
}

}
}
