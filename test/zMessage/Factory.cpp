/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "zMessageTest.h"

using namespace Test;
using namespace zUtils;

int
zMessageTest_FactoryError(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zMessageTest_FactoryError()");
  ZLOG_DEBUG("#############################################################");

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::MessageFactory::Create(zMessage::Message::TYPE_ERR);
  TEST_IS_NULL(myMessage);

  // Return success
  return (0);

}

int
zMessageTest_FactoryAuth(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zMessageTest_FactoryAuth()");
  ZLOG_DEBUG("#############################################################");

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::MessageFactory::Create(zMessage::Message::TYPE_AUTH);
  TEST_ISNOT_NULL(myMessage);
  TEST_NEQ(std::string(""), myMessage->GetId());
  TEST_EQ_MSG(myMessage->GetType(), zMessage::Message::TYPE_AUTH, myMessage->GetJson());
  TEST_EQ(std::string(""), myMessage->GetDst());
  TEST_EQ(std::string(""), myMessage->GetSrc());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_FactoryHello(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zMessageTest_FactoryHello()");
  ZLOG_DEBUG("#############################################################");

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::MessageFactory::Create(zMessage::Message::TYPE_HELLO);
  TEST_ISNOT_NULL(myMessage);
  TEST_NEQ(std::string(""), myMessage->GetId());
  TEST_EQ_MSG(myMessage->GetType(), zMessage::Message::TYPE_HELLO, myMessage->GetJson());
  TEST_EQ(std::string(""), myMessage->GetDst());
  TEST_EQ(std::string(""), myMessage->GetSrc());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_FactoryAck(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zMessageTest_FactoryAck()");
  ZLOG_DEBUG("#############################################################");

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::MessageFactory::Create(zMessage::Message::TYPE_ACK);
  TEST_ISNOT_NULL(myMessage);
  TEST_NEQ(std::string(""), myMessage->GetId());
  TEST_EQ_MSG(myMessage->GetType(), zMessage::Message::TYPE_ACK, myMessage->GetJson());
  TEST_EQ(std::string(""), myMessage->GetDst());
  TEST_EQ(std::string(""), myMessage->GetSrc());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_FactoryBye(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zMessageTest_FactoryBye()");
  ZLOG_DEBUG("#############################################################");

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::MessageFactory::Create(zMessage::Message::TYPE_BYE);
  TEST_ISNOT_NULL(myMessage);
  TEST_NEQ(std::string(""), myMessage->GetId());
  TEST_EQ_MSG(myMessage->GetType(), zMessage::Message::TYPE_BYE, myMessage->GetJson());
  TEST_EQ(std::string(""), myMessage->GetDst());
  TEST_EQ(std::string(""), myMessage->GetSrc());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_FactoryCfg(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zMessageTest_FactoryCfg()");
  ZLOG_DEBUG("#############################################################");

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::MessageFactory::Create(zMessage::Message::TYPE_CFG);
  TEST_ISNOT_NULL(myMessage);
  TEST_NEQ(std::string(""), myMessage->GetId());
  TEST_EQ_MSG(myMessage->GetType(), zMessage::Message::TYPE_CFG, myMessage->GetJson());
  TEST_EQ(std::string(""), myMessage->GetDst());
  TEST_EQ(std::string(""), myMessage->GetSrc());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_FactoryCmd(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zMessageTest_FactoryCmd()");
  ZLOG_DEBUG("#############################################################");

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::MessageFactory::Create(zMessage::Message::TYPE_CMD);
  TEST_ISNOT_NULL(myMessage);
  TEST_NEQ(std::string(""), myMessage->GetId());
  TEST_EQ_MSG(myMessage->GetType(), zMessage::Message::TYPE_CMD, myMessage->GetJson());
  TEST_EQ(std::string(""), myMessage->GetDst());
  TEST_EQ(std::string(""), myMessage->GetSrc());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_FactoryData(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zMessageTest_FactoryData()");
  ZLOG_DEBUG("#############################################################");

  // Create new node and validate
  zMessage::Message *myMessage = zMessage::MessageFactory::Create(zMessage::Message::TYPE_DATA);
  TEST_ISNOT_NULL(myMessage);
  TEST_NEQ(std::string(""), myMessage->GetId());
  TEST_EQ_MSG(myMessage->GetType(), zMessage::Message::TYPE_DATA, myMessage->GetJson());
  TEST_EQ(std::string(""), myMessage->GetDst());
  TEST_EQ(std::string(""), myMessage->GetSrc());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

