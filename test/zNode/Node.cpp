#include "zNodeTest.h"

using namespace Test;
using namespace zUtils;

int
zNodeTest_NodeGetSet(void* arg_)
{

  std::string exp;

  // Create new node and validate
  zNode::Node *MyNode = new zNode::Node;
  TEST_EQ( MyNode->GetType(), std::string( "" ));
  TEST_EQ( MyNode->GetId(), std::string( "" ));

  // Test setting type
  exp = "";
  TEST_EQ( MyNode->GetType(), exp);
  exp = "Type";
  MyNode->SetType(exp);
  TEST_EQ( MyNode->GetType(), exp);

  // Test setting identifier
  exp = "";
  TEST_EQ( MyNode->GetId(), exp);
  exp = "Identifier";
  MyNode->SetId(exp);
  TEST_EQ( MyNode->GetId(), exp);

  // Cleanup
  delete (MyNode);

  // Return success
  return (0);
}

