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
// Class: Adapter
//**********************************************************************

Adapter::Adapter(Event& event_) :
    Event(event_.GetType()), _event(event_)
{
}

Adapter::~Adapter()
{
}

bool
Adapter::registerHandler(Handler* handler_)
{
  return (this->_event.registerHandler(handler_));
}

bool
Adapter::unregisterHandler(Handler* handler_)
{
  return (this->_event.unregisterHandler(handler_));
}

bool
Adapter::notifyHandlers(SHARED_PTR(zEvent::Notification) n_)
{
  fprintf(stderr, "Adapter::notifyHandlers()\n");
  return (this->_event.notifyHandlers(this->AdaptEvent(n_)));
}

}
}
