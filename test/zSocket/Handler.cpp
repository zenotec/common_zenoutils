/*
 * Handler.cpp
 *
 *  Created on: Jan 11, 2015
 *      Author: kmahoney
 */

#include <unistd.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int zSocketTest_HandlerRegister(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_HandlerRegister()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    zSocket::Address myAddr;
    TEST_EQ(zSocket::Address::TYPE_NONE, myAddr.GetType());
    TEST_EQ(std::string(""), myAddr.GetAddress());

    // Create new observer and validate
    TestObserver myObserver;

    // Create new socket and validate
    TestSocket mySocket(&myAddr);

    // Create new handler and validate
    zSocket::Handler myHandler;

    // Register observer
    TEST_TRUE(myHandler.Register(&myObserver));

    // Pause to let listener run a moment
    usleep(100000);

    // Clean up
    myHandler.Unregister(&myObserver);

    // Return success
    return (0);

}

int zSocketTest_HandlerStartStop(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_HandlerStartStop()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    zSocket::Address myAddress(zSocket::Address::TYPE_LOOP, "lo");
    TEST_EQ(std::string("lo"), myAddress.GetAddress());

    // Create new observer and validate
    TestObserver *myObserver = new TestObserver;
    TEST_ISNOT_NULL(myObserver);

    // Create new socket and validate
    TestSocket *mySocket = new TestSocket(&myAddress);
    TEST_ISNOT_NULL(mySocket);

    // Create new handler and validate
    zSocket::Handler *myHandler = new zSocket::Handler;
    TEST_ISNOT_NULL(myHandler);

    // Register observer
    TEST_TRUE(myHandler->Register(myObserver));

    // Bind socket and validate
    TEST_TRUE(myHandler->Bind(mySocket));

    // Start listener
    TEST_TRUE(myHandler->StartListener());

    // Pause to let listener run a moment
    usleep(100000);

    // Stop listener
    TEST_TRUE(myHandler->StopListener());

    // Clean up
    myHandler->Close();
    delete (mySocket);
    myHandler->Unregister(myObserver);
    delete (myObserver);
    delete (myHandler);

    // Return success
    return (0);

}

int zSocketTest_HandlerSendRecv(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_HandlerSendRecv()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    zSocket::Address myAddress(zSocket::Address::TYPE_LOOP, "lo");
    TEST_EQ(std::string("lo"), myAddress.GetAddress());

    // Create new observer and validate
    TestObserver *myObserver = new TestObserver;
    TEST_ISNOT_NULL(myObserver);

    // Create new socket and validate
    TestSocket *mySocket = new TestSocket(&myAddress);
    TEST_ISNOT_NULL(mySocket);

    // Create new handler and validate
    zSocket::Handler *myHandler = new zSocket::Handler;
    TEST_ISNOT_NULL(myHandler);

    // Register observer
    TEST_TRUE(myHandler->Register(myObserver));

    // Bind socket and validate
    TEST_TRUE(myHandler->Bind(mySocket));

    // Start listener
    TEST_TRUE(myHandler->StartListener());

    // Send string and validate
    std::string expStr = "zSocketTest_HandlerSendRecv";
    TEST_TRUE(myHandler->Send(&myAddress, expStr));

    // Wait for socket observer to be notified
    zSocket::Buffer *sb = myObserver->WaitForPacket(1000);
    TEST_ISNOT_NULL(sb);
    delete (sb);

    // Stop listener
    TEST_TRUE(myHandler->StopListener());

    // Clean up
    myHandler->Close();
    delete (mySocket);
    myHandler->Unregister(myObserver);
    delete (myObserver);
    delete (myHandler);

    // Return success
    return (0);

}

int zSocketTest_HandlerBroadcast(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_HandlerBroadcast()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    zSocket::Address myAddress(zSocket::Address::TYPE_LOOP, "lo");
    TEST_EQ(std::string("lo"), myAddress.GetAddress());

    // Create new observer and validate
    TestObserver *myObserver = new TestObserver;
    TEST_ISNOT_NULL(myObserver);

    // Create new socket and validate
    TestSocket *mySocket = new TestSocket(&myAddress);
    TEST_ISNOT_NULL(mySocket);

    // Create new handler and validate
    zSocket::Handler *myHandler = new zSocket::Handler;
    TEST_ISNOT_NULL(myHandler);

    // Register observer
    TEST_TRUE(myHandler->Register(myObserver));

    // Bind socket and validate
    TEST_TRUE(myHandler->Bind(mySocket));

    // Start listener
    TEST_TRUE(myHandler->StartListener());

    // Send string and validate
    std::string expStr = "zSocketTest_HandlerSendRecv";
    TEST_TRUE(myHandler->Broadcast(expStr));

    // Wait for socket observer to be notified
    zSocket::Buffer *sb = myObserver->WaitForPacket(1000);
    TEST_ISNOT_NULL(sb);
    delete (sb);

    // Stop listener
    TEST_TRUE(myHandler->StopListener());

    // Clean up
    myHandler->Close();
    delete (mySocket);
    myHandler->Unregister(myObserver);
    delete (myObserver);
    delete (myHandler);

    // Return success
    return (0);

}
