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

#include <list>
#include <mutex>
#include <memory>

#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>

namespace zUtils
{
namespace zConfig
{

//**********************************************************************
// Class: ConfigPath
//**********************************************************************

const std::string ConfigPath::ConfigRoot("zConfig");

ConfigPath::ConfigPath(const std::string& root_) :
    zData::DataPath(ConfigPath::ConfigRoot)
{
  if (!root_.empty())
  {
    this->_root.push_back(root_);
  }
}

ConfigPath::ConfigPath(zData::DataPath& path_) :
    zData::DataPath(path_)
{
}

ConfigPath::ConfigPath(const zData::DataPath& path_) :
    zData::DataPath(path_)
{
}

ConfigPath::~ConfigPath()
{
}

zData::DataPath&
ConfigPath::GetDataPath()
{
  return (*this);
}

const zData::DataPath&
ConfigPath::GetDataPath() const
{
  return (*this);
}

ConfigPath&
ConfigPath::GetConfigPath()
{
  return (*this);
}

const ConfigPath&
ConfigPath::GetConfigPath() const
{
  return (*this);
}

}
}
