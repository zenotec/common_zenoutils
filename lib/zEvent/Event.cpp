/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <iostream>

#include <zutils/zEvent.h>

namespace zUtils
{
namespace zEvent
{

//**********************************************************************
// Class: Event
//**********************************************************************

Event::Event(Event::TYPE type_) :
    _type(type_)
{
  this->_event_lock.Unlock();
}

Event::~Event()
{
  this->_event_lock.Lock();
}

Event::TYPE
Event::Type() const
{
  // Read only, no need for locking
  return (this->_type);
}

void
Event::Notify(zEvent::EventNotification* notification_)
{
  if (this->_event_lock.Lock())
  {
    // Notify all registered event handlers
    FOREACH (auto& handler, this->_handler_list)
    {
      handler->notify(notification_);
    }
    this->_event_lock.Unlock();
  }
  return;
}

bool
Event::registerHandler(EventHandler *handler_)
{
  bool status = false;
  if (handler_ && this->_event_lock.Lock())
  {
    this->_handler_list.push_front(handler_);
    status = this->_event_lock.Unlock();
  }
  return (status);
}

bool
Event::unregisterHandler(EventHandler *handler_)
{
  bool status = false;
  // Remove handler from list
  if (handler_ && this->_event_lock.Lock())
  {
    this->_handler_list.remove(handler_);
    status = this->_event_lock.Unlock();
  }
  return (status);
}

}
}
