#include <stdint.h>
#include <netinet/in.h>
#include <string.h>

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
using namespace zSocket;

int
zSocketTest_BufferDefaults( void* arg_ )
{

    ZLOG_DEBUG( "#############################################################" );
    ZLOG_DEBUG( "# zSocketTest_BufferDefaults()" );
    ZLOG_DEBUG( "#############################################################" );

    // Create new packet and validate
    SocketBuffer mySb1;
    TEST_ISNOT_NULL( mySb1.Head() );
    TEST_EQ( mySb1.Head(), mySb1.Data() );
    TEST_IS_ZERO( mySb1.Length() );
    TEST_IS_ZERO( mySb1.Size() );
    TEST_EQ( mySb1.TotalSize(), 1500 );
    TEST_EQ( std::string( "" ), mySb1.Str() );

    // Create new packet of set size and validate
    SocketBuffer mySb2( 500 );
    TEST_ISNOT_NULL( mySb2.Head() );
    TEST_EQ( mySb1.Head(), mySb1.Data() );
    TEST_ISNOT_NULL( mySb2.Data() );
    TEST_IS_ZERO( mySb2.Length() );
    TEST_IS_ZERO( mySb2.Size() );
    TEST_EQ( mySb2.TotalSize(), 500 );
    TEST_EQ( std::string( "" ), mySb2.Str() );

    // Return success
    return (0);

}

int
zSocketTest_AddressDefaults( void* arg_ )
{

    ZLOG_DEBUG( "#############################################################" );
    ZLOG_DEBUG( "# zSocketTest_AddressDefaults()" );
    ZLOG_DEBUG( "#############################################################" );

    // Create new socket address and validate
    SocketAddress *myAddr = new SocketAddress;
    TEST_ISNOT_NULL(myAddr);
    TEST_EQ( SocketType::TYPE_NONE, myAddr->Type() );
    TEST_EQ( std::string( "" ), myAddr->Address() );
    delete(myAddr);

    // Return success
    return (0);
}

int
zSocketTest_ObserverDefaults( void* arg_ )
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_ObserverDefaults()");
    ZLOG_DEBUG("#############################################################");

    TestObserver *myObserver = new TestObserver;
    TEST_ISNOT_NULL(myObserver);
    delete(myObserver);

    // Return success
    return (0);
}

int
zSocketTest_SocketDefaults( void* arg_ )
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_SocketDefaults()");
    ZLOG_DEBUG("#############################################################");

    TestSocket *mySocket = new TestSocket;
    TEST_ISNOT_NULL(mySocket);
    delete(mySocket);

    // Return success
    return (0);

}
