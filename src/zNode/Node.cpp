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

const std::string Node::BCAST_NAME = "ALL";
const std::string Node::BCAST_ID = "FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF";

Node::Node(const zData::Data &data_) :
    _tardyCnt(0)
{
  this->SetName(data_.GetValue(Node::NAME));
  this->SetId(data_.GetValue(Node::ID));
}

Node::Node(const std::string &name_) :
    zData::Data(Node::ROOT), _tardyCnt(0)
{
  zUuid::Uuid uuid;
  this->SetId(uuid());
  this->SetName(name_);
  ZLOG_DEBUG("Creating new node: " + this->GetName() + "[" + this->GetId() + "]");
}

Node::~Node()
{
  ZLOG_DEBUG("Destroying node: " + this->GetName() + "[" + this->GetId() + "]");
}

Node &
Node::operator=(const Node &other_)
{
  this->SetName(other_.GetName());
  this->SetId(other_.GetId());
  return(*this);
}

bool
Node::operator ==(const Node &other_) const
{
  return (this->_pt == other_._pt);
}

bool
Node::operator !=(const Node &other_) const
{
  return (this->_pt != other_._pt);
}

bool
Node::operator >(const Node &other_) const
{
  return ((this->GetName() != other_.GetName()) || (this->GetId() > other_.GetId()));
}

bool
Node::operator <(const Node &other_) const
{
  return ((this->GetName() != other_.GetName()) || (this->GetId() < other_.GetId()));
}

std::string
Node::GetName() const
{
  return (this->GetValue(Node::NAME));
}

bool
Node::SetName(const std::string &name_)
{
  return (this->SetValue(Node::NAME, name_));
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

