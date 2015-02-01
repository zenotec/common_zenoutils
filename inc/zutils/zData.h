//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zData.h
//    Description:
//
//*****************************************************************************

#ifndef __ZDATA_H__
#define __ZDATA_H__

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <zutils/zLog.h>

namespace zUtils
{
namespace zData
{

class Data
{

  static const std::string ROOT;
  static const std::string KEY;

public:

  Data(const std::string &key_ = "");

  virtual
  ~Data();

  bool
  operator ==(const Data &other_) const;
  bool
  operator !=(const Data &other_) const;

  std::string
  GetKey() const;

  std::string
  GetValue(const std::string &key_) const;
  void
  GetValue(const std::string &key_, std::string &value_) const;
  bool
  SetValue(const std::string &key_, const std::string &value_);
  bool
  AddValue(const std::string &key_, const std::string &value_);

  void
  GetChild(const std::string &key_, Data &child_) const;
  bool
  PutChild(const std::string &key_, const Data &child_);
  bool
  AddChild(const std::string &key_, const Data &child_);

  std::string
  GetJson() const;
  bool
  SetJson(const std::string &json_);

  std::string
  GetXml() const;
  bool
  SetXml(const std::string &xml_);

protected:

  std::string
  _getKey() const;
  bool
  _setKey(const std::string &key_);
  boost::property_tree::ptree
  _getValue(const std::string &key_) const;
  bool
  _setValue(const std::string &key_, const boost::property_tree::ptree &pt_);

  boost::property_tree::ptree _pt;

private:

};

}
}

#endif /* __ZDATA_H__ */
