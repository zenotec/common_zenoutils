#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>

#include <sstream>

#include <string>
#include <list>
#include <mutex>
#include <memory>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>
#include <zutils/zSocket.h>
#include <zutils/zLoopSocket.h>
#include <zutils/zInetSocket.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int zSocketTest_InetAddressGetSet(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_InetAddressGetSet()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    InetAddress myAddr;
    TEST_EQ(SocketType::TYPE_INET, myAddr.Type());
    TEST_EQ(std::string("0.0.0.0:0"), myAddr.Address());

    // Set socket address using string notation (bad address, bad port)
    TEST_FALSE(myAddr.Address(std::string("1:a")));
    TEST_EQ(std::string("0.0.0.0:0"), myAddr.Address());

    // Set socket address using string notation (good address, no port)
    TEST_FALSE(myAddr.Address(std::string("1.2.3.4")));
    TEST_EQ(std::string("0.0.0.0:0"), myAddr.Address());

    // Set socket address using string notation (good address, bad port)
    TEST_FALSE(myAddr.Address(std::string("1.2.3.4:123456")));
    TEST_EQ(std::string("0.0.0.0:0"), myAddr.Address());

    // Set socket address using string notation (good address, good port)
    TEST_TRUE(myAddr.Address(std::string("1.2.3.4:5")));
    TEST_EQ(std::string("1.2.3.4:5"), myAddr.Address());

    // Return success
    return (0);

}

int zSocketTest_InetAddressCompare(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_InetAddressCompare()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    InetAddress myAddr1;
    TEST_EQ(SocketType::TYPE_INET, myAddr1.Type());
    TEST_EQ(std::string("0.0.0.0:0"), myAddr1.Address());

    // Create second socket address and validate
    InetAddress myAddr2;
    TEST_EQ(SocketType::TYPE_INET, myAddr2.Type());
    TEST_EQ(std::string("0.0.0.0:0"), myAddr2.Address());

    // Compare address (match)
    TEST_TRUE(myAddr1 == myAddr2);
    TEST_FALSE(myAddr1 != myAddr2);

    // Set socket address using string notation
    myAddr1.Address("1.2.3.4:56789");
    TEST_EQ(std::string("1.2.3.4:56789"), myAddr1.Address());

    // Compare address (no match)
    TEST_FALSE(myAddr1 == myAddr2);
    TEST_TRUE(myAddr1 != myAddr2);

    // Set socket address using string notation
    myAddr2.Address("1.2.3.4:56789");
    TEST_EQ(std::string("1.2.3.4:56789"), myAddr2.Address());

    // Compare address (match)
    TEST_TRUE(myAddr1 == myAddr2);
    TEST_FALSE(myAddr1 != myAddr2);

    // Set socket address using string notation
    myAddr1.Address("1.2.3.4:56788");
    TEST_EQ(std::string("1.2.3.4:56788"), myAddr1.Address());

    // Compare address (no match)
    TEST_FALSE(myAddr1 == myAddr2);
    TEST_TRUE(myAddr1 != myAddr2);

    // Return success
    return (0);

}

