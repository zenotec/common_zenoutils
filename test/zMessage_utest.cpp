#include "UnitTest.h"
#include "zutils/zMessage.h"

using namespace Test;
using namespace zUtils;

static int UnitTestMessageDefaults( int arg_ )
{

    // Create new node and validate
    zMessage *MyMessage = new zMessage();
    TEST_EQ( MyMessage->GetId(), "" );
    TEST_EQ( MyMessage->GetType(), std::string( "" ) );
    TEST_EQ( MyMessage->GetTo().GetId(), "" );
    TEST_EQ( MyMessage->GetFrom().GetId(), "" );

    // Cleanup
    delete (MyMessage);

    // Return success
    return (0);

}

static int UnitTestMessageMethods( int arg_ )
{
    std::string id;
    std::string type;
    zNode to;
    zNode from;
    zData data;

    // Create new node and validate
    zMessage *MyMessage = new zMessage();
    TEST_EQ( MyMessage->GetId(), std::string( "" ) );
    TEST_EQ( MyMessage->GetType(), std::string( "" ) );
    TEST_EQ( MyMessage->GetTo().GetId(), to.GetId() );
    TEST_EQ( MyMessage->GetFrom().GetId(), from.GetId() );

    // Test setting "id" field
    id = "0123456789";
    MyMessage->SetId( id );
    TEST_EQ( MyMessage->GetId(), id );

    // Test setting "type" field
    type = "test";
    MyMessage->SetType( type );
    TEST_EQ( MyMessage->GetType(), type );

    // Test setting "to" field
    to.SetId("toId");
    MyMessage->SetTo( to );
    TEST_EQ( MyMessage->GetTo().GetId(), to.GetId() );

    // Test setting "from" field
    from.SetId("fromId");
    MyMessage->SetFrom( from );
    TEST_EQ( MyMessage->GetFrom().GetId(), from.GetId() );

    // Test setting "data" field
    data = MyMessage->GetData();
    data.SetVal("Key", "Value");
    MyMessage->SetData( data );

    // Cleanup
    delete (MyMessage);

    // Return success
    return (0);

}

static int UnitTestAckMessageDefaults( int arg_ )
{

    // Create new node and validate
    zMessage *MyMessage = new AckMessage();
    TEST_EQ( MyMessage->GetId(), "" );
    TEST_EQ( MyMessage->GetType(), AckMessage::TYPE );
    TEST_EQ( MyMessage->GetTo().GetId(), "" );
    TEST_EQ( MyMessage->GetFrom().GetId(), "" );

    // Cleanup
    delete (MyMessage);

    // Return success
    return (0);

}

int zMessage_utest( void )
{
    INIT();
    UTEST( UnitTestMessageDefaults, 0 );
    UTEST( UnitTestMessageMethods, 0 );
    UTEST( UnitTestAckMessageDefaults, 0 );
    FINI();
}

