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

#include <zutils/zCompatibility.h>
#include <zutils/zEvent.h>

namespace zUtils
{
namespace zEvent
{

//**********************************************************************
// Class: EventHandler
//**********************************************************************

Handler::Handler()
{
  this->_lock.Unlock();
}

Handler::~Handler()
{
  this->_lock.Lock();
  FOREACH(auto& event, this->_event_list)
  {
    event->unregisterHandler(this);
  }
  this->_event_list.clear();
}

bool
Handler::RegisterEvent(Event *event_)
{
  bool status = false;
  if (event_ && this->_lock.Lock())
  {
    // Register event
    this->_event_list.push_back(event_);
    this->_event_list.unique();
    status = event_->registerHandler(this);
    this->_lock.Unlock();
  }
  return (status);
}

bool
Handler::UnregisterEvent(Event *event_)
{
  bool status = false;
  if (event_ && this->_lock.Lock())
  {
    this->_event_list.remove(event_);
    status = event_->unregisterHandler(this);
    this->_lock.Unlock();
  }
  return (status);
}

bool
Handler::RegisterObserver(Observer *obs_)
{
  bool status = false;
  if (obs_ && this->_lock.Lock())
  {
    // Register observer
    this->_obs_list.push_back(obs_);
    this->_obs_list.unique();
    status = true;
    this->_lock.Unlock();
  }
  return (status);
}

bool
Handler::UnregisterObserver(Observer *obs_)
{
  bool status = false;
  if (obs_ && this->_lock.Lock())
  {
    // Unregister observer
    this->_obs_list.remove(obs_);
    status = true;
    this->_lock.Unlock();
  }
  return (status);
}

bool
Handler::notifyObservers(SHARED_PTR(zEvent::Notification) noti_)
{

  bool status = false;
  // Note: never call this routine directly; Only should be called by the event class

  // Start critical section
  if (this->_lock.Lock())
  {
    status = true;

    FOREACH (auto& obs, this->_obs_list)
    {
      status &= obs->ObserveEvent(noti_);
    }

    this->_lock.Unlock();
  }

  return (status);
}

std::list<Event*>
Handler::getEvents()
{
  std::list<Event*> events;
  // Start critical section
  if (this->_lock.Lock())
  {
    events = this->_event_list;
    this->_lock.Unlock();
  }
  return (events);
}

std::list<Observer*>
Handler::getObservers()
{
  std::list<Observer*> obs;
  // Start critical section
  if (this->_lock.Lock())
  {
    obs = this->_obs_list;
    this->_lock.Unlock();
  }
  return (obs);
}

}
}
