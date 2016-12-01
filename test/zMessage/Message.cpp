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
  std::string to;
  std::string from;

  // Create new data object and validate
  zData::Data *myData = new zData::Data;
  TEST_ISNOT_NULL(myData);

  // Create new message and validate
  zMessage::Message *myMessage = new zMessage::Message;
  TEST_EQ(myMessage->GetId(), std::string(""));
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_NONE);
  TEST_EQ(myMessage->GetDst(), to);
  TEST_EQ(myMessage->GetSrc(), from);

  // Test setting "id" field
  id = "0123456789";
  TEST_TRUE(myMessage->SetId(id));
  TEST_EQ(myMessage->GetId(), id);

  // Test setting "type" field
  TEST_TRUE(myMessage->SetType(zMessage::Message::TYPE_HELLO));
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_HELLO);

  // Test setting "to" field
  to = "toStr";
  TEST_TRUE(myMessage->SetDst(to));
  TEST_EQ(myMessage->GetDst(), to);

  // Test setting "from" field
  from = "fromId";
  TEST_TRUE(myMessage->SetSrc(from));
  TEST_EQ(myMessage->GetSrc(), from);

  // Test setting "data" field
  *myData = myMessage->GetData();
  TEST_TRUE(myData->Put("Value", "Key"));
  TEST_TRUE(myMessage->SetData(*myData));

  // Cleanup
  delete (myMessage);
  delete (myData);

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
  std::string to;
  std::string from;

  // Create new data object and validate
  zData::Data myData("MsgData");
  TEST_EQ_MSG(std::string("MsgData"), myData.Key(), myData.GetJson());
  TEST_TRUE_MSG(myData.Put("Value"), myData.GetJson());

  // Create new message and validate
  zMessage::Message *myMessage1 = new zMessage::Message;
  TEST_EQ(myMessage1->GetId(), std::string(""));
  TEST_EQ(myMessage1->GetType(), zMessage::Message::TYPE_NONE);
  TEST_EQ(myMessage1->GetDst(), to);
  TEST_EQ(myMessage1->GetSrc(), from);

  // Create second new message and validate
  zMessage::Message *myMessage2 = new zMessage::Message(*myMessage1);
  TEST_EQ(myMessage2->GetId(), std::string(""));
  TEST_EQ(myMessage2->GetType(), zMessage::Message::TYPE_NONE);
  TEST_EQ(myMessage2->GetDst(), to);
  TEST_EQ(myMessage2->GetSrc(), from);

  // Verify messages are the same
  TEST_TRUE(*myMessage1 == *myMessage2);

  // Setup first message
  std::string id = "0123456789";
  TEST_TRUE_MSG(myMessage1->SetId(id), myMessage1->GetJson());
  TEST_TRUE_MSG(myMessage1->SetType(zMessage::Message::TYPE_DATA), myMessage1->GetJson());
  TEST_TRUE_MSG(myMessage1->SetDst(to), myMessage1->GetJson());
  TEST_TRUE_MSG(myMessage1->SetSrc(from), myMessage1->GetJson());
  myData.DisplayJson();
  TEST_TRUE_MSG(myMessage1->SetData(myData), myMessage1->GetJson());

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

