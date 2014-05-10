#include "UnitTest.h"
#include "zutils/zEvent.h"

using namespace std;
using namespace Test;
using namespace zUtils;

static int UnitTestEvent_Defaults( int arg_ )
{
    // Create new event and validate
    zEvent MyEvent;
    TEST_EQ( MyEvent.GetFd(), 0 );
    TEST_EQ( MyEvent.GetPending(), 0 );

    // Return success
    return (0);
}

int zEvent_utest( void )
{
    INIT();
    UTEST( UnitTestEvent_Defaults, 0 );
    FINI();
}

