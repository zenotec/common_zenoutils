#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

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
#include <zutils/zUnixSocket.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int zSocketTest_UnixAddressGetSet(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_UnixAddressGetSet()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    UnixAddress myAddr;
    TEST_EQ(SocketType::TYPE_UNIX, myAddr.Type());
    TEST_EQ(std::string(""), myAddr.Address());

    // Set socket address
    TEST_TRUE(myAddr.Address(std::string("unix")));
    TEST_EQ(std::string("unix"), myAddr.Address());

    // Return success
    return (0);

}

int zSocketTest_UnixAddressCompare(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_UnixAddressCompare()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    UnixAddress myAddr1;
    TEST_EQ(SocketType::TYPE_UNIX, myAddr1.Type());
    TEST_EQ(std::string(""), myAddr1.Address());

    // Create second socket address and validate
    UnixAddress myAddr2;
    TEST_EQ(SocketType::TYPE_UNIX, myAddr2.Type());
    TEST_EQ(std::string(""), myAddr2.Address());

    // Compare address (match)
    TEST_TRUE(myAddr1 == myAddr2);
    TEST_FALSE(myAddr1 != myAddr2);

    // Set socket address
    myAddr1.Address("unix");
    TEST_EQ(std::string("unix"), myAddr1.Address());

    // Compare address (no match)
    TEST_FALSE(myAddr1 == myAddr2);
    TEST_TRUE(myAddr1 != myAddr2);

    // Set socket address
    myAddr2.Address("unix");
    TEST_EQ(std::string("unix"), myAddr2.Address());

    // Compare address (match)
    TEST_TRUE(myAddr1 == myAddr2);
    TEST_FALSE(myAddr1 != myAddr2);

    // Set socket address
    myAddr1.Address("");
    TEST_EQ(std::string(""), myAddr1.Address());

    // Compare address (no match)
    TEST_FALSE(myAddr1 == myAddr2);
    TEST_TRUE(myAddr1 != myAddr2);

    // Return success
    return (0);

}

