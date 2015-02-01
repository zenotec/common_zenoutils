#include "zMessageTest.h"

#include <zutils/zInetSocket.h>

using namespace Test;
using namespace zUtils;

int
zMessageTest_Handler(void* arg_)
{

  // Setup local node
  zNode::Node self;
  self.SetType("nodeType");

  // Setup network socket
  zSocket::Address myAddr(zSocket::Address::TYPE_INET, std::string("127.0.0.1:8888"));
  zSocket::InetSocket *mySock = new zSocket::InetSocket(myAddr);
  TEST_ISNOT_NULL(mySock);

  // Create new test handler and validate
  TestHandler *myHandler = new TestHandler;
  TEST_ISNOT_NULL(myHandler);

  // Startup the message handler
  myHandler->Bind(mySock);
  myHandler->StartListener(100);

  // Send hello message to self
  zMessage::Message *helloMsg = zMessage::Factory::Create(zMessage::Message::TYPE_HELLO);
  TEST_ISNOT_NULL(helloMsg);
  TEST_TRUE(helloMsg->SetId("helloMsg"));
  TEST_TRUE(helloMsg->SetTo(self));
  TEST_TRUE(helloMsg->SetFrom(self));
  ZLOG_DEBUG(helloMsg->GetJson());
  TEST_EQ((int)helloMsg->GetJson().size(), (int)mySock->Send(myAddr, helloMsg->GetJson()));
  delete (helloMsg);

  // Wait for message to arrive
  TEST_TRUE(myHandler->WaitForMessage(100000));

  // Send hello message to self
  zMessage::Message *ackMsg = zMessage::Factory::Create(zMessage::Message::TYPE_ACK);
  TEST_ISNOT_NULL(ackMsg);
  TEST_TRUE(ackMsg->SetId("ackMsg"));
  TEST_TRUE(ackMsg->SetTo(self));
  TEST_TRUE(ackMsg->SetFrom(self));
  ZLOG_DEBUG(ackMsg->GetJson());
  TEST_EQ((int)ackMsg->GetJson().size(), (int)mySock->Send(myAddr, ackMsg->GetJson()));
  delete (ackMsg);

  // Wait for message to arrive
  TEST_TRUE(myHandler->WaitForMessage(100000));

  // Send hello message to self
  zMessage::Message *byeMsg = zMessage::Factory::Create(zMessage::Message::TYPE_BYE);
  TEST_ISNOT_NULL(byeMsg);
  TEST_TRUE(byeMsg->SetId("byeMsg"));
  TEST_TRUE(byeMsg->SetTo(self));
  TEST_TRUE(byeMsg->SetFrom(self));
  ZLOG_DEBUG(byeMsg->GetJson());
  TEST_EQ((int)byeMsg->GetJson().size(), (int)mySock->Send(myAddr, byeMsg->GetJson()));
  delete (byeMsg);

  // Wait for message to arrive
  TEST_TRUE(myHandler->WaitForMessage(100000));

  // Clean up
  myHandler->StopListener();
  myHandler->Close(mySock);
  delete (mySock);
  delete (myHandler);

  // Return success
  return (0);

}

