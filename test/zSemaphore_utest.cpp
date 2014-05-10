#include "UnitTest.h"
#include "zutils/zSemaphore.h"

using namespace std;
using namespace Test;
using namespace zUtils;

static int UnitTestSemphoreDefaults( int arg_ )
{
    // Create new semaphore and validate
    zSemaphore MySemaphore;
    TEST_NEQ( MySemaphore.GetFd(), 0 );
    TEST_EQ( MySemaphore.GetPending(), 0 );

    // Return success
    return (0);
}

static int UnitTestSemphoreNotifySingle( int arg_ )
{

    uint64_t obs = 0;

    // Create new semaphore and validate
    zSemaphore MySem;
    TEST_NEQ( MySem.GetFd(), 0 );
    TEST_EQ( MySem.GetPending(), 0 );

    // Test notify/acknowledge and validate
    MySem.Post();
    obs = MySem.GetPending();
    TEST_EQ( obs, 1 );
    obs = MySem.Acknowledge();
    TEST_EQ( obs, 1 );
    obs = MySem.GetPending();
    TEST_EQ( obs, 0 );

    // Return success
    return (0);
}

static int UnitTestSemaphoreNotifyMultiple( int arg_ )
{

    uint64_t obs = 0;

    // Create new semaphore and validate
    zSemaphore MySem;
    TEST_NEQ( MySem.GetFd(), 0 );
    TEST_EQ( MySem.GetPending(), 0 );

    // Test notify and validate
    for( int i = 0; i < 4; i++ )
    {
        MySem.Post( 4 );
        obs = MySem.GetPending();
        TEST_EQ( obs, ((i + 1) * 4) );
    } // end for

    // Test acknowledge and validate
    for( int i = 4; i > 0; i-- )
    {
        obs = MySem.Acknowledge( 4 );
        TEST_EQ( obs, (i * 4) );
        obs = MySem.GetPending();
        TEST_EQ( obs, ((i * 4) - 4) );
    } // end for

    // Return success
    return (0);
}

int zSemaphore_utest( void )
{
    INIT();
    UTEST( UnitTestSemphoreDefaults, 0 );
    UTEST( UnitTestSemphoreNotifySingle, 0 );
    UTEST( UnitTestSemaphoreNotifyMultiple, 0 );
    FINI();
}

