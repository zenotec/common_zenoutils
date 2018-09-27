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
zNodeTest_NodeTableAddRemove(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zNodeTest_NodeTableAddRemove()");
  ZLOG_DEBUG("#############################################################");

  // Create new node and validate
  zNode::Node *MyNode = new zNode::Node;
  TEST_EQ(MyNode->GetName(), std::string(""));
  TEST_NEQ(MyNode->GetId(), std::string(""));

  // Initialize node
  MyNode->SetName("TestNode");
  TEST_EQ(MyNode->GetName(), "TestNode");
  TEST_TRUE(MyNode->SetId("abcdef56789"));
  TEST_EQ(MyNode->GetId(), "abcdef56789");
  TEST_TRUE(MyNode->SetAddress("1.2.3.4:5"));
  TEST_EQ(MyNode->GetAddress(), "1.2.3.4:5");

  // Create new node table test observer and validate
  TestObserver *MyObsvr = new TestObserver;
  TEST_IS_ZERO(MyObsvr->GetCount());

  // Create new node table and validate
  zNode::Table *MyNodeTable = new zNode::Table(1000);
  TEST_IS_NULL(MyNodeTable->Find(MyNode->GetId()));

  // Register observer
  MyNodeTable->Register(MyObsvr);
  TEST_IS_ZERO(MyObsvr->GetCount());

  // Add node to table and validate
  TEST_TRUE(MyNodeTable->Add(*MyNode));
  TEST_ISNOT_NULL(MyNodeTable->Find(MyNode->GetId()));
  TEST_EQ(1, MyObsvr->GetCount());

  // Add node to table a second time and validate (should fail)
  TEST_FALSE(MyNodeTable->Add(*MyNode));
  TEST_ISNOT_NULL(MyNodeTable->Find(MyNode->GetId()));
  TEST_EQ(1, MyObsvr->GetCount());

  // Remove node
  TEST_TRUE(MyNodeTable->Remove(*MyNode));
  TEST_IS_NULL(MyNodeTable->Find(MyNode->GetId()));
  TEST_EQ(2, MyObsvr->GetCount());

  // Add node back to table and validate
  TEST_TRUE(MyNodeTable->Add(*MyNode));
  TEST_ISNOT_NULL(MyNodeTable->Find(MyNode->GetId()));
  TEST_EQ(3, MyObsvr->GetCount());

  // Remove node
  TEST_TRUE(MyNodeTable->Remove(*MyNode));
  TEST_IS_NULL(MyNodeTable->Find(MyNode->GetId()));
  TEST_EQ(4, MyObsvr->GetCount());

  // Cleanup
  MyNodeTable->Unregister(MyObsvr);
  delete (MyObsvr);
  delete (MyNodeTable);
  delete (MyNode);

  // Return success
  return (0);
}

int
zNodeTest_NodeTableExpire(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zNodeTest_NodeTableExpire()");
  ZLOG_DEBUG("#############################################################");

  // Create new node and validate
  zNode::Node *MyNode = new zNode::Node;
  TEST_EQ(MyNode->GetName(), std::string(""));
  TEST_NEQ(MyNode->GetId(), std::string(""));

  // Initialize node
  MyNode->SetName("TestNode");
  TEST_EQ(MyNode->GetName(), "TestNode");
  TEST_TRUE(MyNode->SetId("abcdef56789"));
  TEST_EQ(MyNode->GetId(), "abcdef56789");
  TEST_TRUE(MyNode->SetAddress("1.2.3.4:5"));
  TEST_EQ(MyNode->GetAddress(), "1.2.3.4:5");
  TEST_TRUE(MyNode->SetState(zNode::Node::STATE_ONLINE));
  TEST_EQ(zNode::Node::STATE_ONLINE, MyNode->GetState());

  // Create new node table test observer and validate
  TestObserver *MyObsvr = new TestObserver;
  TEST_IS_ZERO(MyObsvr->GetCount());

  // Create new node table and validate
  zNode::Table *MyNodeTable = new zNode::Table(100);
  TEST_IS_NULL(MyNodeTable->Find(MyNode->GetId()));

  // Register observer
  MyNodeTable->Register(MyObsvr);
  TEST_IS_ZERO(MyObsvr->GetCount());

  // Add node to table and validate
  TEST_TRUE(MyNodeTable->Add(*MyNode));
  TEST_ISNOT_NULL(MyNodeTable->Find(MyNode->GetId()));
  TEST_EQ(1, MyObsvr->GetCount());

  // Wait for node to retire
  sleep(1);

  // Remove node
  TEST_FALSE(MyNodeTable->Remove(*MyNode));
  TEST_IS_NULL(MyNodeTable->Find(MyNode->GetId()));
  TEST_EQ(6, MyObsvr->GetCount());

  // Cleanup
  MyNodeTable->Unregister(MyObsvr);
  delete (MyObsvr);
  delete (MyNodeTable);
  delete (MyNode);

  // Return success
  return (0);
}
