/*
 * zMessage.h
 *
 *  Created on: Feb 4, 2014
 *      Author: kmahoney
 */

#ifndef ZMESSAGE_H_
#define ZMESSAGE_H_

#include "zutils/zData.h"
#include "zutils/zNode.h"

namespace zUtils
{
namespace zMessage
{

class Message: public zData::Data
{

    static const std::string ROOT;
    static const std::string ID;
    static const std::string TYPE;
    static const std::string TO;
    static const std::string FROM;
    static const std::string DATA;

public:

    Message();
    Message( const zData::Data &msg_ );
    virtual ~Message();

    std::string GetId() const;
    void SetId( const std::string &id_ );

    std::string GetType() const;
    void SetType( const std::string &type_ );

    zNode::Node GetTo() const;
    void SetTo( const zNode::Node &to_ );

    zNode::Node GetFrom() const;
    void SetFrom( const zNode::Node &from_ );

    Data GetData() const;
    void SetData( const zData::Data &data_ );

protected:

private:

};

class MessageFactory
{
public:
    MessageFactory();
    ~MessageFactory();

    Message *Create( const std::string &type_ );

protected:

private:

};

class AckMessage: public Message
{
public:
    static const std::string TYPE;
    static const std::string ID;

    AckMessage();
    ~AckMessage();

protected:

private:

};

class ByeMessage: public Message
{
public:
    static const std::string TYPE;
    ByeMessage();
    ~ByeMessage();

protected:

private:

};

class HelloMessage: public Message
{
public:
    static const std::string TYPE;
    HelloMessage();
    ~HelloMessage();

protected:

private:

};

}
}

#endif /* ZMESSAGE_H_ */

