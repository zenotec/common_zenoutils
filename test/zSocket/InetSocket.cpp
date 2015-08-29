#include "zSocketTest.h"

#include <zutils/zInet.h>

using namespace Test;
using namespace zUtils;

int zSocketTest_InetSocketDefault(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_InetSocketDefault()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    zSocket::InetAddress myAddr("lo");
    TEST_EQ(std::string("127.0.0.1:0"), myAddr.GetAddress());

    // Set socket address
    TEST_TRUE(myAddr.SetAddress(std::string("127.0.0.1:9876")));
    TEST_EQ(std::string("127.0.0.1:9876"), myAddr.GetAddress());

    // Create new socket handler and validate
    zSocket::Handler *myHandler = new zSocket::Handler;
    TEST_ISNOT_NULL(myHandler);

    // Create new socket and validate
    zSocket::InetSocket *mySock = new zSocket::InetSocket(myAddr);
    TEST_ISNOT_NULL(mySock);
    TEST_TRUE(myHandler->Bind(mySock));

    // Cleanup
    myHandler->Close();
    delete (mySock);
    delete (myHandler);

    // Return success
    return (0);

}

int zSocketTest_InetSocketSendReceiveLoop(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_InetSocketSendReceiveLoop()");
    ZLOG_DEBUG("#############################################################");

    unsigned int n = 0;

    // Create new socket address and validate
    zSocket::InetAddress *srcAddr = new zSocket::InetAddress("lo");
    TEST_EQ(std::string("127.0.0.1:0"), srcAddr->GetAddress());

    // Create new socket address and validate
    zSocket::InetAddress *dstAddr = new zSocket::InetAddress;
    TEST_EQ(std::string("0.0.0.0:0"), dstAddr->GetAddress());

    // Set socket address
    TEST_TRUE(srcAddr->SetPort("9877"));
    TEST_EQ(std::string("127.0.0.1:9877"), srcAddr->GetAddress());
    TEST_TRUE(srcAddr != dstAddr);

    // Create new socket handler and validate
    zSocket::Handler *myHandler = new zSocket::Handler;
    TEST_ISNOT_NULL(myHandler);

    // Create new socket and validate
    zSocket::InetSocket *mySock = new zSocket::InetSocket(*srcAddr);
    TEST_ISNOT_NULL(mySock);
    TEST_TRUE(myHandler->Bind(mySock));

    // Send string and validate
    std::string expStr = "Hello Universe";
    TEST_EQ((int )mySock->Send(srcAddr, expStr), (int )expStr.size());

    // Do not need a context switch as the underlying code puts the buffer into the queue
    //   and therefore the buffer is immediately available

    // Receive string back and validate
    std::string obsStr;
    TEST_EQ((int )mySock->Receive(dstAddr, obsStr), (int )expStr.size());
    TEST_EQ(srcAddr->GetAddress(), dstAddr->GetAddress());
    TEST_EQ(expStr, obsStr);

    // Cleanup
    myHandler->Close();
    delete (mySock);
    delete (myHandler);
    delete (dstAddr);
    delete (srcAddr);

    // Return success
    return (0);

}

int zSocketTest_InetSocketSendReceiveSock2Sock(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_InetSocketSendReceiveSock2Sock()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    zSocket::InetAddress *srcAddr = new zSocket::InetAddress;
    TEST_EQ(std::string("0.0.0.0:0"), srcAddr->GetAddress());

    // Create new socket address and validate
    zSocket::InetAddress *dstAddr = new zSocket::InetAddress;
    TEST_EQ(std::string("0.0.0.0:0"), dstAddr->GetAddress());

    // Set socket address
    TEST_TRUE(srcAddr->SetAddress("127.0.1.1:9878"));
    TEST_EQ(std::string("127.0.1.1:9878"), srcAddr->GetAddress());

    // Set socket address
    TEST_TRUE(dstAddr->SetAddress("127.0.2.1:9878"));
    TEST_EQ(std::string("127.0.2.1:9878"), dstAddr->GetAddress());

    // Create new socket handler and validate
    zSocket::Handler *myHandler1 = new zSocket::Handler;
    TEST_ISNOT_NULL(myHandler1);

    // Create new socket and validate
    zSocket::InetSocket *mySock1 = new zSocket::InetSocket(*srcAddr);
    TEST_ISNOT_NULL(mySock1);
    TEST_TRUE(myHandler1->Bind(mySock1));

    // Create new socket handler and validate
    zSocket::Handler *myHandler2 = new zSocket::Handler;
    TEST_ISNOT_NULL(myHandler2);

    // Create new socket and validate
    zSocket::InetSocket *mySock2 = new zSocket::InetSocket(*dstAddr);
    TEST_ISNOT_NULL(mySock2);
    TEST_TRUE(myHandler2->Bind(mySock2));

    // Send string and validate
    std::string expStr = "Hello Universe";
    TEST_EQ((int )mySock1->Send(dstAddr, expStr), (int )expStr.size());

    // Sleep a moment to allow context switch
    usleep(100000);

    // Receive string back and validate
    zSocket::InetAddress *obsAddr = new zSocket::InetAddress;
    std::string obsStr;
    TEST_EQ((int )mySock2->Receive(obsAddr, obsStr), (int )expStr.size());
    TEST_EQ(obsAddr->GetAddress(), srcAddr->GetAddress());
    TEST_EQ(obsAddr->GetType(), srcAddr->GetType());
    TEST_TRUE(*obsAddr == *srcAddr);
    TEST_EQ(expStr, obsStr);
    delete (obsAddr);

    // Cleanup
    myHandler1->Close();
    delete (mySock1);
    delete (myHandler1);

    myHandler2->Close();
    delete (mySock2);
    delete (myHandler2);

    delete (dstAddr);
    delete (srcAddr);

    // Return success
    return (0);

}

int zSocketTest_InetSocketObserver(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_InetSocketObserver()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    zSocket::InetAddress *srcAddr = new zSocket::InetAddress;
    TEST_EQ(std::string("0.0.0.0:0"), srcAddr->GetAddress());

    // Create new socket address and validate
    zSocket::InetAddress *dstAddr = new zSocket::InetAddress;
    TEST_EQ(std::string("0.0.0.0:0"), dstAddr->GetAddress());

    // Set socket address
    TEST_TRUE(srcAddr->SetAddress("127.0.0.1:9879"));
    TEST_EQ(std::string("127.0.0.1:9879"), srcAddr->GetAddress());
    TEST_TRUE(srcAddr != dstAddr);

    // Create new socket handler and validate
    zSocket::Handler *myHandler = new zSocket::Handler;
    TEST_ISNOT_NULL(myHandler);

    // Create new observer and validate
    TestObserver myObserver;

    // Register handler
    TEST_TRUE(myHandler->Register(&myObserver));

    // Start listening thread
    TEST_TRUE(myHandler->StartListener(1000));

    // Create new socket and validate
    zSocket::InetSocket *mySock = new zSocket::InetSocket(*srcAddr);
    TEST_ISNOT_NULL(mySock);
    TEST_TRUE(myHandler->Bind(mySock));

    // Send string and validate
    std::string expStr = "Hello Universe";
    TEST_EQ((int )mySock->Send(srcAddr, expStr), (int )expStr.size());

    // Sleep a moment to allow context switch
    usleep(100000);

    // Wait for socket observer to be notified
    zSocket::Buffer *sb = myObserver.WaitForPacket(1000);
    TEST_ISNOT_NULL(sb);

    // Cleanup
    delete (sb);
    TEST_TRUE(myHandler->StopListener(1000));
    myHandler->Unregister(&myObserver);
    myHandler->Close();
    delete (mySock);
    delete (myHandler);

    delete (dstAddr);
    delete (srcAddr);

    // Return success
    return (0);

}

