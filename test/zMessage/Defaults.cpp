#include "zMessageTest.h"


using namespace Test;
using namespace zUtils;

int zMessageTest_MessageDefaults(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zMessageTest_MessageDefaults()");
    ZLOG_DEBUG("#############################################################");

    // Create new node and validate
    zMessage::Message *myMessage = new zMessage::Message;
    TEST_ISNOT_NULL(myMessage);
    TEST_EQ(myMessage->GetType(), zMessage::Message::TYPE_NONE);
    TEST_EQ(std::string(""), myMessage->GetId());
    TEST_EQ(std::string(""), myMessage->GetDst());
    TEST_EQ(std::string(""), myMessage->GetSrc());

    // Cleanup
    delete (myMessage);

    // Return success
    return (0);

}

int zMessageTest_MessageSocketDefaults(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zMessageTest_MessageSocketDefaults()");
    ZLOG_DEBUG("#############################################################");

    // Setup network socket
    zSocket::InetSocket *MySock = new zSocket::InetSocket;
    TEST_ISNOT_NULL(MySock);

    // Create new message socket and validate
    zMessage::MessageSocket *MyMsgSock = new zMessage::MessageSocket(MySock);
    TEST_ISNOT_NULL(MyMsgSock);

    // Clean up
    delete (MyMsgSock);
    MySock->Close();
    delete (MySock);

    // Return success
    return (0);
}
