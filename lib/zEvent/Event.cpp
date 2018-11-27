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

// libc includes

#include <string.h>

// libc++ includes

#include <iostream>

// libzutils includes

#include <zutils/zEvent.h>
using namespace zUtils;

// local includes

namespace zUtils
{
namespace zEvent
{

//**********************************************************************
// Class: Event
//**********************************************************************

Event::Event(zEvent::TYPE type_) :
    _type(type_)
{
  this->_lock.Unlock();
}

Event::~Event()
{
  this->_lock.Lock();
  if (!this->_handler_list.empty())
  {
    fprintf(stderr, "BUG: Event not unregistered before destruction\n");
  }
  FOREACH(auto& handler, this->_handler_list)
  {
    handler->UnregisterEvent(this);
  }
}

zEvent::TYPE
Event::GetType() const
{
  return (this->_type); // since type is read only, no need to lock
}

zEvent::STATUS
Event::notifyHandlers(SHARED_PTR(zEvent::Notification) n_)
{
  zEvent::STATUS status = STATUS_NONE;

  if (this->_lock.Lock())
  {
    // Create copy to allow an observer to modify the handler list
    std::list<Handler*> handlers = this->_handler_list;
    FOREACH (auto& handler, handlers)
    {
      status = handler->notifyObservers(n_);
      if (!(status & STATUS_CONT))
      {
        break;
      }
    }
    this->_lock.Unlock();
  }

  return (status);
}

bool
Event::registerHandler(Handler *handler_)
{
  bool status = false;
  if (handler_ && this->_lock.Lock())
  {
    this->_handler_list.push_back(handler_);
    this->_handler_list.unique();
    status = true;
    this->_lock.Unlock();
  }
  return (status);
}

bool
Event::unregisterHandler(Handler *handler_)
{
  bool status = false;
  // Remove handler from list
  if (handler_ && this->_lock.Lock())
  {
    this->_handler_list.remove(handler_);
    status = true;
    this->_lock.Unlock();
  }
  return (status);
}

}
}
