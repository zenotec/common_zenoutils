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

#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include "UnitTest.h"
#include "Nl80211Test.h"
using namespace Test;

#include <zutils/nl80211/ListPhysCommand.h>
#include <zutils/nl80211/GetPhyCommand.h>

int
Nl80211Test_Phy(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Nl80211Test_Phy()");
  ZLOG_DEBUG("#############################################################");

  // First find a radio we can test with, if none exist, bypass test
  nl80211::ListPhysCommand physcmd;
  TEST_TRUE(physcmd.Exec());
  std::map<int, std::string> phys = physcmd();
  if (phys.empty())
  {
    UTEST_BYPASS;
  }

  FOREACH(auto& phy, phys)
  {
    nl80211::GetPhyCommand phycmd(0);
    phycmd.PhyIndex(phy.first);
    TEST_TRUE(phycmd.Exec());
  }

  // Return success
  return (0);
}


