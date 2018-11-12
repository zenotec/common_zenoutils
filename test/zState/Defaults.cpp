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
  zState::Handler* myHandler = new zState::Handler;
  TEST_ISNOT_NULL(myHandler);
  TEST_IS_NULL(myHandler->GetState().get());

  // Create initial state and verify
  SHARED_PTR(TestStateLower) s1(new TestStateLower);
  TEST_ISNOT_NULL(s1.get());
  TEST_EQ(s1->GetId(), STATE_ID::ID_LOWER);

  // Set handler initial state
  TEST_TRUE(myHandler->SetState(s1));
  TEST_EQ(myHandler->GetState().get(), s1.get());
  TEST_EQ(myHandler->GetState()->GetId(), STATE_ID::ID_LOWER);

  // Create test notification
  SHARED_PTR(TestNotification) n(new TestNotification);
  TEST_ISNOT_NULL(n.get());
  TEST_TRUE(n->SetString("TestString"));
  TEST_EQ(std::string("TestString"), n->GetString());

  // Notify state of event and verify
  TEST_TRUE(myHandler->Notify(n));
  TEST_EQ(std::string("teststring"), n->GetString());
  TEST_EQ(myHandler->GetState()->GetId(), STATE_ID::ID_LOWER);

  // Notify state of event and verify
  TEST_TRUE(myHandler->Notify(n));
  TEST_EQ(std::string("teststring"), n->GetString());
  TEST_EQ(myHandler->GetState()->GetId(), STATE_ID::ID_UPPER);

  // Notify state of event and verify
  TEST_TRUE(myHandler->Notify(n));
  TEST_EQ(std::string("TESTSTRING"), n->GetString());
  TEST_EQ(myHandler->GetState()->GetId(), STATE_ID::ID_UPPER);

  // Notify state of event and verify
  TEST_TRUE(myHandler->Notify(n));
  TEST_EQ(std::string("TESTSTRING"), n->GetString());
  TEST_EQ(myHandler->GetState()->GetId(), STATE_ID::ID_LOWER);

  return (UTEST_PASS);
}

