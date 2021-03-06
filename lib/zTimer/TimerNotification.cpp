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

#include <zutils/zTimer.h>

namespace zUtils
{
namespace zTimer
{

//**********************************************************************
// Class: Notification
//**********************************************************************

Notification::Notification(Timer& timer_) :
    zEvent::Notification(timer_), _id(timer_.GetId()), _tick(timer_.GetTicks())
{
}

Notification::~Notification()
{
}

Timer&
Notification::GetTimer()
{
  return (static_cast<Timer&>(this->GetEvent()));
}

uint32_t
Notification::GetId() const
{
  return (this->_id);
}

uint64_t
Notification::GetTicks() const
{
  return (this->_tick);
}

}
}
