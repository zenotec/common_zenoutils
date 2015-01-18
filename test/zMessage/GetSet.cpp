#include "zMessageTest.h"

using namespace Test;
using namespace zUtils;

int
zMessageTest_GetSet(void* arg_)
{
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

