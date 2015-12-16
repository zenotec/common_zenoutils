//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zEvent.cpp
//    Description:
//
//*****************************************************************************

#include "zutils/zLog.h"
#include "zutils/zEvent.h"

namespace zUtils
{
namespace zEvent
{

const std::string Event::STR_ROOT = "zEvent";
const std::string Event::STR_TYPE = "Type";
const std::string Event::STR_TYPE_NONE = "NONE";
const std::string Event::STR_TYPE_TEST = "TEST";
const std::string Event::STR_TYPE_COM = "COM";
const std::string Event::STR_TYPE_SOCKET = "SOCKET";
const std::string Event::STR_TYPE_TEMP = "TEMP";
const std::string Event::STR_TYPE_GPIO = "GPIO";
const std::string Event::STR_ID = "Id";

//**********************************************************************
// Event Class
//**********************************************************************

Event::Event(Event::TYPE type_, uint32_t id_) :
    zData::Data(Event::STR_ROOT)
{
  // Initialize message
  if (!this->SetType(type_))
  {
    std::string errmsg = "Error setting event type: " + zLog::IntStr(type_);
    ZLOG_ERR(errmsg);
    throw errmsg;
  }
  if (!this->SetId(id_))
  {
    std::string errmsg = "Error setting event ID: " + zLog::IntStr(type_);
    ZLOG_ERR(errmsg);
    throw errmsg;
  }

  this->_lock.Unlock();
}

Event::~Event()
{
}

Event::TYPE
Event::GetType() const
{
  std::string type = this->GetValue(Event::STR_TYPE);
  if (type == Event::STR_TYPE_NONE)
  {
    return (Event::TYPE_NONE);
  }
  else if (type == Event::STR_TYPE_TEST)
  {
    return (Event::TYPE_TEST);
  }
  else if (type == Event::STR_TYPE_COM)
  {
    return (Event::TYPE_COM);
  }
  else if (type == Event::STR_TYPE_TEMP)
  {
    return (Event::TYPE_TEMP);
  }
  else if (type == Event::STR_TYPE_GPIO)
  {
    return (Event::TYPE_GPIO);
  }
  else if (type == Event::STR_TYPE_SOCKET)
  {
    return (Event::TYPE_SOCKET);
  }
  else
  {
    return (Event::TYPE_ERR);
  }
}

bool
Event::SetType(const Event::TYPE &type_)
{
  bool status = false;
  switch (type_)
  {
  case Event::TYPE_NONE:
    status = this->SetValue(Event::STR_TYPE, Event::STR_TYPE_NONE);
    break;
  case Event::TYPE_TEST:
    status = this->SetValue(Event::STR_TYPE, Event::STR_TYPE_TEST);
    break;
  case Event::TYPE_COM:
    status = this->SetValue(Event::STR_TYPE, Event::STR_TYPE_COM);
    break;
  case Event::TYPE_TEMP:
    status = this->SetValue(Event::STR_TYPE, Event::STR_TYPE_TEMP);
    break;
  case Event::TYPE_GPIO:
    status = this->SetValue(Event::STR_TYPE, Event::STR_TYPE_GPIO);
    break;
  case Event::TYPE_SOCKET:
    status = this->SetValue(Event::STR_TYPE, Event::STR_TYPE_SOCKET);
    break;
  default:
    status = false;
    break;
  }
  return (status);
}

uint32_t
Event::GetId()
{
  unsigned int val = 0;
  this->GetValue(Event::STR_ID, val);
  return (val);
}

bool
Event::SetId(const uint32_t &id_)
{
  return (this->SetValue(Event::STR_ID, id_));
}

void
Event::Notify(void *arg_)
{
  ZLOG_DEBUG("Notifying handlers");
  if (this->_lock.Lock())
  {
    // Make a copy of the handler list
    std::list<EventHandler *> handler_list(this->_handler_list);
    this->_lock.Unlock();

    // Notify all registered event handlers
    while (!handler_list.empty())
    {
      EventHandler *handler = handler_list.front();
      handler_list.pop_front();
      handler->notify(this, arg_);
    }
  } // end if
}

void
Event::registerHandler(EventHandler *list_)
{
  if (this->_lock.Lock())
  {
    this->_handler_list.push_front(list_);
    this->_lock.Unlock();
  } // end if
}

void
Event::unregisterHandler(EventHandler *list_)
{
  if (this->_lock.Lock())
  {
    this->_handler_list.remove(list_);
    this->_lock.Unlock();
  } // end if
}

}
}
