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

#include "zSwitchTest.h"

using namespace Test;
using namespace zUtils;

int
zSwitchTest_Defaults(void* arg_)
{
  // Create test switch
  TestSwitch *mySwitch = new TestSwitch(zSwitch::Switch::STATE_OFF);
  TEST_ISNOT_NULL(mySwitch);
  TEST_FALSE(mySwitch->IsOn());
  TEST_TRUE(mySwitch->IsOff());
  delete (mySwitch);
  return (0);
}

