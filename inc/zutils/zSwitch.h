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

#ifndef __ZSWITCH_H__
#define __ZSWITCH_H__

namespace zUtils
{
namespace zSwitch
{

class Switch
{

public:

  enum STATE
  {
    STATE_NONE = 0,
    STATE_ON = 1,
    STATE_OFF = 2,
    STATE_LAST
  };

  Switch(Switch::STATE state_);

  virtual
  ~Switch();

  bool
  On();

  bool
  Off();

  bool
  Toggle();

  bool
  IsOn();

  bool
  IsOff();

protected:

  Switch::STATE _state;

  virtual bool
  _turnOn() = 0;

  virtual bool
  _turnOff() = 0;

private:

};

}
}

#endif /* __ZSWITCH_H__ */
