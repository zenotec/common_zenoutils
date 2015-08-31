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
zGpioTest_PortDefaults( void* arg )
{

    ZLOG_DEBUG( "#############################################################" );
    ZLOG_DEBUG( "# zGpioTest_PortDefaults()" );
    ZLOG_DEBUG( "#############################################################" );

    // Create new GPIO port and verify
    zGpio::Port *myPort = new zGpio::Port( 1 );
    TEST_ISNOT_NULL( myPort );
    TEST_EQ( zGpio::Port::DIR_DEF, myPort->GetDirection() );
    TEST_EQ( zGpio::Port::STATE_ERR, myPort->GetState() );
    TEST_EQ( zGpio::Port::EDGE_DEF, myPort->GetEdge() );

    // Clean up
    delete (myPort);
    return (0);
}

int
zGpioTest_HandlerDefaults( void* arg )
{

    ZLOG_DEBUG( "#############################################################" );
    ZLOG_DEBUG( "# zGpioTest_HandlerDefaults()" );
    ZLOG_DEBUG( "#############################################################" );

    // Create new GPIO handler and verify
    zGpio::Handler *myHandler = new zGpio::Handler;
    TEST_ISNOT_NULL( myHandler );
    TEST_IS_NULL( myHandler->GetPort( 1 ) );

    // Clean up
    delete (myHandler);
    return (0);
}


int
zGpioTest_SwitchDefaults( void* arg )
{

    ZLOG_DEBUG( "#############################################################" );
    ZLOG_DEBUG( "# zGpioTest_SwitchDefaults()" );
    ZLOG_DEBUG( "#############################################################" );

    // Create new Switch and verify
    zGpio::Switch* mySwitch = new zGpio::Switch( zSwitch::Switch::STATE_OFF );
    TEST_ISNOT_NULL( mySwitch );
    TEST_FALSE( mySwitch->IsOn() );
    TEST_TRUE( mySwitch->IsOff() );
    TEST_IS_NULL( mySwitch->GetPort( 1 ) );

    // Clean up
    delete (mySwitch);
    return (0);
}
