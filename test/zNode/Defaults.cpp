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

#include "zNodeTest.h"

using namespace Test;
using namespace zUtils;

int
zNodeTest_NodeDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zNodeTest_NodeDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new node from zData object
  zData::Data myData;
  zNode::Node MyNode1(myData);
  TEST_EQ(std::string(""), MyNode1.GetName());
  TEST_EQ(std::string(""), MyNode1.GetId());
  TEST_EQ(std::string(""), MyNode1.GetAddress());

  // Create new node and validate
  zNode::Node MyNode2;
  TEST_EQ(std::string(""), MyNode2.GetName());
  TEST_NEQ(std::string(""), MyNode2.GetId());
  TEST_EQ(std::string(""), MyNode2.GetAddress());

  // Return success
  return (0);

}

int
zNodeTest_TableDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zNodeTest_TableDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new node table and validate
  zNode::Table *MyTable = new zNode::Table;

  // Cleanup
  delete (MyTable);

  // Return success
  return (0);

}

int
zNodeTest_MessageDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zNodeTest_MessageDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new node and validate
  zNode::Node MyNode;
  TEST_EQ(std::string(""), MyNode.GetName());
  TEST_NEQ(std::string(""), MyNode.GetId());
  TEST_EQ(std::string(""), MyNode.GetAddress());

  // Initialize node
  TEST_TRUE(MyNode.SetName("zNodeTest"));

  // Create new node message and validate
  zNode::Message *MyMessage = new zNode::Message(zMessage::Message::TYPE_NODE, MyNode);
  TEST_EQ(zMessage::Message::TYPE_NODE, MyMessage->GetType());
  TEST_TRUE(MyNode == MyMessage->GetNode());

  // Cleanup
  delete (MyMessage);

  // Return success
  return (0);

}
