#include "zDataTest.h"

using namespace zUtils;

int
zDataTest_Defaults( void* arg )
{

    ZLOG_DEBUG( "#############################################################" );
    ZLOG_DEBUG( "# zDataTest_Defaults()" );
    ZLOG_DEBUG( "#############################################################" );

    std::string key;
    zData::Data *MyData;

    // Create new data object and validate
    key = "";
    MyData = new zData::Data;
    TEST_EQ( MyData->GetKey(), key );
    TEST_EQ( MyData->GetJson(), std::string( "{\n    \"Key\": \"\"\n}\n" ) );
    TEST_EQ( MyData->GetXml(),
            std::string( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key/>" ) );
    delete (MyData);

    // Create new data object and validate
    key = "TestKey";
    MyData = new zData::Data( key );
    TEST_EQ( MyData->GetKey(), key );
    TEST_EQ( MyData->GetJson(),
            std::string( "{\n    \"Key\": \"TestKey\",\n    \"TestKey\": \"\"\n}\n" ) );
    TEST_EQ( MyData->GetXml(),
            std::string(
                    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>TestKey</Key><TestKey/>" ) );
    delete (MyData);

    // Return success
    return (0);

}

