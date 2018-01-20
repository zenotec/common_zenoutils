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

#include <stdint.h>

#include <iostream>
#include <mutex>
#include <list>
#include <queue>
#include <vector>

#include <zutils/zEvent.h>

namespace zUtils
{
namespace zEvent
{

//**********************************************************************
// Class: EventHandler
//**********************************************************************

EventHandler::EventHandler()
{
  this->_event_lock.Unlock();
}

EventHandler::~EventHandler()
{
  this->_event_lock.Lock();
}

bool
EventHandler::RegisterEvent(Event *event_)
{
  bool status = false;
  if (event_ && this->_event_lock.TimedLock(1000))
  {
    // Remove any possible duplicates
    this->_event_list.remove(event_);
    event_->unregisterHandler(this);

    // Register event
    this->_event_list.push_back(event_);
    status = event_->registerHandler(this);
    status &= this->_event_lock.Unlock();
  }
  else
  {
    std::cerr << "Timed out waiting for event handler lock!!" << std::endl;
  }
  return (status);
}

bool
EventHandler::UnregisterEvent(Event *event_)
{
  bool status = false;
  if (event_ && this->_event_lock.TimedLock(1000))
  {
    this->_event_list.remove(event_);
    status = event_->unregisterHandler(this);
    status &= this->_event_lock.Unlock();
  }
  else
  {
    std::cerr << "Timed out waiting for event handler lock!!" << std::endl;
  }
  return (status);
}

bool
EventHandler::RegisterObserver(EventObserver *obs_)
{
  bool status = false;
  if (obs_ && this->_event_lock.TimedLock(1000))
  {
    // Remove any duplicates
    this->_obs_list.remove(obs_);

    // Register observer
    this->_obs_list.push_back(obs_);
    status = this->_event_lock.Unlock();
  }
  else
  {
    std::cerr << "Timed out waiting for event handler lock!!" << std::endl;
  }
  return (status);
}

bool
EventHandler::UnregisterObserver(EventObserver *obs_)
{
  bool status = false;
  if (obs_ && this->_event_lock.TimedLock(1000))
  {
    // Unregister observer
    EventHandler::_obs_list.remove(obs_);
    status = this->_event_lock.Unlock();
  }
  else
  {
    std::cerr << "Timed out waiting for event handler lock!!" << std::endl;
  }
  return (status);
}

void
EventHandler::notify(EventNotification* notification_)
{

  // Note: never call this routine directly; Only should be called by the event class

  // Start critical section
  if (this->_event_lock.TimedLock(1000))
  {
    FOREACH (auto& obs, this->_obs_list)
    {
      obs->EventHandler(notification_);
    }
    this->_event_lock.Unlock();
  }
  else
  {
    std::cerr << "Timed out waiting for event handler lock!!" << std::endl;
  }

  return;
}

}
}
