#include "UnitTest.h"
#include "zNode.h"

using namespace std;
using namespace Test;
using namespace zUtils;

static int UnitTestNodeDefaults( int arg_ )
{

    // Create new node and validate
    zNode MyNode;
    TEST_EQ( MyNode.GetName(), string( "" ) );
    TEST_EQ( MyNode.GetId(), string( "" ) );
    TEST_EQ( MyNode.GetAddress(), string( "" ) );
    TEST_EQ( MyNode.GetTardyCnt(), 0 );

    // Return success
    return (0);

}

static int UnitTestNodeMethods( int arg_ )
{

    // Create new node and validate
    zNode *MyNode = new zNode();
    TEST_EQ( MyNode->GetName(), string( "" ) );
    TEST_EQ( MyNode->GetId(), string( "" ) );
    TEST_EQ( MyNode->GetAddress(), string( "" ) );
    TEST_EQ( MyNode->GetTardyCnt(), 0 );

    // Test setting name
    string exp("");
    TEST_EQ( MyNode->GetName(), exp );
    exp = "Name";
    MyNode->SetName( exp );
    TEST_EQ( MyNode->GetName(), exp );

    // Test setting identifier
    exp = "";
    TEST_EQ( MyNode->GetId(), exp );
    exp = "Identifier";
    MyNode->SetId( exp );
    TEST_EQ( MyNode->GetId(), exp );

    // Test setting address
    exp = "";
    TEST_EQ( MyNode->GetAddress(), exp );
    exp = "Address";
    MyNode->SetAddress( exp );
    TEST_EQ( MyNode->GetAddress(), exp );

    // Test incrementing tardy count
    TEST_EQ( MyNode->GetTardyCnt(), 0 );
    MyNode->SetTardyCnt( MyNode->GetTardyCnt() + 1 );
    TEST_EQ( MyNode->GetTardyCnt(), 1 );
    MyNode->SetTardyCnt( MyNode->GetTardyCnt() + 1 );
    TEST_EQ( MyNode->GetTardyCnt(), 2 );


    // Cleanup
    delete (MyNode);

    // Return success
    return (0);
}

static int UnitTestNodeTableDefaults( int arg_ )
{

    // Create new node table and validate
    zNodeTable *MyNodeTable = new zNodeTable;

    // Cleanup
    delete (MyNodeTable);

    // Return success
    return (0);

}

static int UnitTestNodeTableMethods( int arg_ )
{

    // Create new node and validate
    zNode *MyNode = new zNode();
    TEST_EQ( MyNode->GetName(), "" );
    TEST_EQ( MyNode->GetId(), "" );
    TEST_EQ( MyNode->GetAddress(), "" );
    TEST_EQ( MyNode->GetTardyCnt(), 0 );

    // Initialize node
    MyNode->SetName( "TestNode" );
    TEST_EQ( MyNode->GetName(), "TestNode" );
    MyNode->SetId( "abcdef56789");
    TEST_EQ( MyNode->GetId(), "abcdef56789" );
    MyNode->SetAddress( "1.1.1.1:8888");
    TEST_EQ( MyNode->GetAddress(), "1.1.1.1:8888" );
    MyNode->SetTardyCnt( 3 );
    TEST_EQ( MyNode->GetTardyCnt(), 3 );

    // Create new node table and validate
    zNodeTable *MyNodeTable = new zNodeTable();

    // Update node
    MyNodeTable->UpdateNode( *MyNode );

    // Wait for safe insertion
    sleep(1);

    // Find node
    TEST_TRUE( MyNodeTable->FindNode( MyNode->GetName() ) );

    // Wait for node to expire
    sleep(4);

    // Find node
    TEST_FALSE( MyNodeTable->FindNode( MyNode->GetName() ) );

    // Update node
    MyNodeTable->UpdateNode( *MyNode );

    // Wait for safe insertion
    sleep(1);

    // Find node
    TEST_TRUE( MyNodeTable->FindNode( MyNode->GetName() ) );

    // Remove node
    MyNodeTable->RemoveNode( *MyNode );

    // Wait for safe removal
    sleep(1);

    // Find node
    TEST_FALSE( MyNodeTable->FindNode( MyNode->GetName() ) );

    // Cleanup
    delete (MyNodeTable);
    delete (MyNode);

    // Return success
    return (0);
}

int zNode_utest( void )
{

    INIT();
    UTEST( UnitTestNodeDefaults, 0 );
    UTEST( UnitTestNodeMethods, 0 );
    UTEST( UnitTestNodeTableDefaults, 0 );
    UTEST( UnitTestNodeTableMethods, 0 );
    FINI();

}

