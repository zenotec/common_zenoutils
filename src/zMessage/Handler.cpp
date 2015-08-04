/*
 * Handler.cpp
 *
 *  Created on: Jan 19, 2014
 *      Author: kmahoney
 */

#include <stddef.h>

#include <zutils/zMessage.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// zMessage::Handler Class
//**********************************************************************

Handler::Handler()
{
//  ZLOG_DEBUG("zMessage::Handler::Handler(): Registering self with zSocket::Handler");
    // Register self with socket handler
    zSocket::Handler::Register(this);
}

Handler::~Handler()
{
//  ZLOG_DEBUG("zMessage::Handler::~Handler(): Unregistering self with zSocket::Handler");
    // Unregister with socket handler
    zSocket::Handler::Unregister(this);
}

bool Handler::Register(zMessage::Message::TYPE type_, zMessage::Observer* obs_)
{
//  ZLOG_DEBUG("zMessage::Handler::Register(): Registering observer with self: Type: " + zLog::IntStr(type_));
    this->_obsMap[type_].push_back(obs_);
    return (true);
}

bool Handler::Unregister(zMessage::Message::TYPE type_, zMessage::Observer* obs_)
{
//  ZLOG_DEBUG("zMessage::Handler::Unregister(): Unregistering observer with self: Type: " + zLog::IntStr(type_));
    this->_obsMap[type_].remove(obs_);
    return (true);
}

bool Handler::Send(zMessage::Message &msg_)
{
    bool status = true;

    const zSocket::Address *fromAddr = this->_sock->GetAddress();
    msg_.SetSrc(fromAddr->GetAddress());
    zSocket::Address *toAddr = zSocket::Factory::Create(fromAddr->GetType());
    toAddr->SetAddress(msg_.GetDst());

    ssize_t size = zSocket::Handler::Send(toAddr, msg_.GetJson());
    if (size != msg_.GetJson().size())
    {
        ZLOG_ERR("zMessage::Handler::Send(): Failed to send message: bytes sent: " + zLog::IntStr(size));
        status = false;
    }

    return (status);

}

bool Handler::Broadcast(zMessage::Message &msg_)
{
    bool status = true;

    const zSocket::Address *fromAddr = this->_sock->GetAddress();
    msg_.SetSrc(fromAddr->GetAddress());

    ssize_t size = zSocket::Handler::Broadcast(msg_.GetJson());
    if (size != msg_.GetJson().size())
    {
        ZLOG_ERR("zMessage::Handler::Broadcast(): Failed to broadcast message: bytes sent: " + zLog::IntStr(size));
        status = false;
    }

    return (status);

}

bool Handler::SocketRecv(zSocket::Socket *sock_, const zSocket::Address *addr_,
        zSocket::Buffer *buf_)
{
    bool status = false;

    ZLOG_INFO("zMessage::Handler::SocketRecv(): Received socket buffer");

    // Convert from socket buffer to a message
    zMessage::Message *msg = new zMessage::Message;
    if (!msg->SetJson(buf_->Str()))
    {
        ZLOG_ERR("zMessage::Handler::SocketRecv(): Error converting buffer string to zMessage");
        return (false);
    }

    // Switch on message type
    switch (msg->GetType())
    {
        case zMessage::Message::TYPE_HELLO:
        {
            // Update socket and socket address associated with this zNode
            zMessage::Message *ack = zMessage::Factory::Create(zMessage::Message::TYPE_ACK);
            ack->SetId(msg->GetId());
            ack->SetDst(msg->GetSrc());
            this->Send(*ack);
            delete(ack);
            status = true;
            break;
        }

        case zMessage::Message::TYPE_BYE:
        {
            // Remove socket and socket address associated with this zNode
            status = true;
            break;
        }

        case zMessage::Message::TYPE_ACK:
        case zMessage::Message::TYPE_AUTH:
        case zMessage::Message::TYPE_NODE:
        case zMessage::Message::TYPE_CFG:
        case zMessage::Message::TYPE_CMD:
        case zMessage::Message::TYPE_DATA:
        {
            status = true;
            break;
        }

        default:
        {
            ZLOG_ERR("zMessage::Handler::SocketRecv(): zMessage type not supported");
            break;
        }
    }

    if (status)
    {
        // Notify observers
        this->_notify(msg->GetType(), *msg);
    }

    // Return status
    return (status);
}

void Handler::_notify(zMessage::Message::TYPE type_, zMessage::Message &msg_)
{
    ZLOG_DEBUG("zMessage::Handler::_notify(): Notifying message observers: " + zLog::IntStr(type_));
    std::list<zMessage::Observer *> obsList = this->_obsMap[type_];
    std::list<zMessage::Observer *>::iterator it = obsList.begin();
    std::list<zMessage::Observer *>::iterator end = obsList.end();
    for (; it != end; ++it)
    {
        (*it)->MessageRecv(*this, msg_);
    }
}

}
}

