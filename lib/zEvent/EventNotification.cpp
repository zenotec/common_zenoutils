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

#include <mutex>
#include <list>

#include <zutils/zLog.h>
#include <zutils/zEvent.h>

namespace zUtils
{
namespace zEvent
{

//**********************************************************************
// Class: EventNotification
//**********************************************************************

EventNotification::EventNotification(zEvent::Event::TYPE type_) :
    _type(type_), _event(NULL)
{
}

EventNotification::EventNotification(zEvent::Event* event_) :
    _event(event_)
{
  if (event_)
  {
    EventNotification::type(event_->Type());
  }
}

EventNotification::~EventNotification()
{
}

zEvent::Event::TYPE
EventNotification::Type() const
{
  return (EventNotification::_type);
}

void
EventNotification::type(zEvent::Event::TYPE type_)
{
  EventNotification::_type = type_;
  return;
}

zEvent::Event*
EventNotification::GetEvent() const
{
  return (EventNotification::_event);
}

}
}
