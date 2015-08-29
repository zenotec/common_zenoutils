#include "UnitTest.h"
#include "zConfTest.h"

using namespace Test;
using namespace zUtils;

int
zConfTest_DataDefaults( void* arg_ )
{
    // Create new configuration data item and verify
    zConf::Data *myData = new zConf::Data;
    TEST_ISNOT_NULL(myData);

    // Cleanup
    delete (myData);

    // Return success
    return (0);
}

int
zConfTest_ConnectorDefaults( void* arg_ )
{
    // Create new configuration data connector and verify
    TestConnector *myConnector = new TestConnector;
    TEST_ISNOT_NULL(myConnector);

    // Cleanup
    delete (myConnector);

    // Return success
    return (0);
}
