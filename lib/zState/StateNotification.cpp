/*
 * Copyright (c) 2014-2018 ZenoTec LLC (http://www.zenotec.net)
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

#include <zutils/zState.h>

namespace zUtils
{
namespace zState
{

//**********************************************************************
// Class: Notification
//**********************************************************************

Notification::Notification()
{
}

Notification::~Notification()
{
}

SHARED_PTR(State)
Notification::GetState() const
{
  return (this->_state);
}

bool
Notification::SetState(SHARED_PTR(State) state_)
{
  this->_state = state_;
  return (true);
}

}
}
