#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int zSocketTest_AddressGetSet(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_AddressGetSet()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    zSocket::SocketAddress myAddr(zSocket::SocketAddress::TYPE_INET);
    TEST_EQ(zSocket::SocketAddress::TYPE_INET, myAddr.GetType());
    TEST_EQ(std::string(""), myAddr.GetAddress());
    TEST_TRUE(myAddr.SetAddress(std::string("some_address")));
    TEST_EQ(std::string("some_address"), myAddr.GetAddress());

    // Return success
    return (0);
}

int zSocketTest_AddressCompare(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_AddressCompare()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    zSocket::SocketAddress myAddr1(zSocket::SocketAddress::TYPE_INET);
    TEST_EQ(std::string(""), myAddr1.GetAddress());

    // Create another new socket address and validate
    zSocket::SocketAddress myAddr2(zSocket::SocketAddress::TYPE_INET);
    TEST_EQ(std::string(""), myAddr2.GetAddress());

    // Compare the addresses
    TEST_TRUE(myAddr1 == myAddr2);
    TEST_FALSE(myAddr1 != myAddr2);

    // Set first address to new value and validate
    std::string expAddr = "some_address";
    TEST_TRUE(myAddr1.SetAddress(expAddr));
    TEST_EQ(expAddr, myAddr1.GetAddress());

    // Compare the addresses
    TEST_TRUE(myAddr1 != myAddr2);
    TEST_FALSE(myAddr1 == myAddr2);

    // Set first address to new value and validate
    TEST_TRUE(myAddr2.SetAddress(expAddr));
    TEST_EQ(expAddr, myAddr2.GetAddress());

    // Compare the addresses
    TEST_TRUE(myAddr1 == myAddr2);
    TEST_FALSE(myAddr1 != myAddr2);

    // Return success
    return (0);
}

