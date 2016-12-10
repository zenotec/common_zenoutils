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

#include "zLedTest.h"

using namespace zUtils;
using namespace Test;

int
zLedTest_LedDefaults(void* arg)
{
  LedTest *myLed = 0;
  std::string devRoot = TESTDIR + std::string("/") + zLed::Led::LED_COM1_TX + std::string("/");

  myLed = new LedTest(zLed::Led::LED_COM1_TX);
  TEST_ISNOT_NULL(myLed);
  TEST_EQ(myLed->GetDevRoot(), devRoot);
  TEST_EQ(myLed->GetName(), zLed::Led::LED_COM1_TX);
  TEST_EQ(myLed->GetMode(), zLed::Led::MODE_NONE);
  TEST_EQ(myLed->GetState(), zLed::Led::STATE_OFF);
  delete (myLed);

  return (0);

}

enum
{
  GREEN, RED, AMBER, NUM
};

int
zLedTest_HandlerDefaults(void* arg)
{
  zLed::Handler *myHandler = 0;
  LedTest *myLed = 0;
  std::string devRoot;

  devRoot = TESTDIR + std::string("/") + zLed::Led::LED_TX_GREEN + std::string("/");

  myLed = new LedTest(zLed::Led::LED_TX_GREEN);
  TEST_ISNOT_NULL(myLed);
  TEST_EQ(myLed->GetDevRoot(), devRoot);
  TEST_EQ(myLed->GetName(), zLed::Led::LED_TX_GREEN);
  TEST_EQ(myLed->GetMode(), zLed::Led::MODE_NONE);
  TEST_EQ(myLed->GetState(), zLed::Led::STATE_OFF);

  myHandler = new zLed::Handler(zLed::Led::COLOR_GREEN, myLed);
  TEST_ISNOT_NULL(myHandler);

  // Clean up
  delete (myLed);
  delete (myHandler);

  return (0);

}

int
zLedTest_HandlerAddRemove(void* arg)
{
  zLed::Handler *myHandler = 0;
  LedTest *myLed[NUM] = { 0 };
  std::string devRoot[NUM];

  devRoot[GREEN] = TESTDIR + std::string("/") + zLed::Led::LED_TX_GREEN + std::string("/");
  devRoot[RED] = TESTDIR + std::string("/") + zLed::Led::LED_TX_RED + std::string("/");
  devRoot[AMBER] = TESTDIR + std::string("/") + zLed::Led::LED_TX_AMBER + std::string("/");

  myLed[GREEN] = new LedTest(zLed::Led::LED_TX_GREEN);
  TEST_ISNOT_NULL(myLed[GREEN]);
  TEST_EQ(myLed[GREEN]->GetDevRoot(), devRoot[GREEN]);
  TEST_EQ(myLed[GREEN]->GetName(), zLed::Led::LED_TX_GREEN);
  TEST_EQ(myLed[GREEN]->GetMode(), zLed::Led::MODE_NONE);
  TEST_EQ(myLed[GREEN]->GetState(), zLed::Led::STATE_OFF);

  myLed[RED] = new LedTest(zLed::Led::LED_TX_RED);
  TEST_ISNOT_NULL(myLed[RED]);
  TEST_EQ(myLed[RED]->GetDevRoot(), devRoot[RED]);
  TEST_EQ(myLed[RED]->GetName(), zLed::Led::LED_TX_RED);
  TEST_EQ(myLed[RED]->GetMode(), zLed::Led::MODE_NONE);
  TEST_EQ(myLed[RED]->GetState(), zLed::Led::STATE_OFF);

  myLed[AMBER] = new LedTest(zLed::Led::LED_TX_AMBER);
  TEST_ISNOT_NULL(myLed[AMBER]);
  TEST_EQ(myLed[AMBER]->GetDevRoot(), devRoot[AMBER]);
  TEST_EQ(myLed[AMBER]->GetName(), zLed::Led::LED_TX_AMBER);
  TEST_EQ(myLed[AMBER]->GetMode(), zLed::Led::MODE_NONE);
  TEST_EQ(myLed[AMBER]->GetState(), zLed::Led::STATE_OFF);

  myHandler = new zLed::Handler;
  TEST_ISNOT_NULL(myHandler);

  // Add all the LEDs and verify
  TEST_TRUE(myHandler->AddLed(zLed::Led::COLOR_GREEN, myLed[GREEN]));
  TEST_TRUE(myHandler->AddLed(zLed::Led::COLOR_RED, myLed[RED]));
  TEST_TRUE(myHandler->AddLed(zLed::Led::COLOR_AMBER, myLed[AMBER]));

  TEST_TRUE(myHandler->RemoveLed(myLed[GREEN]));
  TEST_TRUE(myHandler->RemoveLed(myLed[RED]));
  TEST_TRUE(myHandler->RemoveLed(myLed[AMBER]));

  // Clean up
  delete (myLed[GREEN]);
  delete (myLed[RED]);
  delete (myLed[AMBER]);
  delete (myHandler);

  return (0);

}

int
zLedTest_HandlerOnOffToggle(void* arg)
{
  zLed::Handler *myHandler = 0;
  LedTest *myLed[NUM] = { 0 };
  std::string devRoot[NUM];

  devRoot[GREEN] = TESTDIR + std::string("/") + zLed::Led::LED_TX_GREEN + std::string("/");
  devRoot[RED] = TESTDIR + std::string("/") + zLed::Led::LED_TX_RED + std::string("/");
  devRoot[AMBER] = TESTDIR + std::string("/") + zLed::Led::LED_TX_AMBER + std::string("/");

  myLed[GREEN] = new LedTest(zLed::Led::LED_TX_GREEN);
  TEST_ISNOT_NULL(myLed[GREEN]);
  TEST_EQ(myLed[GREEN]->GetDevRoot(), devRoot[GREEN]);
  TEST_EQ(myLed[GREEN]->GetName(), zLed::Led::LED_TX_GREEN);
  TEST_EQ(myLed[GREEN]->GetMode(), zLed::Led::MODE_NONE);
  TEST_EQ(myLed[GREEN]->GetState(), zLed::Led::STATE_OFF);

  myLed[RED] = new LedTest(zLed::Led::LED_TX_RED);
  TEST_ISNOT_NULL(myLed[RED]);
  TEST_EQ(myLed[RED]->GetDevRoot(), devRoot[RED]);
  TEST_EQ(myLed[RED]->GetName(), zLed::Led::LED_TX_RED);
  TEST_EQ(myLed[RED]->GetMode(), zLed::Led::MODE_NONE);
  TEST_EQ(myLed[RED]->GetState(), zLed::Led::STATE_OFF);

  myLed[AMBER] = new LedTest(zLed::Led::LED_TX_AMBER);
  TEST_ISNOT_NULL(myLed[AMBER]);
  TEST_EQ(myLed[AMBER]->GetDevRoot(), devRoot[AMBER]);
  TEST_EQ(myLed[AMBER]->GetName(), zLed::Led::LED_TX_AMBER);
  TEST_EQ(myLed[AMBER]->GetMode(), zLed::Led::MODE_NONE);
  TEST_EQ(myLed[AMBER]->GetState(), zLed::Led::STATE_OFF);

  myHandler = new zLed::Handler;
  TEST_ISNOT_NULL(myHandler);

  // Add the green LED and verify
  TEST_TRUE(myHandler->AddLed(zLed::Led::COLOR_GREEN, myLed[GREEN]));
  TEST_TRUE(myLed[GREEN]->TestRealState(zLed::Led::STATE_OFF));

  // Try turning on all the LEDs; Only the green LED should succeed
  TEST_TRUE(myHandler->On(zLed::Led::COLOR_GREEN));
  TEST_TRUE(myLed[GREEN]->TestRealState(zLed::Led::STATE_ON));
  TEST_FALSE(myHandler->On(zLed::Led::COLOR_RED));
  TEST_FALSE(myHandler->On(zLed::Led::COLOR_AMBER));

  // Add remaining LEDs and verify
  TEST_TRUE(myHandler->AddLed(zLed::Led::COLOR_RED, myLed[RED]));
  TEST_TRUE(myLed[RED]->TestRealState(zLed::Led::STATE_OFF));
  TEST_TRUE(myHandler->AddLed(zLed::Led::COLOR_AMBER, myLed[AMBER]));
  TEST_TRUE(myLed[AMBER]->TestRealState(zLed::Led::STATE_OFF));

  // Turn on all LEDs
  TEST_TRUE(myHandler->On(zLed::Led::COLOR_ALL));
  TEST_TRUE(myLed[GREEN]->TestRealState(zLed::Led::STATE_ON));
  TEST_TRUE(myLed[RED]->TestRealState(zLed::Led::STATE_ON));
  TEST_TRUE(myLed[AMBER]->TestRealState(zLed::Led::STATE_ON));

  // Turn off just one led
  TEST_TRUE(myHandler->Off(zLed::Led::COLOR_RED));
  TEST_TRUE(myLed[GREEN]->TestRealState(zLed::Led::STATE_ON));
  TEST_TRUE(myLed[RED]->TestRealState(zLed::Led::STATE_OFF));
  TEST_TRUE(myLed[AMBER]->TestRealState(zLed::Led::STATE_ON));

  // Toggle all LEDs
  TEST_TRUE(myHandler->Toggle(zLed::Led::COLOR_ALL));
  TEST_TRUE(myLed[GREEN]->TestRealState(zLed::Led::STATE_OFF));
  TEST_TRUE(myLed[RED]->TestRealState(zLed::Led::STATE_ON));
  TEST_TRUE(myLed[AMBER]->TestRealState(zLed::Led::STATE_OFF));

  // Turn off all LEDs
  TEST_TRUE(myHandler->Off(zLed::Led::COLOR_ALL));
  TEST_TRUE(myLed[GREEN]->TestRealState(zLed::Led::STATE_OFF));
  TEST_TRUE(myLed[RED]->TestRealState(zLed::Led::STATE_OFF));
  TEST_TRUE(myLed[AMBER]->TestRealState(zLed::Led::STATE_OFF));

  // Remove all the LEDs and verify
  TEST_TRUE(myHandler->RemoveLed(myLed[GREEN]));
  TEST_TRUE(myHandler->RemoveLed(myLed[RED]));
  TEST_TRUE(myHandler->RemoveLed(myLed[AMBER]));

  // Clean up
  delete (myLed[GREEN]);
  delete (myLed[RED]);
  delete (myLed[AMBER]);
  delete (myHandler);

  return (0);

}

