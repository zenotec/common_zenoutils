#include "zMessageTest.h"

using namespace Test;
using namespace zUtils;

int
zMessageTest_FactoryError(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::Factory::Create(zMessage::Message::TYPE_ERR);
  TEST_IS_NULL(myMessage);

  // Return success
  return (0);

}

int
zMessageTest_FactoryAuth(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::Factory::Create(zMessage::Message::TYPE_AUTH);
  TEST_ISNOT_NULL(myMessage);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_AUTH);
  TEST_NEQ(std::string(""), myMessage->GetId());
  TEST_NEQ(std::string(""), myMessage->GetTo().GetId());
  TEST_EQ(std::string(""), myMessage->GetTo().GetName());
  TEST_NEQ(std::string(""), myMessage->GetFrom().GetId());
  TEST_EQ(std::string(""), myMessage->GetFrom().GetName());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_FactoryHello(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::Factory::Create(zMessage::Message::TYPE_HELLO);
  TEST_ISNOT_NULL(myMessage);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_HELLO);
  TEST_NEQ(std::string(""), myMessage->GetId());
  TEST_NEQ(std::string(""), myMessage->GetTo().GetId());
  TEST_EQ(std::string(""), myMessage->GetTo().GetName());
  TEST_NEQ(std::string(""), myMessage->GetFrom().GetId());
  TEST_EQ(std::string(""), myMessage->GetFrom().GetName());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_FactoryAck(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::Factory::Create(zMessage::Message::TYPE_ACK);
  TEST_ISNOT_NULL(myMessage);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_ACK);
  TEST_NEQ(std::string(""), myMessage->GetId());
  TEST_NEQ(std::string(""), myMessage->GetTo().GetId());
  TEST_EQ(std::string(""), myMessage->GetTo().GetName());
  TEST_NEQ(std::string(""), myMessage->GetFrom().GetId());
  TEST_EQ(std::string(""), myMessage->GetFrom().GetName());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_FactoryBye(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::Factory::Create(zMessage::Message::TYPE_BYE);
  TEST_ISNOT_NULL(myMessage);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_BYE);
  TEST_NEQ(std::string(""), myMessage->GetId());
  TEST_NEQ(std::string(""), myMessage->GetTo().GetId());
  TEST_EQ(std::string(""), myMessage->GetTo().GetName());
  TEST_NEQ(std::string(""), myMessage->GetFrom().GetId());
  TEST_EQ(std::string(""), myMessage->GetFrom().GetName());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_FactoryCfg(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::Factory::Create(zMessage::Message::TYPE_CFG);
  TEST_ISNOT_NULL(myMessage);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_CFG);
  TEST_NEQ(std::string(""), myMessage->GetId());
  TEST_NEQ(std::string(""), myMessage->GetTo().GetId());
  TEST_EQ(std::string(""), myMessage->GetTo().GetName());
  TEST_NEQ(std::string(""), myMessage->GetFrom().GetId());
  TEST_EQ(std::string(""), myMessage->GetFrom().GetName());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_FactoryCmd(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::Factory::Create(zMessage::Message::TYPE_CMD);
  TEST_ISNOT_NULL(myMessage);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_CMD);
  TEST_NEQ(std::string(""), myMessage->GetId());
  TEST_NEQ(std::string(""), myMessage->GetTo().GetId());
  TEST_EQ(std::string(""), myMessage->GetTo().GetName());
  TEST_NEQ(std::string(""), myMessage->GetFrom().GetId());
  TEST_EQ(std::string(""), myMessage->GetFrom().GetName());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_FactoryData(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::Factory::Create(zMessage::Message::TYPE_DATA);
  TEST_ISNOT_NULL(myMessage);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_DATA);
  TEST_NEQ(std::string(""), myMessage->GetId());
  TEST_NEQ(std::string(""), myMessage->GetTo().GetId());
  TEST_EQ(std::string(""), myMessage->GetTo().GetName());
  TEST_NEQ(std::string(""), myMessage->GetFrom().GetId());
  TEST_EQ(std::string(""), myMessage->GetFrom().GetName());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

