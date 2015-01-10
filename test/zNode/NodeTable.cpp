#include "zNodeTest.h"

using namespace Test;
using namespace zUtils;

int
zNodeTest_NodeTableGetSet(void* arg_)
{

  // Create new node and validate
  zNode::Node *MyNode = new zNode::Node;
  TEST_EQ( MyNode->GetType(), std::string(""));
  TEST_EQ( MyNode->GetId(), std::string(""));

  // Initialize node
  MyNode->SetType("TestNode");
  TEST_EQ( MyNode->GetType(), "TestNode");
  MyNode->SetId("abcdef56789");
  TEST_EQ( MyNode->GetId(), "abcdef56789");

  // Create new node table test observer and validate
  TestObserver *MyObsvr = new TestObserver;
  TEST_IS_ZERO(MyObsvr->GetCount());

  // Create new node table and validate
  zNode::NodeTable *MyNodeTable = new zNode::NodeTable;
  TEST_TRUE(MyNodeTable->SetConf(100, 300, 400, 500));
  TEST_IS_NULL(MyNodeTable->FindNode(MyNode->GetId()));

  // Register observer
  MyNodeTable->Register(MyObsvr);
  TEST_IS_ZERO(MyObsvr->GetCount());

  // Add node to table and validate
  TEST_TRUE(MyNodeTable->AddNode(*MyNode));
  TEST_ISNOT_NULL(MyNodeTable->FindNode(MyNode->GetId()));
  TEST_EQ(1, MyObsvr->GetCount());

  // Remove node
  TEST_TRUE(MyNodeTable->RemoveNode(MyNode->GetId()));
  TEST_IS_NULL(MyNodeTable->FindNode(MyNode->GetId()));
  TEST_EQ(2, MyObsvr->GetCount());

  // Add node back to table and validate
  TEST_TRUE(MyNodeTable->AddNode(*MyNode));
  TEST_ISNOT_NULL(MyNodeTable->FindNode(MyNode->GetId()));
  TEST_EQ(3, MyObsvr->GetCount());

  // Wait for node to retire
  sleep(1);
  TEST_IS_NULL(MyNodeTable->FindNode(MyNode->GetId()));
  TEST_EQ(6, MyObsvr->GetCount());

  // Cleanup
  MyNodeTable->Unregister(MyObsvr);
  delete (MyObsvr);
  delete (MyNodeTable);
  delete (MyNode);

  // Return success
  return (0);
}
