/*
 * Array.cpp
 *
 *  Created on: Aug 28, 2015
 *      Author: kmahoney
 */

#include "zDataTest.h"

using namespace zUtils;
using namespace Test;

int
zDataTest_Array( void* arg )
{

    ZLOG_DEBUG( "#############################################################" );
    ZLOG_DEBUG( "# zDataTest_Array()" );
    ZLOG_DEBUG( "#############################################################" );

    const int num = 4;
    std::string key, val, obs;

    // Create new data item and validate
    zData::Data *myParent = new zData::Data( std::string( "parent" ) );
    TEST_ISNOT_NULL( myParent );

    // Create new data array and validate
    zData::Data *myChild;

    for (int i = 0; i < num; i++)
    {
        myChild = new zData::Data( std::string( "child" ) );
        TEST_ISNOT_NULL( myChild );
        key = std::string( "key" );
        val = std::string( "value" ) + zLog::IntStr( i );
        myChild->SetValue( key, val );
        std::cout << std::endl << myChild->GetJson() << std::endl;
        TEST_TRUE( myParent->AddChild( std::string( "child" ), *myChild ) );
        delete (myChild);
    }
    std::cout << std::endl << myParent->GetJson() << std::endl;

    // Parse data array and validate
    myChild = new zData::Data( std::string( "child" ) );
    TEST_ISNOT_NULL( myChild );
    TEST_TRUE( myParent->GetChild( std::string( "child" ), *myChild ) );
    std::cout << std::endl << myChild->GetJson() << std::endl;

    delete (myChild);

    // Cleanup
    if (myParent)
        delete (myParent);

    // Return success
    return (0);

}
