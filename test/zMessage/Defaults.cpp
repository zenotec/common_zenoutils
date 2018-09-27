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
zMessageTest_MessageDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zMessageTest_MessageDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new node and validate
  zMessage::Message *myMessage = new zMessage::Message;
  TEST_ISNOT_NULL(myMessage);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_NONE);
  TEST_EQ(std::string(""), myMessage->GetId());
  TEST_EQ(std::string(""), myMessage->GetDst());
  TEST_EQ(std::string(""), myMessage->GetSrc());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}

int
zMessageTest_MessageSocketDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zMessageTest_MessageSocketDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new message socket and validate
  zMessage::MessageSocket *MyMsgSock = new zMessage::MessageSocket;
  TEST_ISNOT_NULL(MyMsgSock);

  // Clean up
  delete (MyMsgSock);

  // Return success
  return (0);
}
