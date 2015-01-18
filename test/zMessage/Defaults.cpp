#include "zMessageTest.h"

using namespace Test;
using namespace zUtils;

int
zMessageTest_Defaults(void* arg_)
{

  // Create new node and validate
  zMessage::Message *myMessage = new zMessage::Message;
  TEST_ISNOT_NULL(myMessage);
  TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_NONE);
  TEST_EQ(std::string(""), myMessage->GetId());
  TEST_NEQ(std::string(""), myMessage->GetTo().GetId());
  TEST_EQ(std::string(""), myMessage->GetTo().GetType());
  TEST_NEQ(std::string(""), myMessage->GetFrom().GetId());
  TEST_EQ(std::string(""), myMessage->GetFrom().GetType());

  // Cleanup
  delete (myMessage);

  // Return success
  return (0);

}
