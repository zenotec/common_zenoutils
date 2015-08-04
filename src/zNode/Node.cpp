/*
 * Node.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: kmahoney
 */

#include <errno.h>
#include <string.h>

#include <zutils/zLog.h>
#include <zutils/zNode.h>
#include <zutils/zUuid.h>

namespace zUtils
{
namespace zNode
{

//**********************************************************************
// Node
//**********************************************************************

const std::string Node::ROOT = "zNode";
const std::string Node::NAME = "Name";
const std::string Node::ID = "Id";
const std::string Node::ADDRESS = "Address";

Node::Node( const zData::Data &data_ ) :
        zData::Data( Node::ROOT ), _state( Node::STATE_NONE )
{
    this->SetId( data_.GetValue( Node::ID ) );
    this->SetName( data_.GetValue( Node::NAME ) );
    this->SetAddress( data_.GetValue( Node::ADDRESS ) );
}

Node::Node() :
        zData::Data( Node::ROOT ), _state( Node::STATE_NONE )
{
    zUuid::Uuid uuid;
    this->SetId( uuid() );
    this->SetName( "" );
    this->SetAddress( "" );
    ZLOG_DEBUG( "Creating new node: " + this->GetName() + "[" + this->GetId() + "]:"
            + this->GetAddress() );
}

Node::~Node()
{
    ZLOG_DEBUG( "Destroying node: " + this->GetName() + "[" + this->GetId() + "]:"
            + this->GetAddress() );
}

Node &
Node::operator=( const Node &other_ )
{
    this->SetName( other_.GetName() );
    this->SetId( other_.GetId() );
    this->SetAddress( other_.GetAddress() );
    return (*this);
}

bool
Node::operator ==( const Node &other_ ) const
        {
    return (this->_pt == other_._pt);
}

bool
Node::operator !=( const Node &other_ ) const
        {
    return (this->_pt != other_._pt);
}

bool
Node::operator >( const Node &other_ ) const
        {
    return ((this->GetName() != other_.GetName()) || (this->GetId() > other_.GetId()));
}

bool
Node::operator <( const Node &other_ ) const
        {
    return ((this->GetName() != other_.GetName()) || (this->GetId() < other_.GetId()));
}

Node::STATE
Node::GetState() const
{
    return (this->_state);
}

bool
Node::SetState( const Node::STATE state_ )
{
    bool status = false;
    switch (state_)
    {
        case Node::STATE_ONLINE:

        case Node::STATE_PENDING_TARDY:

        case Node::STATE_TARDY:

        case Node::STATE_PENDING_STALE:

        case Node::STATE_STALE:

        case Node::STATE_PENDING_OFFLINE:

        case Node::STATE_OFFLINE:

        case Node::STATE_PENDING_RETIRE:
            {
                status = true;
                this->_state = state_;
                break;
            }
        default:
            break;
    }
    return (status);
}

std::string
Node::GetName() const
{
    return (this->GetValue( Node::NAME ));
}

bool
Node::SetName( const std::string &name_ )
{
    return (this->SetValue( Node::NAME, name_ ));
}

std::string
Node::GetId() const
{
    return (this->GetValue( Node::ID ));
}

bool
Node::SetId( const std::string &id_ )
{
    return (this->SetValue( Node::ID, id_ ));
}

std::string
Node::GetAddress() const
{
    return (this->GetValue( Node::ADDRESS ));
}

bool
Node::SetAddress( const std::string &addr_ )
{
    return (this->SetValue( Node::ADDRESS, addr_ ));
}

}
}

