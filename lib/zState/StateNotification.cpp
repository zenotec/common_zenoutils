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

Notification::Notification(Context& ctx_) :
    zEvent::Notification(ctx_), _cmd(0)
{
}

Notification::~Notification()
{
}

uint32_t
Notification::GetCommand() const
{
  return (this->_cmd);
}

bool
Notification::SetCommand(const uint32_t cmd_)
{
  this->_cmd = cmd_;
  return (true);
}

}
}
