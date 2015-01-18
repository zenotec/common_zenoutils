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
  zMessage::Message *MyMessage = new zMessage::Message();
  TEST_EQ(MyMessage->GetId(), std::string(""));
  TEST_EQ(MyMessage->GetType(), std::string(""));
  TEST_EQ(MyMessage->GetTo().GetId(), to.GetId());
  TEST_EQ(MyMessage->GetFrom().GetId(), from.GetId());

  // Test setting "id" field
  id = "0123456789";
  MyMessage->SetId(id);
  TEST_EQ(MyMessage->GetId(), id);

  // Test setting "type" field
  type = "test";
  MyMessage->SetType(type);
  TEST_EQ(MyMessage->GetType(), type);

  // Test setting "to" field
  to.SetId("toId");
  MyMessage->SetTo(to);
  TEST_EQ(MyMessage->GetTo().GetId(), to.GetId());

  // Test setting "from" field
  from.SetId("fromId");
  MyMessage->SetFrom(from);
  TEST_EQ(MyMessage->GetFrom().GetId(), from.GetId());

  // Test setting "data" field
  data = MyMessage->GetData();
  data.SetValue("Key", "Value");
  MyMessage->SetData(data);

  // Cleanup
  delete (MyMessage);

  // Return success
  return (0);

}

