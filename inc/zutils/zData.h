/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ZDATA_H__
#define __ZDATA_H__

#include <string>
#include <list>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <zutils/zCompatibility.h>
#include <zutils/zSem.h>

namespace zUtils
{
namespace zData
{

namespace pt = boost::property_tree;

//**********************************************************************
// Class: DataPath
//**********************************************************************

class DataPath
{

public:

  static const std::string DataRoot;

  DataPath(const std::string& name_ = std::string(""));

  DataPath(DataPath& path_) :
      _root(path_._root), _path(path_._path)
  {
  }

  DataPath(const DataPath& path_) :
      _root(path_._root), _path(path_._path)
  {
  }

  virtual
  ~DataPath();

  DataPath &
  operator=(DataPath& other_);

  DataPath &
  operator=(const DataPath& other_);

  DataPath
  operator+(const DataPath& other_);

  DataPath &
  operator+=(const DataPath& other_);

  bool
  operator==(const DataPath& other_) const;

  bool
  operator!=(const DataPath& other_) const;

  DataPath
  operator()(const std::string& path_);

  std::string
  operator[](const unsigned int index);

  bool
  Prepend(const std::string& name_);

  bool
  Append(const std::string& name_);

  std::string
  PopFront();

  std::string
  PopBack();

  std::string
  Root(const std::string path_ = std::string("")) const;

  std::string
  Base(const std::string path_ = std::string("")) const;

  std::string
  Key() const;

  std::string
  Path(const std::string path_ = std::string("")) const;

  void
  Clear();

  void
  DisplayPath() const;

protected:

  std::list<std::string> _root;

private:

  std::list<std::string> _path;

};

//**********************************************************************
// Class: Data
//**********************************************************************

class Data : public DataPath
{

public:

  Data(const std::string& path_ = std::string(""));

  Data(const DataPath& path_);

  Data(const pt::ptree& pt_);

  Data(Data& other_);

  Data(const Data& other_);

  virtual
  ~Data();

//  Data &
//  operator=(Data& other_);

  Data &
  operator=(const Data& other_);

  Data &
  operator+=(const Data& other_);

  bool
  operator==(const Data& other_) const;

  bool
  operator!=(const Data& other_) const;

  Data
  operator()(const std::string& path_) const;

  Data
  operator[](int pos_) const;

  const DataPath&
  GetDataPath() const
  {
    return (*this);
  }

  bool
  Empty() const;

  ssize_t
  Size() const;

  void
  Clear();

  bool
  GetChild(Data& child_) const;

  bool
  GetChild(const DataPath& src_, Data& child_) const;

  bool
  GetChild(const DataPath& src_, const DataPath& dst_, Data& child_) const;

  template<typename T>
    T
    GetValue(const std::string& path_) const
        {

      T value;

      // Begin critical section
      if (this->_lock.Lock())
      {
        this->get<T>(this->Path(path_), value);
        this->_lock.Unlock();
      }

      // Return status
      return (value);

    }

  template<typename T>
    bool
    GetValue(const DataPath& src_, T &value_) const
    {

      bool status = false;

      // Begin critical section
      if (this->_lock.Lock())
      {
        status = this->get<T>(src_.Path(), value_);
        this->_lock.Unlock();
      }

      // Return status
      return (status);

    }

  bool
  PutChild(const Data& child_);

  bool
  PutChild(const DataPath& dst_, const Data& child_);

  bool
  PutChild(const DataPath& dst_, const DataPath& src_, const Data& child_);

  template<typename T>
    bool
    PutValue(const DataPath& dst_, const T &value_)
    {

      bool status = false;

      // Begin critical section
      if (this->_lock.Lock())
      {
        status = this->put<T>(dst_.Path(), value_);
        this->_lock.Unlock();
      }

      // Return status
      return (status);

    }

  bool
  AddChild(const Data& child_);

  bool
  AddChild(const DataPath& dst_, const Data& child_);

  bool
  AddChild(const DataPath& dst_, const DataPath& src_, const Data& child_);

  template<typename T>
    bool
    AddValue(const DataPath& path_, const T &value_)
    {

      bool status = false;

      // Begin critical section
      if (this->_lock.Lock())
      {
        status = this->add<T>(path_.Path(), value_);
        this->_lock.Unlock();
      }

      // Return status
      return (status);

    }

  bool
  Del(const DataPath& path_);

  // Json utility functions
  std::string
  GetJson() const;

  std::string
  GetJsonPretty() const;

  bool
  SetJson(const std::string &json_);

  void
  DisplayJson() const;

  // XML utility functions

  std::string
  GetXml() const;

  bool
  SetXml(const std::string &xml_);

  void
  DisplayXml() const;

protected:

  mutable zSem::Mutex _lock;
  pt::ptree _pt;

private:

  bool
  get(const std::string &path_, pt::ptree &pt_) const;

  template<typename T>
    bool
    get(const std::string &path_, T &value_) const
    {
      bool status = false;
      if (!path_.empty())
      {
        try
        {
          value_ = this->_pt.get<T>(path_);
          status = true;
        }
        catch (pt::ptree_bad_path &e)
        {
          status = false;
        }
//      this->DisplayJson();
      }
      return (status);
    }

  bool
  put(const std::string& path_, const pt::ptree &pt_);

  template<typename T>
    bool
    put(const std::string& path_, const T &value_)
    {
      bool status = false;
      if (!path_.empty())
      {
        try
        {
          this->_pt.put<T>(path_, value_);
          status = true;
        }
        catch (pt::ptree_bad_path &e)
        {
          status = false;
        }
//      this->DisplayJson();
      }
      return (status);
    }

  bool
  add(const std::string& path_, const pt::ptree &pt_);

  template<typename T>
    bool
    add(const std::string& path_, const T &value_)
    {
      bool status = false;
      if (!path_.empty())
      {
        try
        {
          pt::ptree parent;
          pt::ptree child;
          this->get(path_, parent);
          child.put_value<T>(value_);
          parent.push_back(std::make_pair("", child));
          status = this->put(path_, parent);
        }
        catch (pt::ptree_bad_path &e)
        {
          status = false;
        }
//      this->DisplayJson();
      }
      return (status);
    }

  bool
  del(const std::string &path_);

};

}
}

#endif /* __ZDATA_H__ */
