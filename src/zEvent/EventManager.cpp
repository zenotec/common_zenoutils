//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Manager.cpp
//    Description:
//
//*****************************************************************************

#include "zutils/zLog.h"
#include "zutils/zEvent.h"

namespace zUtils
{
namespace zEvent
{

//**********************************************************************
// EventManager Class
//**********************************************************************

EventManager &
EventManager::GetInstance()
{
  static EventManager instance;
  return instance;
}

EventManager::EventManager()
{
}

EventManager::~EventManager()
{

}

bool
EventManager::RegisterEvent(Event *event_)
{
  bool status = false;
  if (event_)
  {
    Event::TYPE type = event_->GetType();
    if (type < Event::TYPE_LAST)
    {
      this->_handlers[type].RegisterEvent(event_);
      status = true;
    }
  }
  return(status);
}

bool
EventManager::UnregisterEvent(Event *event_)
{
  bool status = false;
  if (event_)
  {
    Event::TYPE type = event_->GetType();
    if (type < Event::TYPE_LAST)
    {
      this->_handlers[type].UnregisterEvent(event_);
      status = true;
    }
  }
  return(status);
}

bool
EventManager::RegisterObserver(Event::TYPE type_, EventObserver *obs_)
{
  bool status = false;
  if (type_ < Event::TYPE_LAST && obs_)
  {
    this->_handlers[type_].RegisterObserver(obs_);
    status = true;
  }
  return(status);
}

bool
EventManager::UnregisterObserver(Event::TYPE type_, EventObserver *obs_)
{
  bool status = false;
  if (type_ < Event::TYPE_LAST && obs_)
  {
    this->_handlers[type_].UnregisterObserver(obs_);
    status = true;
  }
  return(status);
}

}
}
