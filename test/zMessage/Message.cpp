#include "zMessageTest.h"

using namespace Test;
using namespace zUtils;

int
zMessageTest_MessageGetSet(void* arg_)
{
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zMessageTest_MessageGetSet()");
  ZLOG_DEBUG("#############################################################");

  std::string id;
  std::string type;
  zNode::Node to;
  zNode::Node from;
  zData::Data data;

  // Create new message and validate
  zMessage::Message *MyMessage = new zMessage::Message;
  TEST_EQ(MyMessage->GetId(), std::string(""));
  TEST_EQ(MyMessage->GetType(), zMessage::Message::TYPE_NONE);
  TEST_NEQ(MyMessage->GetTo().GetId(), to.GetId());
  TEST_NEQ(MyMessage->GetFrom().GetId(), from.GetId());

  // Test setting "id" field
  id = "0123456789";
  TEST_TRUE(MyMessage->SetId(id));
  TEST_EQ(MyMessage->GetId(), id);

  // Test setting "type" field
  TEST_TRUE(MyMessage->SetType(zMessage::Message::TYPE_HELLO));
  TEST_EQ(MyMessage->GetType(), zMessage::Message::TYPE_HELLO);

  // Test setting "to" field
  TEST_TRUE(to.SetId("toId"));
  TEST_TRUE(MyMessage->SetTo(to));
  TEST_EQ(MyMessage->GetTo().GetId(), to.GetId());

  // Test setting "from" field
  TEST_TRUE(from.SetId("fromId"));
  TEST_TRUE(MyMessage->SetFrom(from));
  TEST_EQ(MyMessage->GetFrom().GetId(), from.GetId());

  // Test setting "data" field
  data = MyMessage->GetData();
  TEST_TRUE(data.SetValue("Key", "Value"));
  TEST_TRUE(MyMessage->SetData(data));

  // Cleanup
  delete (MyMessage);

  // Return success
  return (0);

}

int
zMessageTest_MessageCopy(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zMessageTest_MessageCopy()");
  ZLOG_DEBUG("#############################################################");

  std::string msgId1, msgId2;

  // Create new "to" node and validate
  zNode::Node to;
  TEST_TRUE(to.SetId("toId"));

  // Create new "from" node and validate
  zNode::Node from;
  TEST_TRUE(from.SetId("fromId"));

  // Create some data
  zData::Data data;
  TEST_TRUE(data.SetValue("Key", "Value"));

  // Create new message and validate
  zMessage::Message *myMessage1 = new zMessage::Message;
  TEST_EQ(myMessage1->GetId(), std::string(""));
  TEST_EQ(myMessage1->GetType(), zMessage::Message::TYPE_NONE);
  TEST_NEQ(myMessage1->GetTo().GetId(), to.GetId());
  TEST_NEQ(myMessage1->GetFrom().GetId(), from.GetId());

  // Create second new message and validate
  zMessage::Message *myMessage2 = new zMessage::Message(*myMessage1);
  TEST_EQ(myMessage2->GetId(), std::string(""));
  TEST_EQ(myMessage2->GetType(), zMessage::Message::TYPE_NONE);
  TEST_NEQ(myMessage2->GetTo().GetId(), to.GetId());
  TEST_NEQ(myMessage2->GetFrom().GetId(), from.GetId());

  // Verify messages are the same
  TEST_TRUE(*myMessage1 == *myMessage2);

  // Setup first message
  std::string id = "0123456789";
  TEST_TRUE(myMessage1->SetId(id));
  TEST_TRUE(myMessage1->SetType(zMessage::Message::TYPE_DATA));
  TEST_TRUE(myMessage1->SetTo(to));
  TEST_TRUE(myMessage1->SetFrom(from));
  TEST_TRUE(myMessage1->SetData(data));

  // Verify messages are NOT the same
  TEST_TRUE(*myMessage1 != *myMessage2);

  // Copy first message to second message
  *myMessage2 = *myMessage1;

  // Verify messages are the same
  TEST_TRUE(*myMessage1 == *myMessage2);

  // Cleanup
  delete (myMessage1);
  delete (myMessage2);

  // Return success
  return (0);

}

