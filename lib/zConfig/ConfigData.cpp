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
// Class: ConfigData
//**********************************************************************

ConfigData::ConfigData(const std::string& path_) :
    zData::Data(ConfigPath::ConfigRoot)
{
  if (!path_.empty())
  {
    this->Append(path_);
    this->PutValue(*this, std::string(""));
  }
}

ConfigData::ConfigData(ConfigPath& path_) :
    zData::Data(path_.GetDataPath())
{
}

ConfigData::ConfigData(zData::Data& data_) :
    zData::Data(data_)
{
}

ConfigData::ConfigData(const zData::Data& data_) :
    zData::Data(data_)
{
}

ConfigData::~ConfigData()
{
}

ConfigData&
ConfigData::operator=(zData::Data& data_)
{
  *this = data_;
  return (*this);
}

ConfigData&
ConfigData::operator=(const zData::Data& data_)
{
  *this = data_;
  return (*this);
}

zData::Data&
ConfigData::GetData()
{
  return (*this);
}

const zData::Data&
ConfigData::GetData() const
{
  return (*this);
}

ConfigData&
ConfigData::GetConfigData()
{
  return (*this);
}

const ConfigData&
ConfigData::GetConfigData() const
{
  return (*this);
}

}
}
