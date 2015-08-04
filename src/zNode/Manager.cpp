/*
 * Manager.cpp
 *
 *  Created on: Aug 2, 2015
 *      Author: kmahoney
 */

#include <zutils/zNode.h>

namespace zUtils
{
namespace zNode
{

//**********************************************************************
// Manager
//**********************************************************************

Manager::Manager( zNode::Node &self_ ) :
        _nodeTable( 30000 )
{
    this->_self = self_;
    this->_nodeTable.Register( this );
    this->_nodeTable.Add( this->_self );
}

Manager::~Manager()
{
    this->_nodeTable.Remove( this->_self );
    this->_nodeTable.Unregister( this );
    std::list<zMessage::Handler *>::iterator it = this->_messageHandlers.begin();
    std::list<zMessage::Handler *>::iterator end = this->_messageHandlers.end();
    for (; it != end; ++it)
    {
        (*it)->Unregister( zMessage::Message::TYPE_HELLO, this );
        (*it)->Unregister( zMessage::Message::TYPE_BYE, this );
        (*it)->Unregister( zMessage::Message::TYPE_ACK, this );
        (*it)->Unregister( zMessage::Message::TYPE_NODE, this );
    }
    this->_messageHandlers.clear();
}

bool
Manager::AddMessageHandler( zMessage::Handler *handler_ )
{
    bool status = false;

    // Add message handler to list
    this->_messageHandlers.push_front( handler_ );

    // Register with handler for messages of interest
    handler_->Register( zMessage::Message::TYPE_HELLO, this );
    handler_->Register( zMessage::Message::TYPE_BYE, this );
    handler_->Register( zMessage::Message::TYPE_ACK, this );
    handler_->Register( zMessage::Message::TYPE_NODE, this );
    handler_->StartListener( 1000 );

    // Say hello to everyone on network
    zNode::Message *HelloMsg = new zNode::Message( zMessage::Message::TYPE_HELLO, this->_self );
    status = handler_->Broadcast( *HelloMsg );
    delete (HelloMsg);

    if (!status)
    {
        ZLOG_ERR( "zNode::Manger::AddMessageHandler(): Error broadcasting hello message" );
    }

    // Return status
    return (status);

}

bool
Manager::RemMessageHandler( zMessage::Handler *handler_ )
{
    bool status = false;

    // Say goodbye to everyone on network
    zNode::Message *ByeMsg = new zNode::Message( zMessage::Message::TYPE_BYE, this->_self );
    status = handler_->Broadcast( *ByeMsg );
    delete (ByeMsg);

    if (!status)
    {
        ZLOG_ERR( "zNode::Manger::RemMessageHandler(): Error broadcasting bye message" );
    }

    // Unregister for all messages
    handler_->StopListener( 1000 );
    handler_->Unregister( zMessage::Message::TYPE_HELLO, this );
    handler_->Unregister( zMessage::Message::TYPE_BYE, this );
    handler_->Unregister( zMessage::Message::TYPE_ACK, this );
    handler_->Unregister( zMessage::Message::TYPE_NODE, this );

    // Remove handler from list
    this->_messageHandlers.remove( handler_ );

    // Return status
    return (status);

}

bool
Manager::Announce()
{
    zNode::Node bcast;
    bcast.SetId( "0" );
    bcast.SetName( "all" );

    zNode::Message *HelloMsg = new zNode::Message( zMessage::Message::TYPE_HELLO, this->_self );

    std::list<zMessage::Handler *>::iterator it = this->_messageHandlers.begin();
    std::list<zMessage::Handler *>::iterator end = this->_messageHandlers.end();
    for (; it != end; ++it)
    {
        if (!(*it)->Broadcast( *HelloMsg ))
        {
            ZLOG_ERR( "zNode::Manger::Announce(): Error sending hello message: " );
        }
    }
    delete (HelloMsg);
}

bool
Manager::Leave()
{
    zNode::Node bcast;
    bcast.SetId( "0" );
    bcast.SetName( "all" );

    zMessage::Message *ByeMsg = zMessage::Factory::Create( zMessage::Message::TYPE_BYE );

    std::list<zMessage::Handler *>::iterator it = this->_messageHandlers.begin();
    std::list<zMessage::Handler *>::iterator end = this->_messageHandlers.end();
    for (; it != end; ++it)
    {
        if (!(*it)->Broadcast( *ByeMsg ))
        {
            ZLOG_ERR( "zNode::Manger::Leave(): Error sending bye message: " );
        }
    }
    delete (ByeMsg);
}

bool
Manager::MessageRecv( zMessage::Handler &handler_, zMessage::Message &msg_ )
{
    bool status = false;

    ZLOG_INFO( "zNode::Manager::MessageRecv(): Received message: " +
            msg_.GetSrc() + " -> " + msg_.GetDst() );

    // Convert message to node message
    zNode::Message *msg = new zNode::Message( msg_ );

    switch (msg_.GetType())
    {
        case zMessage::Message::TYPE_HELLO:
            status = this->_helloMsgHandler( handler_, *msg );
            break;
        case zMessage::Message::TYPE_BYE:
            status = this->_byeMsgHandler( handler_, *msg );
            break;
        case zMessage::Message::TYPE_ACK:
            status = this->_ackMsgHandler( handler_, *msg );
            break;
        case zMessage::Message::TYPE_NODE:
            status = this->_nodeMsgHandler( handler_, *msg );
            break;
        default:
            ZLOG_ERR( "Received message of unknown type: " + ZLOG_INT( msg_.GetType() ) )
            break;
    }

    // Clean up
    delete (msg);

    // Return status
    return (status);

}

void
Manager::EventHandler( zNode::Observer::EVENT event_, const zNode::Node &node_ )
{
    switch (event_)
    {
        case zNode::Observer::EVENT_NEW:
            std::cout << "New node: " << node_.GetName() << "[" << node_.GetId() << "]: "
                    << node_.GetAddress() << std::endl;
            break;

        case zNode::Observer::EVENT_REMOVED:
            std::cout << "Removed node: " << node_.GetName() << "[" << node_.GetId() << "]: "
                    << node_.GetAddress() << std::endl;
            break;

        case zNode::Observer::EVENT_TARDY:
            std::cout << "Tardy node: " << node_.GetName() << "[" << node_.GetId() << "]: "
                    << node_.GetAddress() << std::endl;
            break;

        case zNode::Observer::EVENT_STALE:
            std::cout << "Stale node: " << node_.GetName() << "[" << node_.GetId() << "]: "
                    << node_.GetAddress() << std::endl;
            break;

        case zNode::Observer::EVENT_OFFLINE:
            std::cout << "Offline node: " << node_.GetName() << "[" << node_.GetId() << "]: "
                    << node_.GetAddress() << std::endl;
            break;

        case zNode::Observer::EVENT_RETIRED:
            std::cout << "Retired node: " << node_.GetName() << "[" << node_.GetId() << "]: "
                    << node_.GetAddress() << std::endl;
            break;

        default:
            std::cout << "Unknown node: " << node_.GetName() << "[" << node_.GetId() << "]: "
                    << node_.GetAddress() << std::endl;
            break;
    }

    std::cout.flush();
}

bool
Manager::_helloMsgHandler( zMessage::Handler &handler_, zNode::Message &msg_ )
{
    bool status = false;

    // Lookup node identifier in table
    if (!msg_.GetNode().GetId().empty())
    {
        zNode::Node *node = this->_nodeTable.Find( msg_.GetNode().GetId() );
        if (!node)
        {
            // Valid node, does not exist in table so add it
            node = new zNode::Node( msg_.GetNode() );
            status = this->_nodeTable.Add( *node );
            delete (node);
        }
        else
        {
            // Valid node, already exists in the table so update its state to online
            status = node->SetState( zNode::Node::STATE_ONLINE );
        }
    }

    // Return status
    return (status);
}

bool
Manager::_ackMsgHandler( zMessage::Handler &handler_, zNode::Message &msg_ )
{

}

bool
Manager::_byeMsgHandler( zMessage::Handler &handler_, zNode::Message &msg_ )
{
    bool status = false;

    // Lookup node identifier in table
    if (!msg_.GetNode().GetId().empty())
    {
        zNode::Node *node = this->_nodeTable.Find( msg_.GetNode().GetId() );
        if (node)
        {
            // Valid node, remove it from the table
            this->_nodeTable.Remove(*node);
        }
    }

    // Return status
    return (status);
}

bool
Manager::_nodeMsgHandler( zMessage::Handler &handler_, zNode::Message &msg_ )
{
    bool status = false;

    // Lookup node identifier in table
    if (!msg_.GetNode().GetId().empty())
    {
        zNode::Node *node = this->_nodeTable.Find( msg_.GetNode().GetId() );
        if (!node)
        {
            // Valid node, does not exist in table so add it
            node = new zNode::Node( msg_.GetNode() );
            status = this->_nodeTable.Add( *node );
            delete (node);
        }
        else
        {
            // Valid node, already exists in the table so update its state to online
            status = node->SetState( zNode::Node::STATE_ONLINE );
        }
    }

    // Return status
    return (status);
}

}
}
