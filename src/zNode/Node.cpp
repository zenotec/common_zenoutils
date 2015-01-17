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

namespace zUtils
{
namespace zNode
{

//**********************************************************************
// Node
//**********************************************************************

const std::string Node::ROOT = "zNode";
const std::string Node::TYPE = "Type";
const std::string Node::ID = "Id";

Node::Node(const zData::Data &node_) :
    zData::Data(node_), _tardyCnt(0)
{
}

Node::Node(const std::string &type_, const std::string &id_) :
    zData::Data(Node::ROOT), _tardyCnt(0)
{
  this->SetType(type_);
  this->SetId(id_);
  ZLOG_DEBUG("Creating new node: " + type_ + "[" + id_ + "]");
}

Node::~Node()
{
}

bool
Node::operator ==(const Node &other_) const
{
  return (this->GetId() == other_.GetId());
}

bool
Node::operator !=(const Node &other_) const
{
  return (this->GetId() != other_.GetId());
}

bool
Node::operator >(const Node &other_) const
{
  return (this->GetId() > other_.GetId());
}

bool
Node::operator <(const Node &other_) const
{
  return (this->GetId() < other_.GetId());
}

std::string
Node::GetType() const
{
  return (this->GetValue(Node::TYPE));
}

void
Node::SetType(const std::string &type_)
{
  this->SetValue(Node::TYPE, type_);
}

std::string
Node::GetId() const
{
  return (this->GetValue(Node::ID));
}

void
Node::SetId(const std::string &id_)
{
  this->SetValue(Node::ID, id_);
}

}
}

