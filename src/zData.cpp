/*
 * Data.cpp
 *
 *  Created on: May 8, 2014
 *      Author: kmahoney
 */

#include "zutils/zData.h"

namespace zUtils
{

//**********************************************************************
// Data Class
//**********************************************************************

const std::string zData::ROOT = "zData";
const std::string zData::KEY = "Key";

zData::zData(const std::string &key_)
{
  this->_setKey(key_);
  this->_pt.put(key_, "");
}

zData::~zData()
{
}

std::string
zData::GetKey() const
{
  std::string key;
  try
  {
    key = this->_pt.get < std::string > (zData::KEY);
  }
  catch(boost::property_tree::ptree_bad_path const &e )
  {
    key = "";
  }
  return (key);
}

void
zData::_setKey(const std::string &key_)
{
  try
  {
    this->_pt.put(zData::KEY, key_);
  }
  catch(boost::property_tree::ptree_bad_path const &e )
  {
    return;
  }
}

std::string
zData::GetVal(const std::string &name_) const
{
  std::string name = this->GetKey() + "." + name_;
  return (this->_pt.get < std::string > (name));
}

boost::property_tree::ptree
zData::_getVal(const std::string &name_) const
{
  return (this->_pt.get_child(name_));
}

void
zData::SetVal(const std::string &name_, const std::string &value_)
{
  std::string name = this->GetKey() + "." + name_;
  this->_pt.put(name, value_);
}

void
zData::_setVal(const std::string &name_, const boost::property_tree::ptree &pt_)
{
  this->_pt.put_child(name_, pt_);
}

void
zData::GetChild(const std::string &name_, zData &child_) const
{
//    std::cout << std::endl << "Data::GetChild(): " << std::endl << this->GetJson() << std::endl;
//    std::cout << std::endl << child_.GetJson();
  std::string name = this->GetKey() + "." + name_;
  child_._setKey(name_);
  child_._setVal(name_, this->_pt.get_child(name));
//    std::cout << std::endl << "Data::GetChild(): " << std::endl << child_.GetJson() << std::endl;
}

void
zData::PutChild(const std::string &name_, const zData &child_)
{
//    std::cout << std::endl << "Data::PutChild(): " << std::endl << this->GetJson() << std::endl;
//    std::cout << std::endl << child_.GetJson();
  std::string name = this->GetKey() + "." + name_;
  this->_pt.put_child(name, child_._getVal(child_.GetKey()));
//    std::cout << std::endl << "Data::PutChild(): " << std::endl << this->GetJson() << std::endl;
}

void
zData::AddChild(const std::string &key_, const zData &child_)
{
  std::cout << std::endl << "Data::AddChild(): " << std::endl << this->GetJson() << std::endl;
  std::cout << std::endl << child_.GetJson();
  std::string key = this->GetKey() + "." + key_;
  boost::property_tree::ptree pt = child_._getVal(child_.GetKey());
  this->_pt.add_child(key, pt);
  std::cout << std::endl << "Data::AddChild(): " << std::endl << this->GetJson() << std::endl;
}

std::string
zData::GetJson() const
{
  std::stringstream json;
  boost::property_tree::write_json(json, this->_pt);
  return (json.str());
}

void
zData::SetJson(const std::string &json_)
{
  std::stringstream json(json_);
  boost::property_tree::read_json(json, this->_pt);
}

std::string
zData::GetXml() const
{
  std::stringstream xml;
  boost::property_tree::write_xml(xml, this->_pt);
  return (xml.str());
}

void
zData::SetXml(const std::string &xml_)
{
  std::stringstream xml(xml_);
  boost::property_tree::read_xml(xml, this->_pt);
}

bool
zData::_isMe(const std::string &key_) const
{
  return (key_ == this->GetKey());
}

bool
zData::_isMine(const std::string &key_) const
{
  return (this->_me(key_) == this->GetKey());
}

std::string
zData::_me(const std::string &key_) const
{
  return (key_.substr(0, key_.find_first_of(".")));
}

}
