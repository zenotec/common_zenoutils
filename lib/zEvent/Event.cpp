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
  if (!this->_handler_list.empty())
  {
    fprintf(stderr, "BUG: Event not unregistered before destruction\n");
  }
  FOREACH(auto& handler, this->_handler_list)
  {
    handler->UnregisterEvent(this);
  }
}

Event::TYPE
Event::GetType() const
{
  return (this->_type); // since type is read only, no need to lock
}

bool
Event::notifyHandlers(SHARED_PTR(zEvent::Notification) noti_)
{
  bool status = false;

  if (this->_event_lock.Lock())
  {
    status = true;

    // Notify all registered event handlers
    FOREACH (auto& handler, this->_handler_list)
    {
      status &= handler->notifyObservers(noti_);
    }

    this->_event_lock.Unlock();

  }
  return (status);
}

bool
Event::registerHandler(Handler *handler_)
{
  bool status = false;
  if (handler_ && this->_event_lock.Lock())
  {
    this->_handler_list.push_back(handler_);
    this->_handler_list.unique();
    status = true;
    this->_event_lock.Unlock();
  }
  return (status);
}

bool
Event::unregisterHandler(Handler *handler_)
{
  bool status = false;
  // Remove handler from list
  if (handler_ && this->_event_lock.Lock())
  {
    this->_handler_list.remove(handler_);
    status = true;
    this->_event_lock.Unlock();
  }
  return (status);
}

}
}
