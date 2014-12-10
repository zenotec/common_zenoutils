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

class zMessage: public zData
{

    static const std::string ROOT;
    static const std::string ID;
    static const std::string TYPE;
    static const std::string TO;
    static const std::string FROM;
    static const std::string DATA;

public:

    zMessage();
    zMessage( const zData &msg_ );
    ~zMessage();

    std::string GetId() const;
    void SetId( const std::string &id_ );

    std::string GetType() const;
    void SetType( const std::string &type_ );

    zNode GetTo() const;
    void SetTo( const zNode &to_ );

    zNode GetFrom() const;
    void SetFrom( const zNode &from_ );

    zData GetData() const;
    void SetData( const zData &data_ );

protected:

private:

};

class MessageFactory
{
public:
    MessageFactory();
    ~MessageFactory();

    zMessage *Create( const std::string &type_ );

protected:

private:

};

class AckMessage: public zMessage
{
public:
    static const std::string TYPE;
    static const std::string ID;

    AckMessage();
    ~AckMessage();

protected:

private:

};

class ByeMessage: public zMessage
{
public:
    static const std::string TYPE;
    ByeMessage();
    ~ByeMessage();

protected:

private:

};

class HelloMessage: public zMessage
{
public:
    static const std::string TYPE;
    HelloMessage();
    ~HelloMessage();

protected:

private:

};

}

#endif /* ZMESSAGE_H_ */

