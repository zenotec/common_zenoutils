#include "zNodeTest.h"

using namespace Test;
using namespace zUtils;

int
zNodeTest_NodeGetSet(void* arg_)
{

  std::string exp;

  // Create new node and validate
  zNode::Node *myNode = new zNode::Node;
  TEST_EQ(std::string(""), myNode->GetName());
  TEST_NEQ(std::string(""), myNode->GetId());

  // Test setting type
  exp = "";
  TEST_EQ( myNode->GetName(), exp);
  exp = "Name";
  TEST_TRUE(myNode->SetName(exp));
  TEST_EQ( myNode->GetName(), exp);

  // Test setting identifier
  exp = "";
  TEST_NEQ( myNode->GetId(), exp);
  exp = "Identifier";
  TEST_TRUE(myNode->SetId(exp));
  TEST_EQ( myNode->GetId(), exp);

  // Cleanup
  delete (myNode);

  // Return success
  return (0);
}

int
zNodeTest_NodeCopy(void* arg_)
{

  std::string exp;

  // Create new node and validate
  zNode::Node *myNode1 = new zNode::Node;
  TEST_EQ(std::string(""), myNode1->GetName());
  TEST_NEQ(std::string(""), myNode1->GetId());

  // Create second new node from first and validate
  zNode::Node *myNode2 = new zNode::Node(*myNode1);
  TEST_EQ(std::string(""), myNode2->GetName());

  // Verify nodes are equal
  TEST_TRUE(*myNode1 == *myNode2);

  // Modify first node and validate
  TEST_TRUE(myNode1->SetName("Name1"));
  TEST_TRUE(myNode1->SetId("Id1"));

  // Verify nodes are NOT equal
  TEST_TRUE(*myNode1 != *myNode2);

  // Copy first node to second node
  *myNode2 = *myNode1;

  // Verify nodes are equal
  TEST_TRUE(*myNode1 == *myNode2);

  // Cleanup
  delete (myNode1);

  // Return success
  return (0);
}

