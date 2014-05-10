#include "UnitTest.h"
#include "zTimer.h"

using namespace std;
using namespace Test;
using namespace zUtils;

static int UnitTestTimer_Defaults( int arg_ )
{
    // Create new timer and validate
    zTimer MyTimer;
    TEST_NEQ( MyTimer.GetFd(), 0 );
    TEST_EQ( MyTimer.GetPending(), 0 );

    // Return success
    return (0);
}

static int UnitTestTimer_StartStop( int arg_ )
{
    // Create new timer and validate
    zTimer MyTimer;
    TEST_NEQ( MyTimer.GetFd(), 0 );
    TEST_EQ( MyTimer.GetPending(), 0 );

    // Start timer with half a second interval
    MyTimer.Start( 600000 );

    // Wait for timer to fire at least once and validate
    sleep(1);
    TEST_EQ( MyTimer.GetPending(), 1 );

    // Stop timer
    MyTimer.Stop();

    // Return success
    return (0);
}

int zTimer_utest( void )
{
    INIT();
    UTEST( UnitTestTimer_Defaults, 0 );
    UTEST( UnitTestTimer_StartStop, 0 );
    FINI();
}

