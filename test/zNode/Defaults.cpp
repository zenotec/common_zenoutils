#include "zNodeTest.h"

using namespace Test;
using namespace zUtils;

int
zNodeTest_NodeDefaults(void* arg_)
{

  // Create new node from zData object
  zData::Data myData;
  zNode::Node myNode1(myData);
  TEST_EQ(std::string(""), myNode1.GetType());
  TEST_EQ(std::string(""), myNode1.GetId());

  // Create new node and validate
  zNode::Node myNode2;
  TEST_EQ(std::string(""), myNode2.GetType());
  TEST_NEQ(std::string(""), myNode2.GetId());

  // Return success
  return (0);

}

int
zNodeTest_NodeTableDefaults(void* arg_)
{

  // Create new node table and validate
  zNode::Table *MyNodeTable = new zNode::Table;

  // Cleanup
  delete (MyNodeTable);

  // Return success
  return (0);

}
