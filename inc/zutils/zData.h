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

  std::string
  GetKey() const;

  std::string
  GetValue(const std::string &key_) const;
  void
  GetValue(const std::string &key_, std::string &value_) const;
  void
  SetValue(const std::string &key_, const std::string &value_);
  void
  AddValue(const std::string &key_, const std::string &value_);

  void
  GetChild(const std::string &key_, Data &child_) const;
  void
  PutChild(const std::string &key_, const Data &child_);
  void
  AddChild(const std::string &key_, const Data &child_);

  std::string
  GetJson() const;
  void
  SetJson(const std::string &json_);

  std::string
  GetXml() const;
  void
  SetXml(const std::string &xml_);

protected:
  std::string
  _getKey() const;
  void
  _setKey(const std::string &key_);
  boost::property_tree::ptree
  _getValue(const std::string &key_) const;
  void
  _setValue(const std::string &key_, const boost::property_tree::ptree &pt_);

private:

  boost::property_tree::ptree _pt;
};

}
}

#endif /* __ZDATA_H__ */
