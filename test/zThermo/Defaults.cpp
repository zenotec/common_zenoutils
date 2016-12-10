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

#include <string>
#include <mutex>
#include <list>
#include <map>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>
#include <zutils/zThermo.h>

#include "UnitTest.h"
#include "zThermoTest.h"

using namespace Test;
using namespace zUtils;

int
zThermoTest_Defaults(void* arg_)
{
  // Create new configuration data item and verify
  TestSensor *MySensor = new TestSensor;
  TEST_ISNOT_NULL(MySensor);

  // Cleanup
  delete (MySensor);

  // Return success
  return (0);
}

