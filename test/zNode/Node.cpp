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
zNodeTest_NodeGetSet(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zNodeTest_NodeGetSet()");
  ZLOG_DEBUG("#############################################################");

  std::string exp;

  // Create new node and validate
  zNode::Node *myNode = new zNode::Node;
  TEST_EQ(std::string(""), myNode->GetName());
  TEST_NEQ(std::string(""), myNode->GetId());
  TEST_EQ(std::string(""), myNode->GetAddress());

  // Test setting name
  exp = "";
  TEST_EQ(myNode->GetName(), exp);
  exp = "Name";
  TEST_TRUE(myNode->SetName(exp));
  TEST_EQ(myNode->GetName(), exp);

  // Test setting identifier
  exp = "";
  TEST_NEQ(myNode->GetId(), exp);
  exp = "Identifier";
  TEST_TRUE(myNode->SetId(exp));
  TEST_EQ(myNode->GetId(), exp);

  // Test setting address
  exp = "";
  TEST_EQ(myNode->GetAddress(), exp);
  exp = "Address";
  TEST_TRUE(myNode->SetAddress(exp));
  TEST_EQ(myNode->GetAddress(), exp);

  // Cleanup
  delete (myNode);

  // Return success
  return (0);
}

int
zNodeTest_NodeCopy(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zNodeTest_NodeCopy()");
  ZLOG_DEBUG("#############################################################");

  std::string exp;

  // Create new node and validate
  zNode::Node *myNode1 = new zNode::Node;
  TEST_EQ(std::string(""), myNode1->GetName());
  TEST_NEQ(std::string(""), myNode1->GetId());
  TEST_EQ(std::string(""), myNode1->GetAddress());

  // Create second new node from first and validate
  zNode::Node *myNode2 = new zNode::Node(*myNode1);
  TEST_EQ(std::string(""), myNode2->GetName());
  TEST_NEQ(std::string(""), myNode2->GetId());
  TEST_EQ(std::string(""), myNode2->GetAddress());

  // Verify nodes are equal
  TEST_TRUE(*myNode1 == *myNode2);

  // Modify first node name and validate
  TEST_TRUE(myNode1->SetName("Name1"));

  // Verify nodes are NOT equal
  TEST_TRUE(*myNode1 != *myNode2);

  // Copy first node to second node
  *myNode2 = *myNode1;

  // Verify nodes are equal
  TEST_TRUE(*myNode1 == *myNode2);

  // Modify first node identifier and validate
  TEST_TRUE(myNode1->SetId("Id1"));

  // Verify nodes are NOT equal
  TEST_TRUE(*myNode1 != *myNode2);

  // Copy first node to second node
  *myNode2 = *myNode1;

  // Verify nodes are equal
  TEST_TRUE(*myNode1 == *myNode2);

  // Modify first node address and validate
  TEST_TRUE(myNode1->SetAddress("Address1"));

  // Verify nodes are NOT equal
  TEST_TRUE(*myNode1 != *myNode2);

  // Copy first node to second node
  *myNode2 = *myNode1;

  // Verify nodes are equal
  TEST_TRUE(*myNode1 == *myNode2);

  // Cleanup
  delete (myNode1);
  delete (myNode2);

  // Return success
  return (0);
}

