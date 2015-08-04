//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zNode.h
//    Description:
//
//*****************************************************************************

#ifndef _ZNODE_H_
#define _ZNODE_H_

#include <stdint.h>
#include <string>
#include <map>
#include <list>

#include <zutils/zData.h>
#include <zutils/zQueue.h>
#include <zutils/zTimer.h>
#include <zutils/zMessage.h>

namespace zUtils
{
namespace zNode
{

//**********************************************************************
// zNode::Node Class
//**********************************************************************
class Node: public zData::Data
{

    static const std::string ROOT;
    static const std::string NAME;
    static const std::string ID;
    static const std::string ADDRESS;

public:

    enum STATE
    {
        STATE_NONE = 0,
        STATE_ONLINE = 1,
        STATE_PENDING_TARDY = 2,
        STATE_TARDY = 3,
        STATE_PENDING_STALE = 4,
        STATE_STALE = 5,
        STATE_PENDING_OFFLINE = 6,
        STATE_OFFLINE = 7,
        STATE_PENDING_RETIRE = 8,
        STATE_LAST
    };

    Node( const zData::Data &node_ );

    Node();

    virtual
    ~Node();

    Node &
    operator=( const Node &other_ );

    bool
    operator==( const Node &other_ ) const;

    bool
    operator!=( const Node &other_ ) const;

    bool
    operator<( const Node &other_ ) const;

    bool
    operator>( const Node &other_ ) const;

    Node::STATE
    GetState() const;

    bool
    SetState( const Node::STATE state_ );

    std::string
    GetName() const;

    bool
    SetName( const std::string &name_ );

    std::string
    GetId() const;

    bool
    SetId( const std::string &id_ );

    std::string
    GetAddress() const;

    bool
    SetAddress( const std::string &add_ );

protected:

private:

    Node::STATE _state;

};

//**********************************************************************
// zNode::Observer Class
//**********************************************************************
class Observer
{
public:

    enum EVENT
    {
        EVENT_ERR = -1,
        EVENT_NONE = 0,
        EVENT_NEW = 1,
        EVENT_REMOVED = 2,
        EVENT_TARDY = 3,
        EVENT_STALE = 4,
        EVENT_OFFLINE = 5,
        EVENT_RETIRED = 6,
        EVENT_LAST
    };

    virtual void
    EventHandler( Observer::EVENT event_, const Node &node_ ) = 0;

protected:

private:

};

//**********************************************************************
// zNode::Table Class
//**********************************************************************
class Table: private zTimer::Observer
{
public:

    Table( const uint32_t period_ = 500 ); // msecs

    virtual
    ~Table();

    bool
    Add( zNode::Node &node_ );

    bool
    Remove( zNode::Node &node_ );

    zNode::Node *
    Find( const std::string &id_ );

    void
    Register( zNode::Observer *obsvr_ );

    void
    Unregister( zNode::Observer *obsvr_ );

protected:

    virtual void
    TimerTick();

private:

    Table( const zNode::Table &other_ );

    void
    _notify( std::list<std::pair<zNode::Observer::EVENT, zNode::Node> > notifications_ );

    zSem::Mutex _lock;

    zTimer::Timer _timer;

    std::list<zNode::Node> _nodeList;
    std::map<std::string, zNode::Node *> _idMap;

    std::list<zNode::Observer *> _observers;

};

//**********************************************************************
// zNode::Message Class
//**********************************************************************
class Message: public zMessage::Message
{

    static const std::string STR_NODE;

public:

    Message( zMessage::Message::TYPE, zNode::Node &node_ );

    Message( zMessage::Message &msg_ );

    virtual
    ~Message();

    zNode::Node
    GetNode();

    bool
    SetNode( zNode::Node &node_ );

protected:

private:

};

//**********************************************************************
// zNode::Manager Class
//**********************************************************************
class Manager: zMessage::Observer, zNode::Observer
{
public:

    Manager( zNode::Node &self_ );

    virtual
    ~Manager();

    bool
    AddMessageHandler( zMessage::Handler *handler_ );

    bool
    RemMessageHandler( zMessage::Handler *handler_ );

    bool
    Announce();

    bool
    Leave();

protected:

    virtual bool
    MessageRecv( zMessage::Handler &handler_, zMessage::Message &msg_ );

    virtual void
    EventHandler( zNode::Observer::EVENT event_, const zNode::Node &node_ );

private:

    Node _self;

    Table _nodeTable;

    std::list<zMessage::Handler *> _messageHandlers;

    bool
    _helloMsgHandler( zMessage::Handler &handler_, zNode::Message &msg_ );

    bool
    _ackMsgHandler( zMessage::Handler &handler_, zNode::Message &msg_ );

    bool
    _byeMsgHandler( zMessage::Handler &handler_, zNode::Message &msg_ );

    bool
    _nodeMsgHandler( zMessage::Handler &handler_, zNode::Message &msg_ );

};

}
}

#endif /* _ZNODE_H_ */
