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

    static const std::string STR_ROOT;
    static const std::string STR_TYPE;
    static const std::string STR_HELLO;
    static const std::string STR_ACK;
    static const std::string STR_BYE;
    static const std::string STR_CFG;
    static const std::string STR_CMD;
    static const std::string STR_ID;
    static const std::string STR_TO;
    static const std::string STR_FROM;
    static const std::string STR_DATA;

public:

    enum TYPE
    {
        TYPE_ERR = -1,
        TYPE_NONE = 0,
        TYPE_HELLO = 1,
        TYPE_ACK = 2,
        TYPE_BYE = 3,
        TYPE_CFG = 4,
        TYPE_CMD = 5,
        TYPE_LAST
    };

    Message();
    Message( const zData::Data &msg_ );
    virtual ~Message();

    Message::TYPE GetType() const;
    bool SetType( const Message::TYPE &type_ );

    std::string GetId() const;
    bool SetId( const std::string &id_ );

    zNode::Node GetTo() const;
    bool SetTo( const zNode::Node &to_ );

    zNode::Node GetFrom() const;
    bool SetFrom( const zNode::Node &from_ );

    zData::Data GetData() const;
    bool SetData( const zData::Data &data_ );

protected:

private:

};

class Factory
{
public:

    static Message *Create( const Message::TYPE &type_ );

protected:

private:

};

}
}

#endif /* ZMESSAGE_H_ */

