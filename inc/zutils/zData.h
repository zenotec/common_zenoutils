//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

#ifndef _DATA_H_
#define _DATA_H_

#include <string>
#include <mutex>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace zUtils
{
namespace zData
{

//**********************************************************************
// Class: Data
//**********************************************************************

class Data
{

  static const std::string ROOT;
  static const std::string KEY;

public:

  Data(const std::string &key_ = std::string(""));

  Data(const boost::property_tree::ptree &pt_) :
      _pt(pt_)
  {
  }

  Data(Data &other_)
  {
    other_._lock.lock();
    this->_pt = other_._pt;
    other_._lock.unlock();
  }

  // Provided for completeness, however, this is not thread-safe
  Data(const Data &other_)
  {
    this->_pt = other_._pt;
  }

  virtual
  ~Data();

  Data &
  operator=(Data &other_)
  {
    other_._lock.lock();
    this->_lock.lock();
    this->_pt = other_._pt;
    this->_lock.unlock();
    other_._lock.unlock();
    return (*this);
  }

  // Provided for completeness, however, this is not thread-safe
  Data &
  operator=(const Data &other_)
  {
    this->_pt = other_._pt;
    return (*this);
  }

  // Provided for completeness, however, this is not thread-safe
  bool
  operator ==(const Data &other_) const
      {
    bool status = false;
    return (this->_pt == other_._pt);
  }

  // Provided for completeness, however, this is not thread-safe
  bool
  operator !=(const Data &other_) const
      {
    return (this->_pt != other_._pt);
  }

  std::unique_ptr<Data>
  operator [](int pos_);

  std::unique_ptr<Data>
  operator [](const std::string &key_);

  std::string
  Key();

  ssize_t
  Size();

  void
  Clear();

  bool
  Get(Data &value_, const std::string &path_ = std::string("")) const;

  template<typename T>
    bool
    Get(T &value_, const std::string &path_ = std::string("")) const
        {

      bool status = false;

      // Begin critical section
      this->_lock.lock();

      // Setup path
      std::string path = this->key();
      if (!path_.empty())
      {
        path += std::string(".") + path_;
      }

      // Get value
      status = this->get(value_, path);

      // End critical section
      this->_lock.unlock();

      // Return status
      return (status);

    }

  bool
  Put(const Data &child_, const std::string &path_ = std::string(""));

  template<typename T>
    bool
    Put(const T &value_, const std::string &path_ = std::string(""))
    {

      bool status = false;

      // Begin critical section
      this->_lock.lock();

      // Setup path
      std::string path = this->key();
      if (!path_.empty())
      {
        path += std::string(".") + path_;
      }

      status = this->put(value_, path);

      // End critical section
      this->_lock.unlock();

      // Return status
      return (status);

    }

  bool
  Add(const Data &child_, const std::string &path_ = std::string(""));

  template<typename T>
    bool
    Add(const T &value_, const std::string &path_ = std::string(""))
    {

      bool status = false;

      // Begin critical section
      this->_lock.lock();

      // Setup path
      std::string path = this->key();
      if (!path_.empty())
      {
        path += std::string(".") + path_;
      }

      status = this->add(value_, path);

      // End critical section
      this->_lock.unlock();

      // Return status
      return (status);

    }

  bool
  Del(const std::string &path_ = std::string(""));

  std::string
  GetJson() const;
  bool
  SetJson(const std::string &json_);
  void
  DisplayJson() const;

  std::string
  GetXml() const;
  bool
  SetXml(const std::string &xml_);
  void
  DisplayXml() const;

  // Path utility functions

  template<typename lT, typename rT>
    static std::string
    PathConcat(const lT &l_, const rT &r_)
    {
      std::stringstream ss;
      ss << l_ << "." << r_;
      return (ss.str());
    }

  static std::string
  PathFirst(const std::string &path_)
  {
    std::string str;
    if (!path_.empty())
    {
      size_t pos = path_.find_first_of(".");
      str = path_.substr(0, pos);
    }
    return (str);
  }

  static std::string
  PathLast(const std::string &path_)
  {
    std::string str;
    if (!path_.empty())
    {
      size_t pos = path_.find_last_of(".");
      if (pos != std::string::npos)
        str = path_.substr(pos + 1);
      else
        str = path_;
    }
    return (str);
  }

protected:

  bool
  get(boost::property_tree::ptree &pt_, const std::string &path_) const;

  template<typename T>
    bool
    get(T &value_, const std::string &path_) const
        {
      bool status = false;
      if (!path_.empty())
      {
//        std::cout << "Getting value: " << path_ << std::endl;
        try
        {
          value_ = this->_pt.get<T>(path_);
          status = true;
        }
        catch (boost::property_tree::ptree_bad_path &e)
        {
          status = false;
        }
      }
      return (status);
    }

  bool
  put(const boost::property_tree::ptree &pt_, const std::string &path_);

  template<typename T>
    bool
    put(const T &value_, const std::string &path_)
    {
      bool status = false;
      if (!path_.empty())
      {
//        std::cout << "Putting value: " << path_ << std::endl;
        try
        {
          this->_pt.put<T>(path_, value_);
          status = true;
        }
        catch (boost::property_tree::ptree_bad_path &e)
        {
          status = false;
        }
      }
      return (status);
    }

  bool
  add(const boost::property_tree::ptree &pt_, const std::string &path_);

  template<typename T>
    bool
    add(const T &value_, const std::string &path_)
    {
      bool status = false;
      if (!path_.empty())
      {
//        std::cout << "Adding value: " << path_ << std::endl;
//        this->DisplayJson();
        try
        {
          boost::property_tree::ptree parent;
          boost::property_tree::ptree child;
          this->get(parent, path_); // Ignore return value
          child.put_value<T>(value_);
          parent.push_back(std::make_pair("", child));
          status = this->put(parent, path_);
        }
        catch (boost::property_tree::ptree_bad_path &e)
        {
          status = false;
        }
      }
      return (status);
    }

  bool
  del(const std::string &path_);

  std::string
  key() const;

  bool
  key(const std::string &key_);

  boost::property_tree::ptree _pt;
  mutable std::mutex _lock;

private:

};

}
}

#endif /* _DATA_H_ */
