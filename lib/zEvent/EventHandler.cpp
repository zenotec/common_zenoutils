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
  this->_event_lock.Unlock();
}

Handler::~Handler()
{
  this->_event_lock.Lock();
}

bool
Handler::RegisterEvent(Event *event_)
{
  bool status = false;
  if (event_ && this->_event_lock.Lock())
  {
    // Remove any possible duplicates
    this->_event_list.remove(event_);
    event_->unregisterHandler(this);

    // Register event
    this->_event_list.push_back(event_);
    status = event_->registerHandler(this);
    status &= this->_event_lock.Unlock();
  }
  return (status);
}

bool
Handler::UnregisterEvent(Event *event_)
{
  bool status = false;
  if (event_ && this->_event_lock.Lock())
  {
    this->_event_list.remove(event_);
    status = event_->unregisterHandler(this);
    status &= this->_event_lock.Unlock();
  }
  return (status);
}

bool
Handler::RegisterObserver(Observer *obs_)
{
  bool status = false;
  if (obs_ && this->_event_lock.Lock())
  {
    // Remove any duplicates
    this->_obs_list.remove(obs_);

    // Register observer
    this->_obs_list.push_back(obs_);
    status = this->_event_lock.Unlock();
  }
  return (status);
}

bool
Handler::UnregisterObserver(Observer *obs_)
{
  bool status = false;
  if (obs_ && this->_event_lock.Lock())
  {
    // Unregister observer
    this->_obs_list.remove(obs_);
    status = this->_event_lock.Unlock();
  }
  return (status);
}

void
Handler::notifyObservers(SHARED_PTR(zEvent::Notification) noti_)
{

  // Note: never call this routine directly; Only should be called by the event class

  // Start critical section
  if (this->_event_lock.Lock())
  {
    FOREACH (auto& obs, this->_obs_list)
    {
      obs->Notify(noti_);
    }
    this->_event_lock.Unlock();
  }

  return;
}

}
}
