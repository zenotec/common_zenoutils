#include "zNodeTest.h"

using namespace Test;
using namespace zUtils;

int
zNodeTest_NodeGetSet(void* arg_)
{

  std::string exp;

  // Create new node and validate
  zNode::Node *myNode = new zNode::Node;
  TEST_EQ(std::string(""), myNode->GetType());
  TEST_NEQ(std::string(""), myNode->GetId());

  // Test setting type
  exp = "";
  TEST_EQ( myNode->GetType(), exp);
  exp = "Type";
  TEST_TRUE(myNode->SetType(exp));
  TEST_EQ( myNode->GetType(), exp);

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

