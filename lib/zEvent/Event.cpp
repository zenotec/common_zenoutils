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
  this->_hlock.Unlock();
  this->_nlock.Unlock();
}

Event::~Event()
{
  this->_nlock.Lock();
  this->_hlock.Lock(); // supports consecutive locks from same thread
  std::list<Handler*> handlers = this->_handlers;
  FOREACH(auto& handler, handlers)
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
Event::notifyHandlers(SHPTR(zEvent::Notification) n_)
{
  zEvent::STATUS status = STATUS_NONE;

  // Create copy of the handler list to allow observer to register/unregister events
  std::list<Handler*> handlers = this->getHandlers();

  // Loop through and notify all handlers; stop if observer set stop flag
  if (this->_nlock.Lock())
  {
    FOREACH (auto& handler, handlers)
    {
      if (((status = handler->notifyObservers(n_)) & STATUS_STOP))
      {
        break;
      }
    }
    this->_nlock.Unlock();
  }

  return (status);
}

bool
Event::registerHandler(Handler *handler_)
{
  bool status = false;
  if (handler_ && this->_hlock.Lock())
  {
    this->_handlers.push_back(handler_);
    this->_handlers.unique();
    status = this->_hlock.Unlock();
  }
  return (status);
}

bool
Event::unregisterHandler(Handler *handler_)
{
  bool status = false;
  // Remove handler from list
  if (handler_ && this->_hlock.Lock())
  {
    this->_handlers.remove(handler_);
    status = this->_hlock.Unlock();
  }
  return (status);
}

std::list<Handler*>
Event::getHandlers()
{
  std::list<Handler*> handlers;
  // Start critical section
  if (this->_hlock.Lock())
  {
    handlers = this->_handlers;
    this->_hlock.Unlock();
  }
  return (handlers);
}

}
}
