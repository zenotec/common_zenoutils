/*
 * Copyright (c) 2016 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
 *
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

// libc includes
// libc++ includes
#include <string>
#include <fstream>
#include <iostream>

// libzutils includes

#include <zutils/zCompatibility.h>
#include <zutils/zInterface.h>

namespace zUtils
{
namespace zInterface
{

// ****************************************************************************
// Class: InterfaceConfigPath
// ****************************************************************************

const std::string InterfaceConfigPath::ConfigRoot("zInterface");
const std::string InterfaceConfigPath::ConfigNamePath("Name");
const std::string InterfaceConfigPath::ConfigTypePath("Type");
const std::string InterfaceConfigPath::ConfigStatePath("State");
const std::string InterfaceConfigPath::ConfigRatePath("Rate");

InterfaceConfigPath::InterfaceConfigPath() :
    zConfig::ConfigPath(ConfigRoot)
{
}

InterfaceConfigPath::~InterfaceConfigPath()
{
}

zConfig::ConfigPath
InterfaceConfigPath::Name() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigNamePath);
  return (path);
}

zConfig::ConfigPath
InterfaceConfigPath::Type() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigTypePath);
  return (path);
}

zConfig::ConfigPath
InterfaceConfigPath::State() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigStatePath);
  return (path);
}

zConfig::ConfigPath
InterfaceConfigPath::Rate() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigRatePath);
  return (path);
}

// ****************************************************************************
// Class: InterfaceConfiguration
// ****************************************************************************

const std::string InterfaceConfigData::ConfigNameDefault("");

const std::string InterfaceConfigData::ConfigTypeNone("NONE");
const std::string InterfaceConfigData::ConfigTypeLoop("LOOP");
const std::string InterfaceConfigData::ConfigTypeWired("WIRED");
const std::string InterfaceConfigData::ConfigTypeWireless("WIRELESS");
const std::string InterfaceConfigData::ConfigTypeOther("OTHER");
const std::string InterfaceConfigData::ConfigTypeBond("BOND");
const std::string InterfaceConfigData::ConfigTypeBridge("BRIDGE");
const std::string InterfaceConfigData::ConfigTypeDefault(ConfigTypeNone);

const std::string InterfaceConfigData::ConfigStateUp("UP");
const std::string InterfaceConfigData::ConfigStateDown("DOWN");
const std::string InterfaceConfigData::ConfigStateDefault(ConfigStateDown);

const uint32_t InterfaceConfigData::ConfigRateDefault(1000000);

InterfaceConfigData::InterfaceConfigData() :
    zConfig::ConfigData(InterfaceConfigPath::ConfigRoot)
{
  ZLOG_DEBUG("InterfaceConfigData::InterfaceConfigData()");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());

  this->SetName(this->GetName());
  this->SetType(this->GetType());
  this->SetState(this->GetState());
  this->SetRate(this->GetRate());
}

InterfaceConfigData::InterfaceConfigData(const zData::Data& data_) :
    zConfig::ConfigData(data_)
{
  ZLOG_DEBUG("InterfaceConfigData::InterfaceConfigData(data_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());

  this->SetName(this->GetName());
  this->SetType(this->GetType());
  this->SetState(this->GetState());
  this->SetRate(this->GetRate());
}

InterfaceConfigData::InterfaceConfigData(const zConfig::ConfigData& config_) :
    zConfig::ConfigData(config_)
{
  ZLOG_DEBUG("InterfaceConfigData::InterfaceConfigData(config_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());

  this->SetName(this->GetName());
  this->SetType(this->GetType());
  this->SetState(this->GetState());
  this->SetRate(this->GetRate());
}

InterfaceConfigData::InterfaceConfigData(const InterfaceConfigData& other_) :
    zConfig::ConfigData(other_.GetConfigData())
{
  ZLOG_DEBUG("InterfaceConfigData::InterfaceConfigData(other_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());

  this->SetName(this->GetName());
  this->SetType(this->GetType());
  this->SetState(this->GetState());
  this->SetRate(this->GetRate());
}

InterfaceConfigData::~InterfaceConfigData()
{
}

std::string
InterfaceConfigData::GetName() const
{
  std::string str;
  InterfaceConfigPath path;
  if (!this->Get(path.Name(), str))
  {
    str = ConfigNameDefault;
  }
  return (str);
}

bool
InterfaceConfigData::SetName(const std::string& name_)
{
  InterfaceConfigPath path;
  return (this->Put(path.Name(), name_));
}

std::string
InterfaceConfigData::GetType() const
{
  std::string str;
  InterfaceConfigPath path;
  if (!this->Get(path.Type(), str))
  {
    str = ConfigTypeDefault;
  }
  return (str);
}

bool
InterfaceConfigData::SetType(const std::string& type_)
{
  InterfaceConfigPath path;
  return (this->Put(path.Type(), type_));
}

std::string
InterfaceConfigData::GetState() const
{
  std::string str;
  InterfaceConfigPath path;
  if (!this->Get(path.State(), str))
  {
    str = ConfigStateDefault;
  }
  return (str);
}

bool
InterfaceConfigData::SetState(const std::string& state_)
{
  InterfaceConfigPath path;
  return (this->Put(path.State(), state_));
}

uint32_t
InterfaceConfigData::GetRate() const
{
  uint32_t rate;
  InterfaceConfigPath path;
  if (!this->Get(path.Rate(), rate))
  {
    rate = ConfigRateDefault;
  }
  return (rate);
}

bool
InterfaceConfigData::SetRate(const uint32_t& rate_)
{
  InterfaceConfigPath path;
  return (this->Put(path.Rate(), rate_));
}

}
}
