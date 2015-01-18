#include "zMessageTest.h"

using namespace Test;
using namespace zUtils;

int
zMessageTest_Defaults(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = new zMessage::Message;
  TEST_ISNOT_NULL(myMessage);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_NONE);
  TEST_EQ(std::string(""), myMessage->GetId());
  TEST_NEQ(std::string(""), myMessage->GetTo().GetId());
  TEST_EQ(std::string(""), myMessage->GetTo().GetType());
  TEST_NEQ(std::string(""), myMessage->GetFrom().GetId());
  TEST_EQ(std::string(""), myMessage->GetFrom().GetType());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_AuthDefaults(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::Factory::Create(zMessage::Message::TYPE_AUTH);
  TEST_ISNOT_NULL(myMessage);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_AUTH);
  TEST_EQ(std::string(""), myMessage->GetId());
  TEST_NEQ(std::string(""), myMessage->GetTo().GetId());
  TEST_EQ(std::string(""), myMessage->GetTo().GetType());
  TEST_NEQ(std::string(""), myMessage->GetFrom().GetId());
  TEST_EQ(std::string(""), myMessage->GetFrom().GetType());

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
  TEST_ISNOT_NULL(myMessage);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_HELLO);
  TEST_EQ(std::string(""), myMessage->GetId());
  TEST_NEQ(std::string(""), myMessage->GetTo().GetId());
  TEST_EQ(std::string(""), myMessage->GetTo().GetType());
  TEST_NEQ(std::string(""), myMessage->GetFrom().GetId());
  TEST_EQ(std::string(""), myMessage->GetFrom().GetType());

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
  TEST_ISNOT_NULL(myMessage);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_ACK);
  TEST_EQ(std::string(""), myMessage->GetId());
  TEST_NEQ(std::string(""), myMessage->GetTo().GetId());
  TEST_EQ(std::string(""), myMessage->GetTo().GetType());
  TEST_NEQ(std::string(""), myMessage->GetFrom().GetId());
  TEST_EQ(std::string(""), myMessage->GetFrom().GetType());

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
  TEST_ISNOT_NULL(myMessage);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_BYE);
  TEST_EQ(std::string(""), myMessage->GetId());
  TEST_NEQ(std::string(""), myMessage->GetTo().GetId());
  TEST_EQ(std::string(""), myMessage->GetTo().GetType());
  TEST_NEQ(std::string(""), myMessage->GetFrom().GetId());
  TEST_EQ(std::string(""), myMessage->GetFrom().GetType());

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
  TEST_ISNOT_NULL(myMessage);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_CMD);
  TEST_EQ(std::string(""), myMessage->GetId());
  TEST_NEQ(std::string(""), myMessage->GetTo().GetId());
  TEST_EQ(std::string(""), myMessage->GetTo().GetType());
  TEST_NEQ(std::string(""), myMessage->GetFrom().GetId());
  TEST_EQ(std::string(""), myMessage->GetFrom().GetType());

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
  TEST_ISNOT_NULL(myMessage);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_CMD);
  TEST_EQ(std::string(""), myMessage->GetId());
  TEST_NEQ(std::string(""), myMessage->GetTo().GetId());
  TEST_EQ(std::string(""), myMessage->GetTo().GetType());
  TEST_NEQ(std::string(""), myMessage->GetFrom().GetId());
  TEST_EQ(std::string(""), myMessage->GetFrom().GetType());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_DataDefaults(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::Factory::Create(zMessage::Message::TYPE_DATA);
  TEST_ISNOT_NULL(myMessage);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_DATA);
  TEST_EQ(std::string(""), myMessage->GetId());
  TEST_NEQ(std::string(""), myMessage->GetTo().GetId());
  TEST_EQ(std::string(""), myMessage->GetTo().GetType());
  TEST_NEQ(std::string(""), myMessage->GetFrom().GetId());
  TEST_EQ(std::string(""), myMessage->GetFrom().GetType());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

