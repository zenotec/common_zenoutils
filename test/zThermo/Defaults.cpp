#include "UnitTest.h"
#include "zThermoTest.h"

using namespace Test;
using namespace zUtils;

int
zThermoTest_Defaults( void* arg_ )
{
    // Create new configuration data item and verify
    TestSensor *MySensor = new TestSensor;
    TEST_ISNOT_NULL(MySensor);

    // Cleanup
    delete (MySensor);

    // Return success
    return (0);
}

