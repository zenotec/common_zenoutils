#include "zMessageTest.h"

#include <zutils/zInetSocket.h>

using namespace Test;
using namespace zUtils;

int
zMessageTest_Handler(void* arg_)
{

  // Setup local node
  zNode::Node self;
  self.SetName("TestNode");

  // Setup network socket
  zSocket::InetAddress myAddr("lo");
  TEST_TRUE(myAddr.SetPort(std::string("8888")));
  zSocket::InetSocket *mySock = new zSocket::InetSocket(myAddr);
  TEST_ISNOT_NULL(mySock);

  // Create new test handler and validate
  TestHandler *myHandler = new TestHandler;
  TEST_ISNOT_NULL(myHandler);

  // Startup the message handler
  TEST_TRUE(myHandler->Bind(mySock));
  TEST_TRUE(myHandler->StartListener(100));

  // Broadcast hello message to self
  zMessage::Message *helloMsg = zMessage::Factory::Create(zMessage::Message::TYPE_HELLO);
  TEST_ISNOT_NULL(helloMsg);
  TEST_TRUE(helloMsg->SetId("helloMsg"));
  TEST_TRUE(helloMsg->SetTo(self));
  TEST_TRUE(helloMsg->SetFrom(self));
  ZLOG_DEBUG(helloMsg->GetJson());
  TEST_TRUE(myHandler->Broadcast(*helloMsg));
  delete (helloMsg);

  // Wait for message to arrive
  TEST_TRUE(myHandler->WaitForMessage(100000));

  // Acknowledge hello message
  zMessage::Message *ackMsg = zMessage::Factory::Create(zMessage::Message::TYPE_ACK);
  TEST_ISNOT_NULL(ackMsg);
  TEST_TRUE(ackMsg->SetId("ackMsg"));
  TEST_TRUE(ackMsg->SetTo(self));
  TEST_TRUE(ackMsg->SetFrom(self));
  ZLOG_DEBUG(ackMsg->GetJson());
  TEST_TRUE(myHandler->Send(myAddr, *ackMsg));
  delete (ackMsg);

  // Wait for message to arrive
  TEST_TRUE(myHandler->WaitForMessage(100000));

  // Send bye message to self
  zMessage::Message *byeMsg = zMessage::Factory::Create(zMessage::Message::TYPE_BYE);
  TEST_ISNOT_NULL(byeMsg);
  TEST_TRUE(byeMsg->SetId("byeMsg"));
  TEST_TRUE(byeMsg->SetTo(self));
  TEST_TRUE(byeMsg->SetFrom(self));
  ZLOG_DEBUG(byeMsg->GetJson());
  TEST_TRUE(myHandler->Send(myAddr, *byeMsg));
  delete (byeMsg);

  // Wait for message to arrive
  TEST_TRUE(myHandler->WaitForMessage(100000));

  // Clean up
  myHandler->StopListener();
  myHandler->Close();
  delete (mySock);
  delete (myHandler);

  // Return success
  return (0);

}

