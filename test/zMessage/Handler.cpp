#include "zMessageTest.h"

#include <zutils/zInet.h>

using namespace Test;
using namespace zUtils;

int zMessageTest_MessageHandler(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zMessageTest_MessageHandler()");
    ZLOG_DEBUG("#############################################################");

    // Setup network socket
    zSocket::InetAddress myAddr("lo");
    TEST_TRUE(myAddr.SetPort("8888"));
    zSocket::InetSocket *mySock = new zSocket::InetSocket(myAddr);
    TEST_ISNOT_NULL(mySock);

    // Create new test handler and validate
    TestHandler *myHandler = new TestHandler;
    TEST_ISNOT_NULL(myHandler);

    // Startup the message handler
    TEST_TRUE(myHandler->Bind(mySock));
    TEST_TRUE(myHandler->StartListener(500));

    // Broadcast hello message to self
    zMessage::Message *helloMsg = zMessage::Factory::Create(zMessage::Message::TYPE_HELLO);
    TEST_ISNOT_NULL(helloMsg);
    TEST_TRUE(helloMsg->SetId("helloMsg"));
    TEST_TRUE(helloMsg->SetTo(""));
    TEST_TRUE(helloMsg->SetFrom(myAddr.GetAddress()));
    ZLOG_DEBUG(helloMsg->GetJson());
    TEST_TRUE(myHandler->Broadcast(*helloMsg));
    delete (helloMsg);

    // Wait for message to arrive
    TEST_TRUE(myHandler->WaitForMessage(100000));

    // Wait for Ack to arrive
    TEST_TRUE(myHandler->WaitForMessage(100000));

    // No more messages should be waiting
    TEST_FALSE(myHandler->WaitForMessage(10000));

    // Acknowledge hello message
    zMessage::Message *ackMsg = zMessage::Factory::Create(zMessage::Message::TYPE_ACK);
    TEST_ISNOT_NULL(ackMsg);
    TEST_TRUE(ackMsg->SetId("ackMsg"));
    TEST_TRUE(ackMsg->SetTo(myAddr.GetAddress()));
    TEST_TRUE(ackMsg->SetFrom(myAddr.GetAddress()));
    ZLOG_DEBUG(ackMsg->GetJson());
    TEST_TRUE(myHandler->Send(*ackMsg));
    delete (ackMsg);

    // Wait for message to arrive
    TEST_TRUE(myHandler->WaitForMessage(100000));

    // No more messages should be waiting
    TEST_FALSE(myHandler->WaitForMessage(10000));

    // Send bye message to self
    zMessage::Message *byeMsg = zMessage::Factory::Create(zMessage::Message::TYPE_BYE);
    TEST_ISNOT_NULL(byeMsg);
    TEST_TRUE(byeMsg->SetId("byeMsg"));
    TEST_TRUE(byeMsg->SetTo(myAddr.GetAddress()));
    TEST_TRUE(byeMsg->SetFrom(myAddr.GetAddress()));
    ZLOG_DEBUG(byeMsg->GetJson());
    TEST_TRUE(myHandler->Send(*byeMsg));
    delete (byeMsg);

    // Wait for message to arrive
    TEST_TRUE(myHandler->WaitForMessage(100000));

    // No more messages should be waiting
    TEST_FALSE(myHandler->WaitForMessage(10000));

    // Clean up
    myHandler->StopListener();
    myHandler->Close();
    delete (mySock);
    delete (myHandler);

    // Return success
    return (0);

}

