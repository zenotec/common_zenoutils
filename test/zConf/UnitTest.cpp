#include "zutils/zLog.h"
#include "zConfTest.h"
#include "UnitTest.h"

int
main( int argc, const char **argv )
{

    // Setup logging for testing
    zUtils::zLog::FileConnector conn( "zConfTest.zlog" );
    zUtils::zLog::Log::Instance().RegisterConnector( zUtils::zLog::CRIT, &conn );
    zUtils::zLog::Log::Instance().RegisterConnector( zUtils::zLog::ERROR, &conn );
    zUtils::zLog::Log::Instance().RegisterConnector( zUtils::zLog::WARN, &conn );
    zUtils::zLog::Log::Instance().RegisterConnector( zUtils::zLog::INFO, &conn );
    zUtils::zLog::Log::Instance().RegisterConnector( zUtils::zLog::DBG, &conn );
    zUtils::zLog::Log::Instance().SetMaxLevel( zUtils::zLog::INFO );

    // Test all classes
    UTEST_INIT();
    UTEST_TEST( zConfTest_DataDefaults, 0 );
    UTEST_TEST( zConfTest_ConnectorDefaults, 0 );
    UTEST_TEST( zConfTest_DataGetSetKeyValue, 0 );
    UTEST_TEST( zConfTest_DataGetSetData, 0 );
    UTEST_TEST( zConfTest_DataCompare, 0 );
    UTEST_TEST( zConfTest_FileLoadStore, 0 );
    UTEST_TEST( zConfTest_DataLoadStore, 0 );
    UTEST_FINI();

    // Exit
    exit( 0 );

}
