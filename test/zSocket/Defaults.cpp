#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_BufferDefaults( void* arg_ )
{

    ZLOG_DEBUG( "#############################################################" );
    ZLOG_DEBUG( "# zSocketTest_BufferDefaults()" );
    ZLOG_DEBUG( "#############################################################" );

    // Create new packet and validate
    zSocket::SocketBuffer mySb1;
    TEST_ISNOT_NULL( mySb1.Head() );
    TEST_EQ( mySb1.Head(), mySb1.Data() );
    TEST_IS_ZERO( mySb1.Length() );
    TEST_IS_ZERO( mySb1.Size() );
    TEST_EQ( mySb1.TotalSize(), 1500 );
    TEST_EQ( std::string( "" ), mySb1.Str() );

    // Create new packet of set size and validate
    zSocket::SocketBuffer mySb2( 500 );
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
    zSocket::SocketAddress myAddr1;
    TEST_EQ( zSocket::SocketAddress::TYPE_NONE, myAddr1.GetType() );
    TEST_EQ( std::string( "" ), myAddr1.GetAddress() );

    // Create Socket address using string notation
    zSocket::SocketAddress myAddr2( zSocket::SocketAddress::TYPE_INET, "1.2.3.4:56789" );
    TEST_EQ( zSocket::SocketAddress::TYPE_INET, myAddr2.GetType() );
    TEST_EQ( std::string( "1.2.3.4:56789" ), myAddr2.GetAddress() );

    // Return success
    return (0);
}

int
zSocketTest_ObserverDefaults( void* arg_ )
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_ObserverDefaults()");
    ZLOG_DEBUG("#############################################################");

    TestObserver myObserver;

    // Return success
    return (0);
}

int
zSocketTest_SocketDefaults( void* arg_ )
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zSocketTest_SocketDefaults()");
    ZLOG_DEBUG("#############################################################");

    // Create new socket address and validate
    zSocket::SocketAddress myAddr;
    TEST_EQ( zSocket::SocketAddress::TYPE_NONE, myAddr.GetType() );
    TEST_EQ( std::string( "" ), myAddr.GetAddress() );

    TestSocket mySocket( &myAddr );

    // Return success
    return (0);

}
