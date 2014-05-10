#include "UnitTest.h"

#include "zQueue.h"
#include "zSemaphore.h"
#include "zSelect.h"

using namespace std;
using namespace Test;
using namespace zUtils;


static int UnitTestSelect_Defaults( int arg_ )
{

    // Create new node and validate
    zSelect *MySelect = new zSelect();

    // Cleanup
    delete (MySelect);

    // Return success
    return (0);

}

static int UnitTestSelect_Semaphore( int arg_ )
{

    // Create new select class and validate
    zSelect MySelect;

    // Create new semaphore and validate
    zSemaphore MySem;
    TEST_NEQ( MySem.GetFd(), 0 );
    TEST_EQ( MySem.GetPending(), 0 );

    // Add event to select event list
    MySelect.RegisterEvent( &MySem );

    // Notify event
    MySem.Post();
    TEST_EQ( MySem.GetPending(), 1 );

    // Wait on event (should return immediately)
    TEST_TRUE( MySelect.Wait(1000) );

    // Acknowledge event
    MySem.Acknowledge();
    TEST_EQ( MySem.GetPending(), 0 );

    // Wait on event (should timeout)
    TEST_FALSE( MySelect.Wait(1000) );

    // Cleanup
    MySelect.DelEvent( &MySem );

    // Return success
    return (0);
}

static int UnitTestSelect_Queue( int arg_ )
{

    // Create new select class and validate
    zSelect MySelect;

    // Create new queue and validate
    zQueue<std::string>MyQueue;
    TEST_NEQ( MyQueue.GetFd(), 0 );
    TEST_EQ( MyQueue.GetPending(), 0 );

    // Add event to select event list
    MySelect.RegisterEvent( &MyQueue );

    // Wait on event (should timeout)
    TEST_FALSE( MySelect.Wait(1000) );

    // Add item to queue and validate
    std::string str( "MyQueue" );
    MyQueue.Push( str );
    TEST_EQ( MyQueue.Size(), 1 );
    TEST_FALSE( MyQueue.Empty() );
    TEST_EQ( MyQueue.Front(), str );
    TEST_EQ( MyQueue.Back(), str );

    // Wait on event (should return immediately)
    TEST_TRUE( MySelect.Wait(1000) );

    // Remove item from queue and validate
    MyQueue.Pop();
    TEST_EQ( MyQueue.Size(), 0 );
    TEST_TRUE( MyQueue.Empty() );

    // Wait on event (should timeout)
    TEST_FALSE( MySelect.Wait(1000) );

    // Cleanup
    MySelect.DelEvent( &MyQueue );

    // Return success
    return (0);
}

int zSelect_utest( void )
{

    INIT();
    UTEST( UnitTestSelect_Defaults, 0 );
    UTEST( UnitTestSelect_Semaphore, 0 );
    UTEST( UnitTestSelect_Queue, 0 );
    FINI();

}

