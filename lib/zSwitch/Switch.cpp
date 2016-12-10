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

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "zutils/zSwitch.h"
#include "zutils/zLog.h"

namespace zUtils
{
namespace zSwitch
{

//*****************************************************************************
// Class Switch
//*****************************************************************************

Switch::Switch(Switch::STATE state_) :
    _state(state_)
{
}

Switch::~Switch()
{
}

bool
Switch::On()
{
  if (!this->IsOn() && this->_turnOn())
  {
    this->_state = Switch::STATE_ON;
  }
  return (this->IsOn());
}

bool
Switch::Off()
{
  if (!this->IsOff() && this->_turnOff())
  {
    this->_state = Switch::STATE_OFF;
  }
  return (this->IsOff());
}

bool
Switch::Toggle()
{
  // If in any other state than off, first toggle off
  if (!this->IsOff() && this->_turnOff())
  {
    this->_state = Switch::STATE_OFF;
    return (true);
  }

  // Only if it is in off state, toggle on
  else if (this->IsOff() && this->_turnOn())
  {
    this->_state = Switch::STATE_ON;
    return (true);
  }

  // Everything else should cause error
  else
  {
    return (false);
  }

}

bool
Switch::IsOn()
{
  return (this->_state == Switch::STATE_ON);
}

bool
Switch::IsOff()
{
  return (this->_state == Switch::STATE_OFF);
}

}
}
