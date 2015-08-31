/*
 * Data.cpp
 *
 *  Created on: Aug 27, 2015
 *      Author: kmahoney
 */

#include "UnitTest.h"
#include "zConfTest.h"

using namespace Test;
using namespace zUtils;

int
zConfTest_DataGetSetKeyValue( void* arg_ )
{
    std::string obs;

    // Create new configuration data item and verify
    zConf::Data *myData = new zConf::Data;
    TEST_ISNOT_NULL( myData );

    std::string key1 = "key1";
    std::string val1 = "value1";

    TEST_FALSE( myData->Get( key1, obs ) );

    TEST_TRUE( myData->Set( key1, val1 ) );
    TEST_TRUE( myData->Get( key1, obs ) );
    TEST_EQ( obs, val1 );

    // Cleanup
    delete (myData);

    // Return success
    return (0);
}

int
zConfTest_DataGetSetData( void* arg_ )
{
    std::string obs;

    // Create new data item and verify
    zData::Data *myData = new zData::Data("zConf");
    TEST_ISNOT_NULL( myData );

    // Create new configuration data item and verify
    zConf::Data *myConfData = new zConf::Data;
    TEST_ISNOT_NULL( myConfData );

    // Setup data item and verify
    std::string key1 = "key1";
    std::string val1 = "value1";
    myData->SetValue(key1, val1);
    obs = myData->GetValue(key1);
    TEST_EQ(obs, val1);

    // Verify the key doesn't exist and isn't set
    TEST_FALSE( myConfData->Get( key1, obs ) );

    // Set the data and verify (this replaces the entire data set)
    TEST_TRUE( myConfData->Set( *myData ) );
    TEST_TRUE( myConfData->Get( key1, obs ) );
    TEST_EQ( obs, val1 );

    // Cleanup
    delete (myConfData);

    // Return success
    return (0);
}

int
zConfTest_DataCompare( void* arg_ )
{

    // Create new configuration data item and verify
    zConf::Data *myData1 = new zConf::Data;
    TEST_ISNOT_NULL( myData1 );

    // Create second configuration data item and verify
    zConf::Data *myData2 = new zConf::Data;
    TEST_ISNOT_NULL( myData2 );

    // Verify both are equal
    TEST_TRUE( *myData1 == *myData2 );
    TEST_FALSE( *myData1 != *myData2 );

    std::string key = "key";
    std::string val = "value";

    // Modify first configuration data item
    myData1->Set( key, val );

    // Verify they are no longer equal
    TEST_FALSE( *myData1 == *myData2 );
    TEST_TRUE( *myData1 != *myData2 );

    // Modify second configuration data item
    myData2->Set( key, val );

    // Verify both are equal again
    TEST_TRUE( *myData1 == *myData2 );
    TEST_FALSE( *myData1 != *myData2 );

    // Cleanup
    delete (myData2);
    delete (myData1);

    // Return success
    return (0);
}

int
zConfTest_DataLoadStore( void* arg_ )
{

    std::string obs;

    // Create new configuration data item and verify
    zConf::Data *myData = new zConf::Data;
    TEST_ISNOT_NULL(myData);

    // Create new configuration data connector and verify
    TestConnector *myConnector = new TestConnector;
    TEST_ISNOT_NULL(myConnector);

    // Attempt to load configuration and verify
    TEST_FALSE(myData->Load());

    // Attempt to store configuration and verify
    TEST_FALSE(myData->Commit());

    // Connect data to file connector
    TEST_TRUE(myData->Connect(myConnector));

    // Attempt to load configuration and verify
    TEST_FALSE(myData->Load());

    // Update configuration data and verify
    std::string key = "Key";
    std::string val1 = "Value1";
    std::string val2 = "Value2";
    TEST_TRUE(myData->Set(key, val1));
    TEST_TRUE(myData->Get(key, obs));
    TEST_EQ(obs, val1);

    // Store configuration and verify
    TEST_TRUE(myData->Commit());

    // Modify data and verify
    TEST_TRUE(myData->Set(key, val2));
    TEST_TRUE(myData->Get(key, obs));
    TEST_EQ(obs, val2);

    // Load configuration and verify
    TEST_TRUE(myData->Load());
    TEST_TRUE(myData->Get(key, obs));
    TEST_EQ(obs, val1);

    // Cleanup
    delete (myConnector);
    delete (myData);

    // Return success
    return (0);
}
