#include "zMessageTest.h"

using namespace Test;
using namespace zUtils;

int
zMessageTest_Defaults(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = new zMessage::Message();
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_NONE);
  TEST_EQ(myMessage->GetId(), "");
  TEST_EQ(myMessage->GetTo().GetId(), "");
  TEST_EQ(myMessage->GetFrom().GetId(), "");

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_HelloDefaults(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::Factory::Create(zMessage::Message::TYPE_HELLO);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_HELLO);
  TEST_EQ(myMessage->GetId(), "");
  TEST_EQ(myMessage->GetTo().GetId(), "");
  TEST_EQ(myMessage->GetFrom().GetId(), "");

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_AckDefaults(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::Factory::Create(zMessage::Message::TYPE_ACK);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_ACK);
  TEST_EQ(myMessage->GetId(), "");
  TEST_EQ(myMessage->GetTo().GetId(), "");
  TEST_EQ(myMessage->GetFrom().GetId(), "");

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_ByeDefaults(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::Factory::Create(zMessage::Message::TYPE_BYE);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_BYE);
  TEST_EQ(myMessage->GetId(), "");
  TEST_EQ(myMessage->GetTo().GetId(), "");
  TEST_EQ(myMessage->GetFrom().GetId(), "");

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_CfgDefaults(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::Factory::Create(zMessage::Message::TYPE_CMD);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_CMD);
  TEST_EQ(myMessage->GetId(), "");
  TEST_EQ(myMessage->GetTo().GetId(), "");
  TEST_EQ(myMessage->GetFrom().GetId(), "");

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}
int
zMessageTest_CmdDefaults(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::Factory::Create(zMessage::Message::TYPE_CMD);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_CMD);
  TEST_EQ(myMessage->GetId(), "");
  TEST_EQ(myMessage->GetTo().GetId(), "");
  TEST_EQ(myMessage->GetFrom().GetId(), "");

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

