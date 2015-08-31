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
zGpioTest_HandlerAddRemove( void* arg )
{

    ZLOG_DEBUG( "#############################################################" );
    ZLOG_DEBUG( "# zGpioTest_HandlerAddRemove()" );
    ZLOG_DEBUG( "#############################################################" );

    // Create new GPIO port test configuration and verify
    TestPortConf *myConf = new TestPortConf(1);
    TEST_ISNOT_NULL(myConf);

    // Create new GPIO port and verify
    zGpio::Port *myPort = new zGpio::Port( *myConf );
    TEST_ISNOT_NULL( myPort );
    TEST_EQ( zGpio::Port::DIR_DEF, myPort->GetDirection() );
    TEST_EQ( zGpio::Port::STATE_ERR, myPort->GetState() );
    TEST_EQ( zGpio::Port::EDGE_DEF, myPort->GetEdge() );

    // Create new GPIO handler and verify
    zGpio::Handler* myHandler = new zGpio::Handler;
    TEST_ISNOT_NULL( myHandler );

    // Try to get nonexistent GPIO; should fail
    zGpio::Port* expGpioPort = myHandler->GetPort( 1 );
    TEST_IS_NULL( expGpioPort );

    // Add GpioPort and verify
    TEST_TRUE( myHandler->AddPort( myPort ) );
    expGpioPort = myHandler->GetPort( 1 );
    TEST_EQ( (int )expGpioPort->GetId(), (int )myPort->GetId() );

    // Remove GpioPort and verify
    TEST_TRUE( myHandler->RemovePort( myPort ) );
    expGpioPort = myHandler->GetPort( 1 );
    TEST_IS_NULL( expGpioPort );

    // Clean up
    delete (myHandler);
    delete (myPort);
    delete (myConf);
    return (0);
}

int
zGpioTest_HandlerOnOff( void* arg )
{

    ZLOG_DEBUG( "#############################################################" );
    ZLOG_DEBUG( "# zGpioTest_HandlerOnOff()" );
    ZLOG_DEBUG( "#############################################################" );

    // Create new GPIO port test configuration and verify
    TestPortConf *myConf1 = new TestPortConf(1);
    TEST_ISNOT_NULL(myConf1);

    // Create new GPIO port and verify
    zGpio::Port *myPort1 = new zGpio::Port( *myConf1 );
    TEST_ISNOT_NULL( myPort1 );
    TEST_EQ( zGpio::Port::DIR_DEF, myPort1->GetDirection() );
    TEST_EQ( zGpio::Port::STATE_ERR, myPort1->GetState() );
    TEST_EQ( zGpio::Port::EDGE_DEF, myPort1->GetEdge() );

    // Create new GPIO port test configuration and verify
    TestPortConf *myConf2 = new TestPortConf(2);
    TEST_ISNOT_NULL(myConf2);

    // Create new GPIO port and verify
    zGpio::Port *myPort2 = new zGpio::Port( *myConf2 );
    TEST_ISNOT_NULL( myPort2 );
    TEST_EQ( zGpio::Port::DIR_DEF, myPort2->GetDirection() );
    TEST_EQ( zGpio::Port::STATE_ERR, myPort2->GetState() );
    TEST_EQ( zGpio::Port::EDGE_DEF, myPort2->GetEdge() );

    // Configure both ports to be outputs
    TEST_TRUE(myPort1->SetDirection(zGpio::Port::DIR_OUT));
    TEST_TRUE(myPort2->SetDirection(zGpio::Port::DIR_OUT));

    // Create new GPIO handler and verify
    zGpio::Handler* myHandler = new zGpio::Handler;
    TEST_ISNOT_NULL( myHandler );

    // Add GpioPort and verify
    TEST_TRUE( myHandler->AddPort( myPort1 ) );
    zGpio::Port* expGpioPort = myHandler->GetPort( 1 );
    TEST_EQ( (int )expGpioPort->GetId(), (int )myPort1->GetId() );
    TEST_EQ( zGpio::Port::STATE_INACTIVE, myPort1->GetState() );
    TEST_EQ( zGpio::Port::DIR_OUT, myPort1->GetDirection() );

    // Add GpioPort and verify
    TEST_TRUE( myHandler->AddPort( myPort2 ) );
    expGpioPort = myHandler->GetPort( 2 );
    TEST_EQ( (int )expGpioPort->GetId(), (int )myPort2->GetId() );
    TEST_EQ( zGpio::Port::STATE_INACTIVE, myPort2->GetState() );
    TEST_EQ( zGpio::Port::DIR_OUT, myPort2->GetDirection() );

    // Set state to inactive
    TEST_TRUE( myHandler->SetState( zGpio::Port::STATE_INACTIVE ) );
    TEST_EQ( zGpio::Port::STATE_INACTIVE, myPort1->GetState() );
    TEST_EQ( zGpio::Port::STATE_INACTIVE, myPort2->GetState() );
    TEST_EQ( zGpio::Port::STATE_INACTIVE, myHandler->GetState() );

    // Set state to active
    TEST_TRUE( myHandler->SetState( zGpio::Port::STATE_ACTIVE ) );
    TEST_EQ( zGpio::Port::STATE_ACTIVE, myPort1->GetState() );
    TEST_EQ( zGpio::Port::STATE_ACTIVE, myPort2->GetState() );
    TEST_EQ( zGpio::Port::STATE_ACTIVE, myHandler->GetState() );

    // Set state to inactive
    TEST_TRUE( myHandler->SetState( zGpio::Port::STATE_INACTIVE ) );
    TEST_EQ( zGpio::Port::STATE_INACTIVE, myPort1->GetState() );
    TEST_EQ( zGpio::Port::STATE_INACTIVE, myPort2->GetState() );
    TEST_EQ( zGpio::Port::STATE_INACTIVE, myHandler->GetState() );

    // Remove GpioPort and verify
    TEST_TRUE( myHandler->RemovePort( myPort1 ) );
    expGpioPort = myHandler->GetPort( 1 );
    TEST_IS_NULL( expGpioPort );

    // Clean up
    delete (myHandler);
    delete (myPort2);
    delete (myPort1);
    delete (myConf2);
    delete (myConf1);
    return (0);
}

