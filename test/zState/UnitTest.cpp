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

#include <ctype.h>

#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include "zStateTest.h"
#include "UnitTest.h"

//**********************************************************************
// Class: TestStateUpper
//**********************************************************************

TestStateUpper::TestStateUpper(zState::Handler& handler_) :
    zState::State(handler_, STATE_ID::ID_UPPER)
{
}

TestStateUpper::~TestStateUpper()
{
}

bool
TestStateUpper::ObserveEvent(SHARED_PTR(zEvent::Notification)n_)
{
  int cnt = 0;
  SHARED_PTR(TestNotification) n (STATIC_CAST(TestNotification)(n_));
  std::string str = n->GetString();
  FOREACH (auto& c, str)
  {
    if (islower(c))
    {
      c = toupper(c);
      cnt++;
    }
  }
  if (!cnt)
  {
    SHARED_PTR(TestStateLower) s(new TestStateLower(this->GetHandler()));
    this->GetHandler().SetNextState(s);
  }
  return (n->SetString(str));
}


//**********************************************************************
// Class: TestStateLower
//**********************************************************************

TestStateLower::TestStateLower(zState::Handler& handler_) :
    zState::State(handler_, STATE_ID::ID_LOWER)
{
}

TestStateLower::~TestStateLower()
{
}

bool
TestStateLower::ObserveEvent(SHARED_PTR(zEvent::Notification)n_)
{
  SHARED_PTR(TestNotification) n (STATIC_CAST(TestNotification)(n_));
  std::string str = n->GetString();
  int cnt = 0;
  FOREACH (auto& c, str)
  {
    if (isupper(c))
    {
      c = tolower(c);
      cnt++;
    }
  }
  if (!cnt)
  {
    SHARED_PTR(TestStateUpper) s(new TestStateUpper(this->GetHandler()));
    this->GetHandler().SetNextState(STATIC_CAST(zState::State)(s));
  }
  return (n->SetString(str));
}


int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zLog::FileConnector conn("UnitTest.zlog");
  zLog::Manager::Instance().RegisterConnector(zLog::Log::LEVEL_ALL, &conn);
  zLog::Manager::Instance().SetMaxLevel(zLog::Log::MODULE_TEST, zLog::Log::LEVEL_DEBUG);

  // Test all classes
  UTEST_INIT();
  UTEST_TEST(zStateTest_Defaults, 0);

  zLog::Manager::Instance().UnregisterConnector(zLog::Log::LEVEL_ALL);

  UTEST_FINI();

}
