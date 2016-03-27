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
zGpioTest_SwitchOnOff( void* arg )
{
    // Create new GPIO port test configuration and verify
    TestPortConf *myConf = new TestPortConf( 1 );
    TEST_ISNOT_NULL( myConf );

    // Create new GPIO port and verify
    zGpio::GpioPort *myPort = new zGpio::GpioPort( *myConf );
    TEST_ISNOT_NULL( myPort );
    TEST_EQ( zGpio::GpioPort::DIR_DEF, myPort->GetDirection() );
    TEST_EQ( zGpio::GpioPort::STATE_ERR, myPort->GetState() );
    TEST_EQ( zGpio::GpioPort::EDGE_DEF, myPort->GetEdge() );

    // Configure port to be output
    TEST_TRUE( myPort->SetDirection( zGpio::GpioPort::DIR_OUT ) );

    // Create new Switch and verify
    zGpio::GpioSwitch* mySwitch = new zGpio::GpioSwitch( zSwitch::Switch::STATE_OFF );
    TEST_ISNOT_NULL( mySwitch );
    TEST_FALSE( mySwitch->IsOn() );
    TEST_TRUE( mySwitch->IsOff() );
    TEST_IS_NULL( mySwitch->GetPort( 1 ) );

    // Add port to switch and verify
    TEST_TRUE( mySwitch->AddPort( myPort ) );
    TEST_ISNOT_NULL( mySwitch->GetPort( 1 ) );

    // Turn switch off and verify
    TEST_TRUE( mySwitch->Off() );
    TEST_EQ( zGpio::GpioPort::STATE_INACTIVE, myPort->GetState() );
    TEST_EQ( zGpio::GpioPort::STATE_INACTIVE, mySwitch->GetState() );

    // Turn switch on and verify
    TEST_TRUE( mySwitch->On() );
    TEST_EQ( zGpio::GpioPort::STATE_ACTIVE, myPort->GetState() );
    TEST_EQ( zGpio::GpioPort::STATE_ACTIVE, mySwitch->GetState() );

    // Toggle switch off and verify
    TEST_TRUE( mySwitch->Toggle() );
    TEST_EQ( zGpio::GpioPort::STATE_INACTIVE, myPort->GetState() );
    TEST_EQ( zGpio::GpioPort::STATE_INACTIVE, mySwitch->GetState() );

    // Toggle switch on and verify
    TEST_TRUE( mySwitch->Toggle() );
    TEST_EQ( zGpio::GpioPort::STATE_ACTIVE, myPort->GetState() );
    TEST_EQ( zGpio::GpioPort::STATE_ACTIVE, mySwitch->GetState() );

    // Clean up
    delete (mySwitch);
    delete (myPort);
    delete (myConf);
    return (0);
}

