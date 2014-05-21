#include "UnitTest.h"
#include "zutils/zNode.h"

using namespace std;
using namespace Test;
using namespace zUtils;

static int
UnitTestNodeDefaults(int arg_)
{

  // Create new node and validate
  zNode MyNode;
  TEST_EQ( MyNode.GetType(), string( "" ));
  TEST_EQ( MyNode.GetId(), string( "" ));
  TEST_EQ( MyNode.GetTardyCnt(), 0);

  // Return success
  return (0);

}

static int
UnitTestNodeMethods(int arg_)
{

  std::string exp;

  // Create new node and validate
  zNode *MyNode = new zNode();
  TEST_EQ( MyNode->GetType(), string( "" ));
  TEST_EQ( MyNode->GetId(), string( "" ));
  TEST_EQ( MyNode->GetTardyCnt(), 0);

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

  // Test incrementing tardy count
  TEST_EQ( MyNode->GetTardyCnt(), 0);
  MyNode->SetTardyCnt(MyNode->GetTardyCnt() + 1);
  TEST_EQ( MyNode->GetTardyCnt(), 1);
  MyNode->SetTardyCnt(MyNode->GetTardyCnt() + 1);
  TEST_EQ( MyNode->GetTardyCnt(), 2);

  // Cleanup
  delete (MyNode);

  // Return success
  return (0);
}

static int
UnitTestNodeTableDefaults(int arg_)
{

  // Create new node table and validate
  zNodeTable *MyNodeTable = new zNodeTable;

  // Cleanup
  delete (MyNodeTable);

  // Return success
  return (0);

}

static int
UnitTestNodeTableMethods(int arg_)
{

  // Create new node and validate
  zNode *MyNode = new zNode();
  TEST_EQ( MyNode->GetType(), "");
  TEST_EQ( MyNode->GetId(), "");
  TEST_EQ( MyNode->GetTardyCnt(), 0);

  // Initialize node
  MyNode->SetType("TestNode");
  TEST_EQ( MyNode->GetType(), "TestNode");
  MyNode->SetId("abcdef56789");
  TEST_EQ( MyNode->GetId(), "abcdef56789");
  MyNode->SetTardyCnt(3);
  TEST_EQ( MyNode->GetTardyCnt(), 3);

  // Create new node table and validate
  zNodeTable *MyNodeTable = new zNodeTable();

  // Update node
//  MyNodeTable->UpdateNode(MyNode->GetId());

  // Wait for safe insertion
  sleep(1);

//  // Find node
//  TEST_TRUE( MyNodeTable->FindNode( MyNode->GetId() ));
//
//  // Wait for node to expire
//  sleep(4);
//
//  // Find node
//  TEST_FALSE( MyNodeTable->FindNode( MyNode->GetId() ));
//
//  // Update node
//  MyNodeTable->UpdateNode(*MyNode);
//
//  // Wait for safe insertion
//  sleep(1);
//
//  // Find node
//  TEST_TRUE( MyNodeTable->FindNode( MyNode->GetId() ));
//
//  // Remove node
//  MyNodeTable->RemoveNode(*MyNode);
//
//  // Wait for safe removal
//  sleep(1);
//
//  // Find node
//  TEST_FALSE( MyNodeTable->FindNode( MyNode->GetId() ));

  // Cleanup
  delete (MyNodeTable);
  delete (MyNode);

  // Return success
  return (0);
}

int
zNode_utest(void)
{

  INIT();
  UTEST( UnitTestNodeDefaults, 0);
  UTEST( UnitTestNodeMethods, 0);
  UTEST( UnitTestNodeTableDefaults, 0);
  UTEST( UnitTestNodeTableMethods, 0);
  FINI();

}

