#include "zutils/zData.h"
#include "UnitTest.h"
#include "zDataTest.h"

using namespace zUtils;

int
zDataTest_JsonSimple( void* arg_ )
{

    ZLOG_DEBUG( "#############################################################" );
    ZLOG_DEBUG( "# zDataTest_JsonSimple()" );
    ZLOG_DEBUG( "#############################################################" );

    std::string key( "TestKey" );
    std::string name( "TestName" );
    std::string val( "TestValue" );

    // Create new data object and validate
    zData::Data MyData1( key );
    TEST_EQ( MyData1.GetKey(), key );
    TEST_EQ( MyData1.GetJson(),
            std::string( "{\n    \"Key\": \"TestKey\",\n    \"TestKey\": \"\"\n}\n" ) );
    TEST_EQ( MyData1.GetXml(),
            std::string(
                    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>TestKey</Key><TestKey/>" ) );

    // Set value and validate
    MyData1.SetValue( name, val );
    TEST_EQ( MyData1.GetValue( name ), val );

    // Create second empty data object and validate
    zData::Data MyData2;
    TEST_EQ( MyData2.GetKey(), "" );
    TEST_EQ( MyData2.GetJson(), std::string( "{\n    \"Key\": \"\"\n}\n" ) );
    TEST_EQ( MyData2.GetXml(),
            std::string( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key/>" ) );

    // Set the second data object from the first using only JSON
    MyData2.SetJson( MyData1.GetJson() );
    TEST_EQ( MyData2.GetKey(), key );
    TEST_EQ( MyData2.GetValue( name ), val );
    TEST_EQ( MyData2.GetJson(), MyData1.GetJson() );
    TEST_EQ( MyData2.GetXml(), MyData1.GetXml() );

    // Return success
    return (0);

}
