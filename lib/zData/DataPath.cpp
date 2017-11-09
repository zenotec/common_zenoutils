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

#include <iostream>

#include <zutils/zLog.h>
#include <zutils/zData.h>

namespace zUtils
{
namespace zData
{

static std::list<std::string>
path2list(const std::string& path_)
{
  std::list<std::string>path;
  if (!path_.empty())
  {
    char* str = (char*) malloc(path_.size() + 1);
    char* p = NULL;
    if (str)
    {
      strcpy(str, path_.c_str());
      p = strtok(str, ".");
      while (p != NULL)
      {
        path.push_back(p);
        p = strtok(NULL, ".");
      }
      free(str);
    }
  }
  return(path);
}

//**********************************************************************
// Class: DataPath
//**********************************************************************

const std::string DataPath::DataRoot("zData");

DataPath::DataPath(const std::string& path_)
{
  this->_root.push_back(DataRoot);
  if (!path_.empty())
  {
    std::list<std::string>path = path2list(path_);
    while(!path.empty())
    {
      this->_root.push_back(path.front());
      path.pop_front();
    }
  }
}

DataPath::~DataPath()
{
  this->_root.clear();
  this->_path.clear();
}

DataPath &
DataPath::operator=(DataPath& other_)
{
  this->_root = other_._root;
  this->_path = other_._path;
  return(*this);
}

DataPath &
DataPath::operator=(const DataPath& other_)
{
  this->_root = other_._root;
  this->_path = other_._path;
  return(*this);
}

DataPath
DataPath::operator+(const DataPath& other_)
{
  DataPath path(*this);
  path += other_;
  return (path);
}

DataPath &
DataPath::operator+=(const DataPath& other_)
{
  std::list<std::string>path = other_._path;
  while(!path.empty())
  {
    this->_path.push_back(path.front());
    path.pop_front();
  }
  return(*this);
}

bool
DataPath::operator ==(const DataPath& other_) const
    {
  return (this->Path() == other_.Path());
}

bool
DataPath::operator !=(const DataPath& other_) const
    {
  return (this->Path() != other_.Path());
}

DataPath
DataPath::operator ()(const std::string& path_)
{
  DataPath path(*this);
  path.Append(path_);
  return (path);
}

std::string
DataPath::operator [](const unsigned int index)
{
  std::string str = this->Root();
  if (!this->_path.empty() && index)
  {
    unsigned int i = 1;
    std::list<std::string> path = this->_path;
    str.clear();
    while (!path.empty())
    {
      if (index == i++)
      {
        str = path.front();
        break;
      }
      path.pop_front();
    }
  }
  return (str);

}

bool
DataPath::Prepend(const std::string& name_)
{
  bool status = false;
  if (!name_.empty())
  {
    std::list<std::string>path = path2list(name_);
    while(!path.empty())
    {
      this->_path.push_front(path.front());
      path.pop_front();
    }
    status = true;
  }
  return (status);
}

bool
DataPath::Append(const std::string& name_)
{
  bool status = false;
  if (!name_.empty())
  {
    std::list<std::string>path = path2list(name_);
    while(!path.empty())
    {
      this->_path.push_back(path.front());
      path.pop_front();
    }
    status = true;
  }
  return (status);
}

std::string
DataPath::PopFront()
{
  std::string str;
  if (!this->_path.empty())
  {
    str = this->_path.front();
    this->_path.pop_front();
  }
  return (str);
}

std::string
DataPath::PopBack()
{
  std::string str;
  if (!this->_path.empty())
  {
    str = this->_path.back();
    this->_path.pop_back();
  }
  return (str);
}

std::string
DataPath::Root(const std::string path_) const
{
  std::string root;
  if (!this->_root.empty())
  {
    std::list<std::string> path = this->_root;
    root += path.front();
    path.pop_front();
    while (!path.empty())
    {
      root += '.';
      root += path.front();
      path.pop_front();
    }
  }
  if (!path_.empty())
  {
    root += '.';
    root += path_;
  }
  return (root);
}

std::string
DataPath::Base(const std::string path_) const
{
  std::string base = this->Root();
  std::string key;

  if (!this->_path.empty())
  {
    std::list<std::string> path = this->_path;
    // Throw away last element
    key = path.back();
    path.pop_back();
    while (!path.empty())
    {
      base += '.';
      base += path.front();
      path.pop_front();
    }
  }
  if (!path_.empty())
  {
    base += '.';
    base += path_;
  }
  return (base);
}

std::string
DataPath::Key() const
{
  std::string key;
  if (!this->_path.empty())
  {
    key = this->_path.back();
  }
  return (key);
}

std::string
DataPath::Path(const std::string path_) const
{
  std::string path = this->Base();
  if (!this->_path.empty())
  {
    path += '.';
    path += this->Key();
  }
  if (!path_.empty())
  {
    path += '.';
    path += path_;
  }
  return (path);
}

void
DataPath::Clear()
{
  this->_path.clear();
}

void
DataPath::DisplayPath() const
{
  std::cout << "Root: " << this->Root() << std::endl;
  std::cout << "Base: " << this->Base() << std::endl;
  std::cout << "Key: " << this->Key() << std::endl;
  std::cout << "Path: " << this->Path() << std::endl;
}

}
}
