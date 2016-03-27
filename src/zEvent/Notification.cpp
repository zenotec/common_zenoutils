//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

#include <stdint.h>

#include <mutex>
#include <list>

#include <zutils/zLog.h>
#include <zutils/zEvent.h>

namespace zUtils
{
namespace zEvent
{

//**********************************************************************
// Class: Notification
//**********************************************************************

EventNotification::EventNotification() :
    _type(Event::TYPE_NONE)
{

}

EventNotification::~EventNotification()
{

}

Event::TYPE
EventNotification::Type() const
{
  return(this->_type);
}

bool
EventNotification::type(const Event::TYPE type_)
{
  bool status = false;
  if ((type_ > Event::TYPE_NONE) && (type_ < Event::TYPE_LAST))
  {
    this->_type = type_;
    status = true;
  }
  return(status);
}

}
}
