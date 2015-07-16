/*
 * GpioHandlerTest.cpp
 *
 *  Created on: Dec 23, 2014
 *      Author: freewave
 */

#include "zGpioTest.h"

using namespace zUtils;
using namespace Test;

int
zGpioTest_PortDefaults(void* arg)
{
  // Create new GPIO port and verify
  TestGpioPort* myPort = new TestGpioPort(1);
  TEST_ISNOT_NULL(myPort);
  TEST_EQ(zGpio::Port::DIR_IN, myPort->GetDirection());
  TEST_EQ(zGpio::Port::STATE_NONE, myPort->GetState());

  // Clean up
  delete (myPort);
  return (0);
}

int
zGpioTest_HandlerDefaults(void* arg)
{

  // Create new GPIO handler and verify
  zGpio::Handler* myHandler = new zGpio::Handler;
  TEST_ISNOT_NULL(myHandler);
  TEST_IS_NULL(myHandler->GetPort(1));

  // Clean up
  delete (myHandler);
  return (0);
}

int
zGpioTest_HandlerAddRemove(void* arg)
{
  // Create new GPIO port and verify
  TestGpioPort* myPort = new TestGpioPort(1);
  TEST_ISNOT_NULL(myPort);
  TEST_EQ(zGpio::Port::DIR_IN, myPort->GetDirection());
  TEST_EQ(zGpio::Port::STATE_NONE, myPort->GetState());

  // Create new GPIO handler and verify
  zGpio::Handler* myHandler = new zGpio::Handler;
  TEST_ISNOT_NULL(myHandler);

  // Try to get nonexistent GPIO; should fail
  zGpio::Port* expGpioPort = myHandler->GetPort(1);
  TEST_IS_NULL(expGpioPort);

  // Add GpioPort and verify
  TEST_TRUE(myHandler->AddPort(myPort));
  expGpioPort = myHandler->GetPort(1);
  TEST_EQ((int )expGpioPort->GetId(), (int )myPort->GetId());

  // Remove GpioPort and verify
  TEST_TRUE(myHandler->RemovePort(myPort));
  expGpioPort = myHandler->GetPort(1);
  TEST_IS_NULL(expGpioPort);

  // Clean up
  delete (myHandler);
  delete (myPort);
  return (0);
}

int
zGpioTest_HandlerOnOff(void* arg)
{
  // Create new GPIO port and verify
  TestGpioPort* myPort1 = new TestGpioPort(1, zGpio::Port::DIR_OUT, zGpio::Port::STATE_INACTIVE);
  TEST_ISNOT_NULL(myPort1);
  TEST_EQ(zGpio::Port::DIR_OUT, myPort1->GetDirection());
  TEST_EQ(zGpio::Port::STATE_INACTIVE, myPort1->GetState());

  // Create new GPIO port and verify
  TestGpioPort* myPort2 = new TestGpioPort(2, zGpio::Port::DIR_OUT, zGpio::Port::STATE_ACTIVE);
  TEST_ISNOT_NULL(myPort2);
  TEST_EQ(zGpio::Port::DIR_OUT, myPort2->GetDirection());
  TEST_EQ(zGpio::Port::STATE_ACTIVE, myPort2->GetState());

  // Create new GPIO handler and verify
  zGpio::Handler* myHandler = new zGpio::Handler;
  TEST_ISNOT_NULL(myHandler);

  // Add GpioPort and verify
  TEST_TRUE(myHandler->AddPort(myPort1));
  zGpio::Port* expGpioPort = myHandler->GetPort(1);
  TEST_EQ((int )expGpioPort->GetId(), (int )myPort1->GetId());
  TEST_EQ(zGpio::Port::STATE_INACTIVE, myPort1->GetState());
  TEST_EQ(zGpio::Port::DIR_OUT, myPort1->GetDirection());

  // Add GpioPort and verify
  TEST_TRUE(myHandler->AddPort(myPort2));
  expGpioPort = myHandler->GetPort(2);
  TEST_EQ((int )expGpioPort->GetId(), (int )myPort2->GetId());
  TEST_EQ(zGpio::Port::STATE_ACTIVE, myPort2->GetState());
  TEST_EQ(zGpio::Port::DIR_OUT, myPort2->GetDirection());

  // Set state to inactive
  TEST_TRUE(myHandler->SetState(zGpio::Port::STATE_INACTIVE));
  TEST_EQ(zGpio::Port::STATE_INACTIVE, myPort1->GetState());
  TEST_EQ(zGpio::Port::STATE_INACTIVE, myPort2->GetState());
  TEST_EQ(zGpio::Port::STATE_INACTIVE, myHandler->GetState());

  // Set state to active
  TEST_TRUE(myHandler->SetState(zGpio::Port::STATE_ACTIVE));
  TEST_EQ(zGpio::Port::STATE_ACTIVE, myPort1->GetState());
  TEST_EQ(zGpio::Port::STATE_ACTIVE, myPort2->GetState());
  TEST_EQ(zGpio::Port::STATE_ACTIVE, myHandler->GetState());

  // Set state to inactive
  TEST_TRUE(myHandler->SetState(zGpio::Port::STATE_INACTIVE));
  TEST_EQ(zGpio::Port::STATE_INACTIVE, myPort1->GetState());
  TEST_EQ(zGpio::Port::STATE_INACTIVE, myPort2->GetState());
  TEST_EQ(zGpio::Port::STATE_INACTIVE, myHandler->GetState());

  // Remove GpioPort and verify
  TEST_TRUE(myHandler->RemovePort(myPort1));
  expGpioPort = myHandler->GetPort(1);
  TEST_IS_NULL(expGpioPort);

  // Clean up
  delete (myHandler);
  delete (myPort1);
  delete (myPort2);
  return (0);
}

int
zGpioTest_SwitchDefaults(void* arg)
{

  // Create new Switch and verify
  zGpio::Switch* mySwitch = new zGpio::Switch(zSwitch::Switch::STATE_OFF);
  TEST_ISNOT_NULL(mySwitch);
  TEST_FALSE(mySwitch->IsOn());
  TEST_TRUE(mySwitch->IsOff());
  TEST_IS_NULL(mySwitch->GetPort(1));

  // Clean up
  delete (mySwitch);
  return (0);
}

int
zGpioTest_SwitchOnOff(void* arg)
{
  // Create new GPIO port and verify
  TestGpioPort* myPort = new TestGpioPort(1, zGpio::Port::DIR_OUT, zGpio::Port::STATE_INACTIVE);
  TEST_ISNOT_NULL(myPort);
  TEST_EQ(zGpio::Port::DIR_OUT, myPort->GetDirection());
  TEST_EQ(zGpio::Port::STATE_INACTIVE, myPort->GetState());

  // Create new Switch and verify
  zGpio::Switch* mySwitch = new zGpio::Switch(zSwitch::Switch::STATE_OFF);
  TEST_ISNOT_NULL(mySwitch);
  TEST_FALSE(mySwitch->IsOn());
  TEST_TRUE(mySwitch->IsOff());
  TEST_IS_NULL(mySwitch->GetPort(1));

  // Add port to switch and verify
  TEST_TRUE(mySwitch->AddPort(myPort));
  TEST_ISNOT_NULL(mySwitch->GetPort(1));

  // Turn switch off and verify
  TEST_TRUE(mySwitch->Off());
  TEST_EQ(zGpio::Port::STATE_INACTIVE, myPort->GetState());
  TEST_EQ(zGpio::Port::STATE_INACTIVE, mySwitch->GetState());

  // Turn switch on and verify
  TEST_TRUE(mySwitch->On());
  TEST_EQ(zGpio::Port::STATE_ACTIVE, myPort->GetState());
  TEST_EQ(zGpio::Port::STATE_ACTIVE, mySwitch->GetState());

  // Toggle switch off and verify
  TEST_TRUE(mySwitch->Toggle());
  TEST_EQ(zGpio::Port::STATE_INACTIVE, myPort->GetState());
  TEST_EQ(zGpio::Port::STATE_INACTIVE, mySwitch->GetState());

  // Toggle switch on and verify
  TEST_TRUE(mySwitch->Toggle());
  TEST_EQ(zGpio::Port::STATE_ACTIVE, myPort->GetState());
  TEST_EQ(zGpio::Port::STATE_ACTIVE, mySwitch->GetState());

  // Clean up
  delete (mySwitch);
  delete (myPort);
  return (0);
}

