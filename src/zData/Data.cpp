//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

#include <zutils/zData.h>

namespace zUtils
{

namespace zData
{

//**********************************************************************
// Class: zData::Data
//**********************************************************************

const std::string Data::ROOT = "Data";
const std::string Data::KEY = "Key";

Data::Data(const std::string &key_)
{
  this->key(key_);
  this->put("", key_);
}

Data::~Data()
{
  this->_pt.clear();
}

std::unique_ptr<Data>
Data::operator [](int pos_)
{
  int i = 0;

  // Begin critical section
  this->_lock.lock();

  std::unique_ptr<Data> d(new Data(this->key()));

  try
  {

    for (auto& item : this->_pt.get_child(this->key()))
    {
      if (i++ == pos_)
      {
        d->put(item.second, this->key());
        break;
      }
    }

  }
  catch (boost::property_tree::ptree_bad_path &e)
  {
    d = NULL;
  }

  // End critical section
  this->_lock.unlock();

  return (std::move(d));

}

std::unique_ptr<Data>
Data::operator [](const std::string &path_)
{
  std::unique_ptr<Data> d(new Data(this->Key()));
  if (!this->Get(*d, path_))
  {
    d = NULL;
  }
  return (std::move(d));
}

std::string
Data::Key()
{
  std::string key;

  // Begin critical section
  this->_lock.lock();

  key = this->key();

  // End critical section
  this->_lock.unlock();

  return (key);
}

ssize_t
Data::Size()
{
  ssize_t size = 0;

  try
  {
    for (auto& item : this->_pt.get_child(this->key()))
    {
      size++;
    }
  }
  catch (boost::property_tree::ptree_bad_path &e)
  {
    size = -1;
  }
  return (size);
}

void
Data::Clear()
{
  this->Del();
}

bool
Data::Get(Data &data_, const std::string &path_) const
{

  bool status = false;

  // Begin critical section
  this->_lock.lock();

  // Setup path
  std::string path = this->key();
  if (!path_.empty())
  {
    path = Data::PathConcat(path, path_);
  }

  boost::property_tree::ptree pt;
  if (this->get(pt, path))
  {
    data_._lock.lock();
    std::string key = Data::PathLast(path);
    data_.key(key);
    status = data_.put(pt, key);
    data_._lock.unlock();
  }

//  std::cout << "Getting data: " << path << " Data: " << data_.key () << std::endl;

  // End critical section
  this->_lock.unlock();

  // Return status
  return (status);

}

bool
Data::Put(Data &data_, const std::string &path_)
{

  bool status = false;

  // Begin critical section
  this->_lock.lock();

  // Setup path
  std::string path = this->key();
  if (!path_.empty())
  {
    path = Data::PathConcat(path, path_);
  }

//  std::cout << "Putting data: " << path << " Data: " << data_.key () << std::endl;

  boost::property_tree::ptree pt;
  if (data_.Get(pt))
  {
    status = this->put(pt, path);
  }

  // End critical section
  this->_lock.unlock();

  // Return status
  return (status);

}

bool
Data::Add(Data &data_, const std::string &path_)
{

  bool status = false;

  // Begin critical section
  this->_lock.lock();

  // Setup path
  std::string path = this->key();
  if (!path_.empty())
  {
    path = Data::PathConcat(path, path_);
  }

//  std::cout << "Adding data: " << path << " Data: " << data_.key () << std::endl;

  boost::property_tree::ptree pt;
  if (data_.Get(pt))
  {
    status = this->add(pt, path);
  }

  // End critical section
  this->_lock.unlock();

  // Return status
  return (status);

}

bool
Data::Del(const std::string &path_)
{

  bool status = false;

  // Begin critical section
  this->_lock.lock();

  // Setup path
  std::string path = this->key();
  if (!path_.empty())
  {
    path = Data::PathConcat(path, path_);
  }

//  std::cout << "Deleting data: " << path << std::endl;

  status = this->del(path);

//  this->DisplayJson();

// End critical section
  this->_lock.unlock();

  // Return status
  return (status);

}

std::string
Data::GetJson() const
{
  std::stringstream json;
  boost::property_tree::write_json(json, this->_pt);
  return (json.str());
}

bool
Data::SetJson(const std::string &json_)
{
  bool status = false;
  std::stringstream json(json_);
  try
  {
    boost::property_tree::read_json(json, this->_pt);
    status = true;
  }
  catch (boost::property_tree::json_parser_error const &e)
  {
    status = false;
  }
  return (status);
}

void
Data::DisplayJson() const
{
  std::cout << this->GetJson() << std::endl;
}

std::string
Data::GetXml() const
{
  std::stringstream xml;
  boost::property_tree::write_xml(xml, this->_pt);
  return (xml.str());
}

bool
Data::SetXml(const std::string &xml_)
{
  bool status = false;
  std::stringstream xml(xml_);
  try
  {
    boost::property_tree::read_xml(xml, this->_pt);
    status = true;
  }
  catch (boost::property_tree::xml_parser_error const &e)
  {
    status = false;
  }
  return (status);
}

void
Data::DisplayXml() const
{
  std::cout << this->GetXml() << std::endl;
}

bool
Data::get(boost::property_tree::ptree &pt_, const std::string &path_) const
{
  bool status = false;

  if (!path_.empty())
  {
//    std::cout << "Getting child: " << path_ << std::endl;
    try
    {
      pt_ = this->_pt.get_child(path_);
      status = true;
    }
    catch (boost::property_tree::ptree_bad_path const &e)
    {
      status = false;
    }
  }

  return (status);
}

bool
Data::put(const boost::property_tree::ptree &pt_, const std::string &path_)
{
  bool status = false;

  if (!path_.empty())
  {
//    std::cout << "Putting child: " << path_ << std::endl;
    try
    {
      this->_pt.put_child(path_, pt_);
      status = true;
    }
    catch (boost::property_tree::ptree_bad_path const &e)
    {
      status = false;
    }
  }

  return (status);
}

bool
Data::add(const boost::property_tree::ptree &pt_, const std::string &path_)
{
  bool status = false;

  if (!path_.empty())
  {
//    std::cout << "Adding child: " << path_ << std::endl;
    boost::property_tree::ptree parent;
    this->get(parent, path_); // Ignore return value
    parent.push_back(std::make_pair("", pt_));
    status = this->put(parent, path_);
  }

  return (status);
}

bool
Data::del(const std::string &path_)
{
  bool status = false;

  if (!path_.empty())
  {
//    std::cout << "Deleting child: " << path_ << std::endl;
    boost::property_tree::ptree parent;
    boost::property_tree::ptree empty;
    if (this->get(parent, path_))
    {
      status = this->put(empty, path_);
    }
  }

  return (status);
}

std::string
Data::key() const
{
  std::string key;
  this->get(key, Data::KEY);
  return (key);
}

bool
Data::key(const std::string &key_)
{
  return (this->put(key_, Data::KEY));
}

}
}
