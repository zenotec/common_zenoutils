/*
 * Message.cpp
 *
 *  Created on: Jan 19, 2014
 *      Author: kmahoney
 */

#include "zutils/zMessage.h"

namespace zUtils
{

//**********************************************************************
// Message
//**********************************************************************

const std::string zMessage::ROOT = "zMessage";
const std::string zMessage::ID = "Id";
const std::string zMessage::TYPE = "Type";
const std::string zMessage::TO = "To";
const std::string zMessage::FROM = "From";
const std::string zMessage::DATA = "Data";

zMessage::zMessage() :
        zData( zMessage::ROOT )
{
    zData data(zMessage::DATA);
    zNode node;

//    std::cout << std::endl << "Message::Message(): " << std::endl << this->GetJson() << std::endl;

    // Initialize message
    this->SetId( "" );
    this->SetType( "" );
    this->SetTo( node );
    this->SetFrom( node );
    this->SetData( data );

//    std::cout << "Message::Message(): " << std::endl << this->GetJson() << std::endl;
}

zMessage::zMessage( const zData &msg_ ) :
        zData( msg_ )
{
}

zMessage::~zMessage()
{
}

std::string zMessage::GetId() const
{
    return (this->GetVal( zMessage::ID ));
}

void zMessage::SetId( const std::string &id_ )
{
    this->SetVal( zMessage::ID, id_ );
}

std::string zMessage::GetType() const
{
    return (this->GetVal( zMessage::TYPE ));
}

void zMessage::SetType( const std::string &type_ )
{
    this->SetVal( zMessage::TYPE, type_ );
}

zNode zMessage::GetTo() const
{
    zNode to;
    this->GetChild( zMessage::TO, to );
    return ( to );
}

void zMessage::SetTo( const zNode &to_ )
{
//    std::cout << std::endl << "Message::SetTo(): " << std::endl << this->GetJson() << std::endl;
    this->PutChild( zMessage::TO, to_ );
//    std::cout << "Message::SetTo(): " << std::endl << this->GetJson() << std::endl;
}

zNode zMessage::GetFrom() const
{
    zNode from;
    this->GetChild( zMessage::FROM, from );
    return ( from );
}

void zMessage::SetFrom( const zNode &from_ )
{
//    std::cout << std::endl << "Message::SetFrom(): " << std::endl << this->GetJson() << std::endl;
    this->PutChild( zMessage::FROM, from_ );
//    std::cout << "Message::SetFrom(): " << std::endl << this->GetJson() << std::endl;
}

zData zMessage::GetData() const
{
    zData data(zMessage::DATA);
    this->GetChild( zMessage::DATA, data );
    return ( data );
}

void zMessage::SetData( const zData &data_ )
{
//    std::cout << std::endl << "Message::SetData(): " << std::endl << this->GetJson() << std::endl;
    this->PutChild( zMessage::DATA, data_ );
//    std::cout << "Message::SetData(): " << std::endl << this->GetJson() << std::endl;
}

//**********************************************************************
// MessageFactory
//**********************************************************************

MessageFactory::MessageFactory()
{
}

MessageFactory::~MessageFactory()
{
}

zMessage *MessageFactory::Create( const std::string &type_ )
{
    if( type_ == HelloMessage::TYPE )
    {
        return( new HelloMessage() );
    } // end if
    else if( type_ == AckMessage::TYPE )
    {
        return( new AckMessage() );
    } // end else if
    else if( type_ == ByeMessage::TYPE )
    {
        return( new ByeMessage() );
    } // end else if
    else
    {
        return( NULL );
    } // end else
}


}

