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

#include "zStateTest.h"

using namespace Test;
using namespace zUtils;

int
zStateTest_Defaults(void* arg_)
{

  // Create state handler and verify
  SHPTR(zState::Context) myContext(new zState::Context);
  TEST_ISNOT_NULL(myContext.get());
  TEST_IS_NULL(myContext->GetLastState().get());
  TEST_IS_NULL(myContext->GetState().get());
  TEST_IS_NULL(myContext->GetNextState().get());

  // Create initial state and verify
  SHPTR(TestStateLower) s1(new TestStateLower(myContext));
  TEST_ISNOT_NULL(s1.get());
  TEST_EQ(s1->GetId(), STATE_ID::ID_LOWER);

  // Set handler initial state
  TEST_TRUE(myContext->SetNextState(s1));
  TEST_IS_NULL(myContext->GetLastState().get());
  TEST_IS_NULL(myContext->GetState().get());
  TEST_EQ(myContext->GetNextState().get(), s1.get());
  TEST_EQ(myContext->GetNextState()->GetId(), STATE_ID::ID_LOWER);

  // Create test notification
  SHPTR(TestNotification) myNotification(new TestNotification(*myContext));
  TEST_ISNOT_NULL(myNotification.get());
  TEST_IS_ZERO(myNotification->GetCommand());
  TEST_EQ(std::string(""), myNotification->GetString());
  TEST_TRUE(myNotification->SetCommand(8));
  TEST_EQ(8, myNotification->GetCommand());
  TEST_TRUE(myNotification->SetString("TestString"));
  TEST_EQ(std::string("TestString"), myNotification->GetString());

  // Notify state of event and verify
  TEST_EQ(zEvent::STATUS_OK, myContext->Notify(myNotification));
  TEST_EQ(std::string("teststring"), myNotification->GetString());
  TEST_IS_NULL(myContext->GetLastState().get());
  TEST_ISNOT_NULL(myContext->GetState().get());
  TEST_EQ(myContext->GetState()->GetId(), STATE_ID::ID_LOWER);
  TEST_ISNOT_NULL(myContext->GetNextState().get());
  TEST_EQ(myContext->GetNextState()->GetId(), STATE_ID::ID_LOWER);

  // Notify state of event and verify
  TEST_EQ(zEvent::STATUS_OK, myContext->Notify(myNotification));
  TEST_EQ(std::string("teststring"), myNotification->GetString());
  TEST_ISNOT_NULL(myContext->GetLastState().get());
  TEST_EQ(myContext->GetLastState()->GetId(), STATE_ID::ID_LOWER);
  TEST_ISNOT_NULL(myContext->GetState().get());
  TEST_EQ(myContext->GetState()->GetId(), STATE_ID::ID_LOWER);
  TEST_ISNOT_NULL(myContext->GetNextState().get());
  TEST_EQ(myContext->GetNextState()->GetId(), STATE_ID::ID_UPPER);

  // Notify state of event and verify
  TEST_TRUE(myContext->Notify(myNotification));
  TEST_EQ(std::string("TESTSTRING"), myNotification->GetString());
  TEST_ISNOT_NULL(myContext->GetLastState().get());
  TEST_EQ(myContext->GetLastState()->GetId(), STATE_ID::ID_LOWER);
  TEST_ISNOT_NULL(myContext->GetState().get());
  TEST_EQ(myContext->GetState()->GetId(), STATE_ID::ID_UPPER);
  TEST_ISNOT_NULL(myContext->GetNextState().get());
  TEST_EQ(myContext->GetNextState()->GetId(), STATE_ID::ID_UPPER);

  // Notify state of event and verify
  TEST_TRUE(myContext->Notify(myNotification));
  TEST_EQ(std::string("TESTSTRING"), myNotification->GetString());
  TEST_ISNOT_NULL(myContext->GetLastState().get());
  TEST_EQ(myContext->GetLastState()->GetId(), STATE_ID::ID_UPPER);
  TEST_ISNOT_NULL(myContext->GetState().get());
  TEST_EQ(myContext->GetState()->GetId(), STATE_ID::ID_UPPER);
  TEST_ISNOT_NULL(myContext->GetNextState().get());
  TEST_EQ(myContext->GetNextState()->GetId(), STATE_ID::ID_LOWER);

  return (UTEST_PASS);
}

