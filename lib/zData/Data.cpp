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

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_DATA);

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

static std::string
ptJson(pt::ptree pt_)
{
  std::stringstream json;
  try
  {
    pt::write_json(json, pt_);
  }
  catch (pt::json_parser_error const &e)
  {
    json << "Parser error: " << e.what();
  }
  catch (pt::ptree_bad_path const &e)
  {
    json << "Path error: " << e.what();
  }
  return(json.str());
}

static void
ptDisplay(pt::ptree pt_)
{
  std::cout << ptJson(pt_) << std::endl;
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
  DataPath::operator =(other_.GetDataPath());
  this->_pt = other_._pt;
  other_._lock.Unlock();
  this->_lock.Unlock();
}

Data::Data(const Data &other_)
{
  other_._lock.Lock();
  DataPath::operator =(other_.GetDataPath());
  this->_pt = other_._pt;
  other_._lock.Unlock();
  this->_lock.Unlock();
}

Data::~Data()
{
  this->_pt.clear();
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

Data &
Data::operator+=(const Data& other_)
{
  other_._lock.Lock();
  this->PutChild(other_);
  other_._lock.Unlock();
  return(*this);
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

Data
Data::operator ()(const std::string& path_) const
{
  Data d(path_);
  try
  {
    d.put(d.Path(), this->_pt.get_child(this->Path(path_)));
  }
  catch (pt::ptree_bad_path &e)
  {
    ZLOG_WARN("Child does not exist: " + this->Path());
    ZLOG_INFO(this->GetJson());
  }
  return (d);
}

Data
Data::operator [](int pos_) const
{
  int i = 0;

  Data d(this->Key());

  // Begin critical section
  if (this->_lock.Lock())
  {
    try
    {
      FOREACH (auto& child, this->_pt.get_child(this->Path()))
      {
        if (i++ == pos_)
        {
          d.put(d.Path(), child.second);
          break;
        }
      }
    }
    catch (pt::ptree_bad_path &e)
    {
      ZLOG_WARN("Child does not exist: " + this->Path());
    }
    this->_lock.Unlock();
  }

  return (d);

}

bool
Data::Empty() const
{
  bool status = false;
  status = this->_pt.empty();
  return(status);
}

ssize_t
Data::Size() const
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
Data::GetChild(Data& child_) const
{
  bool status = false;

  // Begin critical section
  if (this->_lock.Lock())
  {
    if (child_._lock.Lock())
    {
      ZLOG_DEBUG("SRC: " + this->Root() + "; DST: " + child_.Root());
      pt::ptree pt;
      if (this->get(this->Root(), pt))
      {
        status = child_.put(child_.Root(), pt);
      }
      child_._lock.Unlock();
    }
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

bool
Data::GetChild(const DataPath& src_, Data& child_) const
{

  bool status = false;

  // Begin critical section
  if (this->_lock.Lock())
  {
    if (child_._lock.Lock())
    {
      ZLOG_DEBUG("SRC: " + src_.Path() + "; DST: " + child_.Root());
      pt::ptree pt;
      if (this->get(src_.Path(), pt))
      {
        child_.Clear();
        status = child_.put(child_.Root(), pt);
      }
      child_._lock.Unlock();
    }
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

bool
Data::GetChild(const DataPath& src_, const DataPath& dst_, Data& child_) const
{

  bool status = false;

  // Begin critical section
  if (this->_lock.Lock())
  {
    if (child_._lock.Lock())
    {
      ZLOG_DEBUG("SRC: " + src_.Path() + "; DST: " + dst_.Path());
      pt::ptree pt;
      if (this->get(src_.Path(), pt))
      {
        child_.Clear();
        status = child_.put(dst_.Path(), pt);
      }
      child_._lock.Unlock();
    }
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

bool
Data::PutChild(const Data& child_)
{
  bool status = false;

  if (this->_lock.Lock())
  {
    if (child_._lock.Lock())
    {
      ZLOG_DEBUG("SRC: " + child_.Root() + "; DST: " + this->Root());
      pt::ptree pt;
      if (child_.get(child_.Root(), pt))
      {
        status = this->put(this->Root(), pt);
      }
      child_._lock.Unlock();
    }
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

bool
Data::PutChild(const DataPath& dst_, const Data& child_)
{

  bool status = false;

  if (this->_lock.Lock())
  {
    if (child_._lock.Lock())
    {
      ZLOG_DEBUG("SRC: " + child_.Root() + "; DST: " + dst_.Path());
      pt::ptree pt;
      if (child_.get(child_.Root(), pt))
      {
        status = this->put(dst_.Path(), pt);
      }
      child_._lock.Unlock();
    }
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

bool
Data::PutChild(const DataPath& dst_, const DataPath& src_, const Data& child_)
{

  bool status = false;

  if (this->_lock.Lock())
  {
    if (child_._lock.Lock())
    {
      ZLOG_DEBUG("SRC: " + src_.Path() + "; DST: " + dst_.Path());
      pt::ptree pt;
      if (child_.get(src_.Path(), pt))
      {
        status = this->put(dst_.Path(), pt);
      }
      child_._lock.Unlock();
    }
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

bool
Data::AddChild(const Data& child_)
{

  bool status = false;

  // Begin critical section
  if (this->_lock.Lock())
  {
    ZLOG_DEBUG("SRC: " + child_.Root() + "; DST: " + this->Root());
    if (child_._lock.Lock())
    {
      pt::ptree pt;
      if (child_.get(child_.Root(), pt))
      {
        status = this->add(this->Root(), pt);
      }
      child_._lock.Unlock();
    }
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

bool
Data::AddChild(const DataPath& dst_, const Data& child_)
{

  bool status = false;

  // Begin critical section
  if (this->_lock.Lock())
  {
    ZLOG_DEBUG("SRC: " + child_.Root() + "; DST: " + dst_.Path());
    if (child_._lock.Lock())
    {
      pt::ptree pt;
      if (child_.get(child_.Root(), pt))
      {
        status = this->add(dst_.Path(), pt);
      }
      child_._lock.Unlock();
    }
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

bool
Data::AddChild(const DataPath& dst_, const DataPath& src_, const Data& child_)
{

  bool status = false;

  // Begin critical section
  if (this->_lock.Lock())
  {
    ZLOG_DEBUG("SRC: " + src_.Path() + "; DST: " + dst_.Path());
    if (child_._lock.Lock())
    {
      pt::ptree pt;
      if (child_.get(src_.Path(), pt))
      {
        status = this->add(dst_.Path(), pt);
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
  pt::write_json(json, this->_pt, false);
  return (json.str());
}

std::string
Data::GetJsonPretty() const
{
  std::stringstream json;
  pt::write_json(json, this->_pt, true);
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
  std::cout << std::endl << this->GetJsonPretty() << std::endl;
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
      ZLOG_DEBUG(ptJson(this->_pt));
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
      ZLOG_DEBUG(ptJson(pt_));
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

