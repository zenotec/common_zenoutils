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

#include <zutils/zLog.h>
#include <zutils/zData.h>

namespace pt = boost::property_tree;

namespace zUtils
{
namespace zData
{

static std::string
ptKey(pt::ptree pt_, int index_ = 0)
{
  int i = 0;
  std::string key;
  try
  {
    FOREACH (auto& child, pt_)
    {
      if (i++ == index_)
      {
        key = child.first;
        break;
      }
    }
  }
  catch (pt::ptree_bad_path &e)
  {
    key = "";
  }
  return (key);
}

static void
ptDisplay(pt::ptree pt_)
{
  std::stringstream json;
  std::cout << std::endl << "pt: " << std::endl;
  try
  {
    pt::write_json(json, pt_);
    std::cout << json.str() << std::endl;
  }
  catch (pt::json_parser_error const &e)
  {
    std::cout << "Parser error: " << e.what();
  }
  catch (pt::ptree_bad_path const &e)
  {
    std::cout << "Path error: " << e.what();
  }
}

//**********************************************************************
// Class: zData::Data
//**********************************************************************

Data::Data(const std::string& path_) :
    DataPath(path_)
{
  this->put(this->Path(), std::string(""));
  this->_lock.Unlock();
}

Data::Data(const zData::DataPath& path_) :
    DataPath(path_)
{
  this->put(this->Path(), std::string(""));
  this->_lock.Unlock();
}

Data::Data(const pt::ptree& pt_) :
    _pt(pt_)
{
  this->_lock.Unlock();
}

Data::Data(Data &other_)
{
  other_._lock.Lock();
  DataPath::operator =(other_);
  this->_pt = other_._pt;
  other_._lock.Unlock();
  this->_lock.Unlock();
}

Data::Data(const Data &other_)
{
  other_._lock.Lock();
  DataPath::operator =(other_);
  this->_pt = other_._pt;
  other_._lock.Unlock();
  this->_lock.Unlock();
}

Data::~Data()
{
  this->_pt.clear();
}

Data &
Data::operator=(Data &other_)
{
  other_._lock.Lock();
  this->_lock.Lock();
  DataPath::operator=(other_);
  this->_pt = other_._pt;
  this->_lock.Unlock();
  other_._lock.Unlock();
  return (*this);
}

Data &
Data::operator=(const Data &other_)
{
  other_._lock.Lock();
  this->_lock.Lock();
  DataPath::operator=(other_);
  this->_pt = other_._pt;
  this->_lock.Unlock();
  other_._lock.Unlock();
  return (*this);
}

bool
Data::operator ==(const Data &other_) const
    {
  bool status = true;
  other_._lock.Lock();
  this->_lock.Lock();
  status &= (this->Path() == other_.Path());
  status &= (this->_pt == other_._pt);
  this->_lock.Unlock();
  other_._lock.Unlock();
  return (status);
}

bool
Data::operator !=(const Data &other_) const
    {
  bool status = true;
  other_._lock.Lock();
  this->_lock.Lock();
  status &= (this->Path() == other_.Path());
  status &= (this->_pt == other_._pt);
  this->_lock.Unlock();
  other_._lock.Unlock();
  return (!status);
}

UNIQUE_PTR(Data)
Data::operator [](int pos_)
{
  int i = 0;

  UNIQUE_PTR(Data) d(new Data);

  // Begin critical section
  if (d && this->_lock.Lock())
  {
    ZLOG_DEBUG("Getting child[]: " + this->Root());
    ZLOG_DEBUG("Getting child[]: " + this->Base());
    ZLOG_DEBUG("Getting child[]: " + this->Path());
    try
    {
      FOREACH (auto& child, this->_pt.get_child(this->Path()))
      {
        if (i++ == pos_)
        {
          d->Append(this->Key());
          d->put(d->Root(this->Key()), child.second);
          break;
        }
      }
    }
    catch (pt::ptree_bad_path &e)
    {
      d = NULL;
    }
    this->_lock.Unlock();
  }

  return (MOVE(d));

}

ssize_t
Data::Size()
{
  ssize_t size = 0;

  try
  {
    FOREACH (auto& item, this->_pt.get_child(this->Path()))
    {
      size++;
    }
//    size = this->_pt.count(this->Path());
  }
  catch (pt::ptree_bad_path &e)
  {
    size = -1;
  }
  return (size);
}

void
Data::Clear()
{
  DataPath::Clear();
  this->del(this->Root());
}

bool
Data::Get(Data& child_) const
    {
  bool status = false;

  // Begin critical section
  if (this->_lock.Lock())
  {
    ZLOG_DEBUG("Getting data: " + this->Path());
    if (child_._lock.Lock())
    {
      pt::ptree pt;
      if (this->get(this->Path(), pt))
      {
        status = child_.put(child_.Base(this->Key()), pt);
      }
      child_._lock.Unlock();
    }
    this->_lock.Unlock();
//    child_.DisplayJson();
  }

  // Return status
  return (status);

}

bool
Data::Get(const DataPath& path_, Data& child_) const
    {

  bool status = false;

  // Begin critical section
  if (this->_lock.Lock())
  {
    ZLOG_DEBUG("Getting data: " + path_.Path() + "; Key: " + path_.Key());
    if (child_._lock.Lock())
    {
      pt::ptree pt;
      if (this->get(path_.Path(), pt))
      {
        child_.Clear();
        if ((status = child_.put(child_.Base(path_.Key()), pt)))
        {
          child_.Append(path_.Key());
        }
      }
      child_._lock.Unlock();
    }
    this->_lock.Unlock();
//    child_.DisplayJson();
  }

  // Return status
  return (status);

}

bool
Data::Put(const Data& child_)
{
  bool status = false;

  if (this->_lock.Lock())
  {
    ZLOG_DEBUG("1-Putting data: " + child_.Path() + " -> " + this->Base(child_.Key()));
    if (child_._lock.Lock())
    {
      pt::ptree pt;
      if (child_.get(child_.Path(), pt))
      {
        status = this->put(this->Base(child_.Key()), pt);
      }
      child_._lock.Unlock();
    }
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

bool
Data::Put(const DataPath& path_, const Data& child_)
{

  bool status = false;

  if (this->_lock.Lock())
  {
    ZLOG_DEBUG("2-Putting data: " + child_.Path() + " -> " + path_.Path(child_.Key()));
    if (child_._lock.Lock())
    {
      pt::ptree pt;
      if (child_.get(child_.Path(), pt))
      {
        status = this->put(path_.Path(child_.Key()), pt);
      }
      child_._lock.Unlock();
    }
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

bool
Data::Add(const Data& child_)
{

  bool status = false;

  // Begin critical section
  if (this->_lock.Lock())
  {
    ZLOG_DEBUG("1-Adding data: " + child_.Path() + " -> " + this->Base(child_.Key()));
    if (child_._lock.Lock())
    {
      pt::ptree pt;
      if (child_.get(child_.Path(), pt))
      {
        status = this->add(this->Base(child_.Key()), pt);
      }
      child_._lock.Unlock();
    }
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

bool
Data::Add(const DataPath& path_, const Data& child_)
{

  bool status = false;

  // Begin critical section
  if (this->_lock.Lock())
  {
    ZLOG_DEBUG("2-Adding data: " + child_.Path() + " -> " + path_.Path(child_.Key()));
    if (child_._lock.Lock())
    {
      pt::ptree pt;
      if (child_.get(child_.Path(), pt))
      {
        status = this->add(path_.Path(child_.Key()), pt);
      }
      child_._lock.Unlock();
    }
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

bool
Data::Del(const DataPath& path_)
{

  bool status = false;

  // Begin critical section
  if (this->_lock.Lock())
  {
    ZLOG_DEBUG("Deleting data: " + path_.Path());
    status = this->del(path_.Path());
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

std::string
Data::GetJson() const
{
  std::stringstream json;
  pt::write_json(json, this->_pt);
  return (json.str());
}

bool
Data::SetJson(const std::string &json_)
{
  bool status = false;
  try
  {
    std::stringstream json(json_);
    pt::ptree pt;
    // Convert json into property tree
    pt::read_json(json, pt);
    // Begin critical section
    if (this->_lock.Lock())
    {
      // Copy only the root node
      status = this->put(DataPath::DataRoot, pt.get_child(DataPath::DataRoot));
      this->_lock.Unlock();
    }
  }
  catch (pt::json_parser_error const &e)
  {
    ZLOG_WARN(std::string("Parser error: ") + e.what());
    status = false;
  }
  catch (pt::ptree_bad_path const &e)
  {
    ZLOG_WARN(std::string("Path error: ") + e.what());
    status = false;
  }
  return (status);
}

void
Data::DisplayJson() const
{
  std::cout << std::endl << this->GetJson() << std::endl;
}

std::string
Data::GetXml() const
{
  std::stringstream xml;
  pt::write_xml(xml, this->_pt);
  return (xml.str());
}

bool
Data::SetXml(const std::string &xml_)
{
  bool status = false;
  try
  {
    std::stringstream xml(xml_);
    pt::ptree pt;
    // Convert json into property tree
    pt::read_xml(xml, pt);
    // Begin critical section
    if (this->_lock.Lock())
    {
      // Copy only the root node
      this->put(DataPath::DataRoot, pt.get_child(DataPath::DataRoot));
      status = true;
      this->_lock.Unlock();
    }
  }
  catch (pt::xml_parser_error const &e)
  {
    status = false;
  }
  catch (pt::ptree_bad_path const &e)
  {
    status = false;
  }
  return (status);
}

void
Data::DisplayXml() const
{
  std::cout << std::endl << this->GetXml() << std::endl;
}

bool
Data::get(const std::string &path_, pt::ptree &pt_) const
    {
  bool status = false;

  if (!path_.empty())
  {
    ZLOG_DEBUG("getting pt: " + path_);
    try
    {
      pt_ = this->_pt.get_child(path_);
      status = true;
    }
    catch (pt::ptree_bad_path const &e)
    {
      ZLOG_WARN(std::string("Path error: ") + e.what());
      status = false;
    }
  }

  return (status);
}

bool
Data::put(const std::string &path_, const pt::ptree &pt_)
{
  bool status = false;

  if (!path_.empty())
  {
    ZLOG_DEBUG("putting pt: " + path_);
    try
    {
      this->_pt.put_child(path_, pt_);
      status = true;
    }
    catch (pt::ptree_bad_path const &e)
    {
      ZLOG_WARN(std::string("Path error: ") + e.what());
      status = false;
    }
  }

  return (status);
}

bool
Data::add(const std::string &path_, const pt::ptree &pt_)
{
  bool status = false;

  if (!path_.empty())
  {
    ZLOG_DEBUG("adding pt: " + path_);
    pt::ptree parent;
    this->get(path_, parent); // Ignore return
    parent.push_back(std::make_pair("", pt_));
    status = this->put(path_, parent);
  }

  return (status);
}

bool
Data::del(const std::string &path_)
{
  bool status = false;

  if (!path_.empty())
  {
    ZLOG_DEBUG("deleting pt: " + path_);
    pt::ptree parent;
    pt::ptree empty;
    if (this->get(path_, parent))
    {
      status = this->put(path_, empty);
    }
  }

  return (status);
}

}
}

