#include "UnitTest.h"
#include "zutils/zNode.h"

using namespace std;
using namespace Test;
using namespace zUtils;

static int
UT_zNode_Defaults(int arg_)
{

  // Create new node from zData object
  zData MyData;
  zNode MyNode1(MyData);
  TEST_EQ( MyNode1.GetType(), string( "" ));
  TEST_EQ( MyNode1.GetId(), string( "" ));
  TEST_EQ( MyNode1.GetTardyCnt(), 0);

  // Create new node and validate
  zNode MyNode2;
  TEST_EQ( MyNode2.GetType(), string(""));
  TEST_EQ( MyNode2.GetId(), string(""));
  TEST_EQ( MyNode2.GetTardyCnt(), 0);

  // Return success
  return (0);

}

static int
UT_zNode_Methods(int arg_)
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

class TestObserver : public zNodeTableObserver
{
public:
  TestObserver() :
      Count(0)
  {
  }
  int Count;

protected:
  virtual void
  EventHandler(zNodeTableObserver::Event event_, zNode &node_)
  {
    this->Count++;
  }

private:
};

static int
UT_zNodeTable_Defaults(int arg_)
{

  // Create new node table and validate
  zNodeTable *MyNodeTable = new zNodeTable;

  // Cleanup
  delete (MyNodeTable);

  // Return success
  return (0);

}

static int
UT_zNodeTable_Methods(int arg_)
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

  // Create new node table test observer and validate
  TestObserver *MyObsvr = new TestObserver;
  TEST_IS_ZERO(MyObsvr->Count);

  // Create new node table and validate
  zNodeTable *MyNodeTable = new zNodeTable();
  TEST_IS_NULL(MyNodeTable->FindNode(MyNode->GetId()));

  // Register observer
  MyNodeTable->Register(MyObsvr);
  TEST_IS_ZERO(MyObsvr->Count);

  // Add node to table and validate
  TEST_TRUE(MyNodeTable->AddNode(*MyNode));
  TEST_ISNOT_NULL(MyNodeTable->FindNode(MyNode->GetId()));
  TEST_EQ(1, MyObsvr->Count);

  // Remove node
  TEST_TRUE(MyNodeTable->RemoveNode(MyNode->GetId()));
  TEST_IS_NULL(MyNodeTable->FindNode(MyNode->GetId()));
  TEST_EQ(2, MyObsvr->Count);

  // Add node back to table and validate
  TEST_TRUE(MyNodeTable->AddNode(*MyNode));
  TEST_ISNOT_NULL(MyNodeTable->FindNode(MyNode->GetId()));
  TEST_EQ(3, MyObsvr->Count);

  // Wait for node to expire
  sleep(1);
  TEST_IS_NULL(MyNodeTable->FindNode(MyNode->GetId()));
  TEST_EQ(5, MyObsvr->Count);

  // Cleanup
  MyNodeTable->Unregister(MyObsvr);
  delete (MyObsvr);
  delete (MyNodeTable);
  delete (MyNode);

  // Return success
  return (0);
}

int
zNode_utest(void)
{

  INIT();
  UTEST( UT_zNode_Defaults, 0);
  UTEST( UT_zNode_Methods, 0);
  UTEST( UT_zNodeTable_Defaults, 0);
  UTEST( UT_zNodeTable_Methods, 0);
  FINI();

}

