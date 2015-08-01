#include "zSocketTest.h"

#include <zutils/zInetSocket.h>

using namespace Test;
using namespace zUtils;

int zSocketTest_InetAddressGetSet(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_InetAddressGetSet()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    zSocket::InetAddress myAddr;
    TEST_EQ(zSocket::Address::TYPE_INET, myAddr.GetType());
    TEST_EQ(std::string("0.0.0.0:0"), myAddr.GetAddress());
    TEST_EQ(std::string("0.0.0.0"), myAddr.GetIpAddr());
    TEST_EQ(std::string("0"), myAddr.GetPort());
    TEST_EQ(std::string("0.0.0.0"), myAddr.GetNetmask());
    TEST_EQ(std::string("0.0.0.0"), myAddr.GetBroadcast());

    // Set socket address using string notation
    std::string expAddrString = "1.2.3.4:56789";
    myAddr.SetAddress(expAddrString);
    TEST_EQ(expAddrString, myAddr.GetAddress());
    TEST_EQ(std::string("56789"), myAddr.GetPort());

    // Return success
    return (0);

}

int zSocketTest_InetAddressCompare(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_InetAddressCompare()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    zSocket::InetAddress myAddr1;
    TEST_EQ(zSocket::Address::TYPE_INET, myAddr1.GetType());
    TEST_EQ(std::string("0.0.0.0:0"), myAddr1.GetAddress());

    // Create second socket address and validate
    zSocket::InetAddress myAddr2;
    TEST_EQ(zSocket::Address::TYPE_INET, myAddr2.GetType());
    TEST_EQ(std::string("0.0.0.0:0"), myAddr2.GetAddress());

    // Compare address (match)
    TEST_TRUE(myAddr1 == myAddr2);
    TEST_FALSE(myAddr1 != myAddr2);

    // Set socket address using string notation
    myAddr1.SetAddress("1.2.3.4:56789");
    TEST_EQ(std::string("1.2.3.4:56789"), myAddr1.GetAddress());

    // Compare address (no match)
    TEST_FALSE(myAddr1 == myAddr2);
    TEST_TRUE(myAddr1 != myAddr2);

    // Set socket address using string notation
    myAddr2.SetAddress("1.2.3.4:56789");
    TEST_EQ(std::string("1.2.3.4:56789"), myAddr2.GetAddress());

    // Compare address (match)
    TEST_TRUE(myAddr1 == myAddr2);
    TEST_FALSE(myAddr1 != myAddr2);

    // Set socket address using string notation
    myAddr1.SetAddress("1.2.3.4:56788");
    TEST_EQ(std::string("1.2.3.4:56788"), myAddr1.GetAddress());

    // Compare address (no match)
    TEST_FALSE(myAddr1 == myAddr2);
    TEST_TRUE(myAddr1 != myAddr2);

    // Return success
    return (0);

}

int zSocketTest_InetAddressIface(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_InetAddressIface()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    zSocket::InetAddress myAddr("eth0");
    TEST_EQ(zSocket::Address::TYPE_INET, myAddr.GetType());
    TEST_NEQ(std::string("0.0.0.0:0"), myAddr.GetAddress());
    TEST_NEQ(std::string("0.0.0.0"), myAddr.GetIpAddr());
    TEST_EQ(std::string("0"), myAddr.GetPort());
    TEST_NEQ(std::string("0.0.0.0"), myAddr.GetNetmask());
    TEST_NEQ(std::string("0.0.0.0"), myAddr.GetBroadcast());

    // Return success
    return (0);

}

