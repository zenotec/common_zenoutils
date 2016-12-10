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
// Class: SerialConfiguration
//**********************************************************************

const std::string SerialConfiguration::ConfigRoot("zSerial");
const std::string SerialConfiguration::ConfigTypePath("Type");
const std::string SerialConfiguration::ConfigTypeNone("NONE");
const std::string SerialConfiguration::ConfigTypeEcho("Echo");
const std::string SerialConfiguration::ConfigTypeTty("TTY");

SerialConfiguration::SerialConfiguration()
{
  this->Type(ConfigTypeNone);
}

SerialConfiguration::SerialConfiguration(zData::Data &data_) :
    zConfig::Configuration(data_)
{
  this->Type(this->Type());
}

SerialConfiguration::SerialConfiguration(zConfig::Configuration &config_) :
    zConfig::Configuration(config_)
{
  this->Type(this->Type());
}

SerialConfiguration::~SerialConfiguration()
{
}

zConfig::Configuration&
SerialConfiguration::GetConfig()
{
  return (*this);
}

std::string
SerialConfiguration::Type() const
{
  std::string type;
  std::string path = zData::Data::PathConcat(SerialConfiguration::ConfigRoot,
      SerialConfiguration::ConfigTypePath);
  this->Get(type, path);
  return (type);
}

bool
SerialConfiguration::Type(const std::string& type_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(SerialConfiguration::ConfigRoot,
      SerialConfiguration::ConfigTypePath);
  if (this->Put(type_, path))
  {
    status = this->Commit();
  }
  return (status);
}

}
}
