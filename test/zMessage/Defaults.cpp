#include "zMessageTest.h"

using namespace Test;
using namespace zUtils;

int
zMessageTest_Defaults(void* arg_)
{

  // Create new node and validate
  zMessage::Message *MyMessage = new zMessage::Message();
  TEST_EQ(MyMessage->GetId(), "");
  TEST_EQ(MyMessage->GetType(), std::string(""));
  TEST_EQ(MyMessage->GetTo().GetId(), "");
  TEST_EQ(MyMessage->GetFrom().GetId(), "");

  // Cleanup
  delete (MyMessage);

  // Return success
  return (0);

}

int
zMessageTest_HelloDefaults(void* arg_)
{

  // Create new node and validate
  zMessage::Message *MyMessage = new zMessage::HelloMessage();
  TEST_EQ(MyMessage->GetId(), "");
  TEST_EQ(MyMessage->GetType(), zMessage::HelloMessage::TYPE);
  TEST_EQ(MyMessage->GetTo().GetId(), "");
  TEST_EQ(MyMessage->GetFrom().GetId(), "");

  // Cleanup
  delete (MyMessage);

  // Return success
  return (0);

}

int
zMessageTest_AckDefaults(void* arg_)
{

  // Create new node and validate
  zMessage::Message *MyMessage = new zMessage::AckMessage();
  TEST_EQ(MyMessage->GetId(), "");
  TEST_EQ(MyMessage->GetType(), zMessage::AckMessage::TYPE);
  TEST_EQ(MyMessage->GetTo().GetId(), "");
  TEST_EQ(MyMessage->GetFrom().GetId(), "");

  // Cleanup
  delete (MyMessage);

  // Return success
  return (0);

}

int
zMessageTest_ByeDefaults(void* arg_)
{

  // Create new node and validate
  zMessage::Message *MyMessage = new zMessage::ByeMessage();
  TEST_EQ(MyMessage->GetId(), "");
  TEST_EQ(MyMessage->GetType(), zMessage::ByeMessage::TYPE);
  TEST_EQ(MyMessage->GetTo().GetId(), "");
  TEST_EQ(MyMessage->GetFrom().GetId(), "");

  // Cleanup
  delete (MyMessage);

  // Return success
  return (0);

}

