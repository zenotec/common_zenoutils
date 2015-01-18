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
const std::string Node::TYPE = "Type";
const std::string Node::ID = "Id";

Node::Node(const zData::Data &node_) :
    zData::Data(node_), _tardyCnt(0)
{
}

Node::Node(const std::string &type_) :
    zData::Data(Node::ROOT), _tardyCnt(0)
{
  zUuid::Uuid uuid;
  this->SetType(type_);
  this->SetId(uuid());
  ZLOG_DEBUG("Creating new node: " + this->GetType() + "[" + this->GetId() + "]");
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

bool
Node::SetType(const std::string &type_)
{
  return (this->SetValue(Node::TYPE, type_));
}

std::string
Node::GetId() const
{
  return (this->GetValue(Node::ID));
}

bool
Node::SetId(const std::string &id_)
{
  return (this->SetValue(Node::ID, id_));
}

}
}

