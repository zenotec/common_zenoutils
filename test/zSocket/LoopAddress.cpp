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

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int zSocketTest_LoopAddressGetSet(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_LoopAddressGetSet()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    LoopAddress myAddr;
    TEST_EQ(SocketType::TYPE_LOOP, myAddr.Type());
    TEST_EQ(std::string(""), myAddr.Address());

    // Set socket address
    TEST_TRUE(myAddr.Address(std::string("loop")));
    TEST_EQ(std::string("loop"), myAddr.Address());

    // Return success
    return (0);

}

int zSocketTest_LoopAddressCompare(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_LoopAddressCompare()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    LoopAddress myAddr1;
    TEST_EQ(SocketType::TYPE_LOOP, myAddr1.Type());
    TEST_EQ(std::string(""), myAddr1.Address());

    // Create second socket address and validate
    LoopAddress myAddr2;
    TEST_EQ(SocketType::TYPE_LOOP, myAddr2.Type());
    TEST_EQ(std::string(""), myAddr2.Address());

    // Compare address (match)
    TEST_TRUE(myAddr1 == myAddr2);
    TEST_FALSE(myAddr1 != myAddr2);

    // Set socket address
    myAddr1.Address("loop");
    TEST_EQ(std::string("loop"), myAddr1.Address());

    // Compare address (no match)
    TEST_FALSE(myAddr1 == myAddr2);
    TEST_TRUE(myAddr1 != myAddr2);

    // Set socket address
    myAddr2.Address("loop");
    TEST_EQ(std::string("loop"), myAddr2.Address());

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

