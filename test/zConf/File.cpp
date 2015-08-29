#include "UnitTest.h"
#include "zConfTest.h"

using namespace Test;
using namespace zUtils;


int
zConfTest_FileLoadStore( void* arg_ )
{

    // Create new configuration data item and verify
    zConf::Data *expData = new zConf::Data;
    TEST_ISNOT_NULL(expData);
    zConf::Data *obsData = new zConf::Data;
    TEST_ISNOT_NULL(obsData);

    // Create new configuration data connector and verify
    TestConnector *myConnector = new TestConnector;
    TEST_ISNOT_NULL(myConnector);

    // Attempt to load configuration and verify
    TEST_FALSE(myConnector->Load(*expData));

    // Attempt to store configuration and verify
    TEST_TRUE(myConnector->Store(*expData));

    // Attempt to load configuration and verify
    TEST_TRUE(myConnector->Load(*obsData));

    // Update configuration data
    std::string key1 = "Key1";
    std::string val1 = "Value1";
    TEST_TRUE(expData->Set(key1, val1));
    std::string key2 = "Key2";
    std::string val2 = "Value2";
    TEST_TRUE(expData->Set(key2, val2));

    // Verify data is not equal
    TEST_TRUE(*obsData != *expData);

    // Attempt to store configuration and verify
    TEST_TRUE(myConnector->Store(*expData));

    // Attempt to load configuration and verify
    TEST_TRUE(myConnector->Load(*obsData));

    myConnector->Display();

    // Verify data is equal
    TEST_TRUE(*obsData == *expData);

    // Cleanup
    delete (myConnector);
    delete (obsData);
    delete (expData);

    // Return success
    return (0);
}
