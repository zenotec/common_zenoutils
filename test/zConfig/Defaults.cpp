#include "UnitTest.h"
#include "zConfTest.h"

using namespace Test;
using namespace zUtils;

int
zConfigTest_DataDefaults( void* arg_ )
{
    // Create new configuration data item and verify
    zConfig::Configuration *myData = new zConfig::Configuration;
    TEST_ISNOT_NULL(myData);

    // Cleanup
    delete (myData);

    // Return success
    return (0);
}

int
zConfigTest_ConnectorDefaults( void* arg_ )
{
    // Create new configuration data connector and verify
    TestConnector *myConnector = new TestConnector;
    TEST_ISNOT_NULL(myConnector);

    // Cleanup
    delete (myConnector);

    // Return success
    return (0);
}

int
zConfigTest_HandlerDefaults( void* arg_ )
{
    // Create new configuration data connector and verify
    zConfig::ConfigurationHandler *MyHandler = new zConfig::ConfigurationHandler;
    TEST_ISNOT_NULL(MyHandler);

    // Cleanup
    delete (MyHandler);

    // Return success
    return (0);
}