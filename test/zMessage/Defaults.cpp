#include "zMessageTest.h"

using namespace Test;
using namespace zUtils;

int zMessageTest_Defaults(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zMessageTest_Defaults()");
    ZLOG_DEBUG("#############################################################");

    // Create new node and validate
    zMessage::Message *myMessage = new zMessage::Message;
    TEST_ISNOT_NULL(myMessage);
    TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_NONE);
    TEST_EQ(std::string(""), myMessage->GetId());
    TEST_EQ(std::string(""), myMessage->GetTo());
    TEST_EQ(std::string(""), myMessage->GetFrom());

    // Cleanup
    delete (myMessage);

    // Return success
    return (0);

}
