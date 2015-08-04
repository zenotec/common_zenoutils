/*
 * Message.cpp
 *
 *  Created on: Aug 2, 2015
 *      Author: kmahoney
 */

#include <zutils/zNode.h>

namespace zUtils
{
namespace zNode
{

const std::string Message::STR_NODE = "zNode";

Message::Message( zNode::Node &node_ )
{
    this->SetType( Message::TYPE_NODE );
    this->SetNode( node_ );
}

Message::Message( zMessage::Message &msg_ )
{
    this->SetJson(msg_.GetJson());
}

Message::~Message()
{

}

zNode::Node
Message::GetNode()
{
    zData::Data data;
    this->GetData().GetChild( Message::STR_NODE, data );
    zNode::Node node( data );
    return (node);
}

bool
Message::SetNode( zNode::Node &node_ )
{
    bool status = false;
    zData::Data data = this->GetData();
    status = data.PutChild( Message::STR_NODE, node_ );
    status = this->SetData( data );
    return (status);
}

}
}
