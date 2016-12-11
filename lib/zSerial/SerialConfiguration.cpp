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

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include <list>
#include <mutex>

#include <zutils/zQueue.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>
#include <zutils/zSerial.h>

namespace zUtils
{
namespace zSerial
{

//**********************************************************************
// Class: SerialConfigPath
//**********************************************************************

const std::string SerialConfigPath::ConfigRoot("zSerial");
const std::string SerialConfigPath::ConfigTypePath("Type");

SerialConfigPath::SerialConfigPath(const std::string& path_) :
    zConfig::ConfigPath(ConfigRoot)
{
  if (!path_.empty())
  {
    this->Append(path_);
  }
}

SerialConfigPath::~SerialConfigPath()
{
}

zConfig::ConfigPath
SerialConfigPath::Type()
{
  zConfig::ConfigPath path;
  path.Append(ConfigTypePath);
  return(path);
}

//**********************************************************************
// Class: SerialConfiguration
//**********************************************************************

const std::string SerialConfigData::ConfigTypeNone("NONE");
const std::string SerialConfigData::ConfigTypeEcho("Echo");
const std::string SerialConfigData::ConfigTypeTty("TTY");

SerialConfigData::SerialConfigData(const std::string& path_) :
    zConfig::ConfigData(SerialConfigPath::ConfigRoot)
{
  this->SetType(ConfigTypeNone);
}

SerialConfigData::SerialConfigData(zData::Data &data_) :
    zConfig::ConfigData(data_)
{
  this->SetType(this->GetType());
}

SerialConfigData::SerialConfigData(zConfig::ConfigData &config_) :
    zConfig::ConfigData(config_)
{
  this->SetType(this->GetType());
}

SerialConfigData::~SerialConfigData()
{
}

zConfig::ConfigData&
SerialConfigData::GetConfigData()
{
  return (*this);
}

std::string
SerialConfigData::GetType() const
{
  std::string str;
  SerialConfigPath path;
  if (!this->Get(path.Type(), str))
  {
    str = ConfigTypeNone;
  }
  return (str);
}

bool
SerialConfigData::SetType(const std::string& type_)
{
  SerialConfigPath path;
  return (this->Put(path.Type(), type_));
}

}
}
