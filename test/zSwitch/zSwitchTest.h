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

#ifndef _ZTHREADTEST_H_
#define _ZTHREADTEST_H_

#include <unistd.h>

#include <zutils/zSwitch.h>

#include "UnitTest.h"

int
zSwitchTest_Defaults(void* arg_);

using namespace Test;
using namespace zUtils;

class TestSwitch : public zSwitch::Switch
{
public:
  TestSwitch(zSwitch::Switch::STATE state_) :
      zSwitch::Switch(state_)
  {
  }

  virtual
  ~TestSwitch()
  {
  }

protected:

private:

  virtual bool
  _turnOn()
  {
    return (true);
  }

  virtual bool
  _turnOff()
  {
    return (true);
  }

};

#endif /* _ZSWITCHTEST_H_ */
