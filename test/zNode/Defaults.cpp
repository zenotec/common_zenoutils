#include "zNodeTest.h"

using namespace Test;
using namespace zUtils;

int
zNodeTest_NodeDefaults(void* arg_)
{

  // Create new node from zData object
  zData::Data MyData;
  zNode::Node MyNode1(MyData);
  TEST_EQ(MyNode1.GetType(), std::string(""));
  TEST_EQ(MyNode1.GetId(), std::string(""));

  // Create new node and validate
  zNode::Node MyNode2;
  TEST_EQ(MyNode2.GetType(), std::string(""));
  TEST_EQ(MyNode2.GetId(), std::string(""));

  // Return success
  return (0);

}

int
zNodeTest_NodeTableDefaults(void* arg_)
{

  // Create new node table and validate
  zNode::NodeTable *MyNodeTable = new zNode::NodeTable;

  // Cleanup
  delete (MyNodeTable);

  // Return success
  return (0);

}
