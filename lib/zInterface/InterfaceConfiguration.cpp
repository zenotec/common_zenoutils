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

const std::string ConfigPath::ConfigRoot("zInterface");
const std::string ConfigPath::ConfigNamePath("Name");
const std::string ConfigPath::ConfigTypePath("Type");
const std::string ConfigPath::ConfigHwAddressPath("HwAddress");
const std::string ConfigPath::ConfigIpAddressPath("IpAddress");
const std::string ConfigPath::ConfigNetmaskPath("Netmask");
const std::string ConfigPath::ConfigStatePath("State");

ConfigPath::ConfigPath() :
    zConfig::ConfigPath(ConfigRoot)
{
}

ConfigPath::~ConfigPath()
{
}

zConfig::ConfigPath
ConfigPath::Name() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigNamePath);
  return (path);
}

zConfig::ConfigPath
ConfigPath::Type() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigTypePath);
  return (path);
}

zConfig::ConfigPath
ConfigPath::HwAddress() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigHwAddressPath);
  return (path);
}

zConfig::ConfigPath
ConfigPath::IpAddress() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigIpAddressPath);
  return (path);
}

zConfig::ConfigPath
ConfigPath::Netmask() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigNetmaskPath);
  return (path);
}

zConfig::ConfigPath
ConfigPath::State() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigStatePath);
  return (path);
}

// ****************************************************************************
// Class: InterfaceConfiguration
// ****************************************************************************

const std::string ConfigData::ConfigNameDefault("");

const std::string ConfigData::ConfigTypeNone("");
const std::string ConfigData::ConfigTypeLoop("LOOP");
const std::string ConfigData::ConfigTypeWired("WIRED");
const std::string ConfigData::ConfigTypeWireless("WIRELESS");
const std::string ConfigData::ConfigTypeOther("OTHER");
const std::string ConfigData::ConfigTypeBond("BOND");
const std::string ConfigData::ConfigTypeBridge("BRIDGE");
const std::string ConfigData::ConfigTypeDefault(ConfigTypeNone);

const std::string ConfigData::ConfigHwAddressDefault("");

const std::string ConfigData::ConfigIpAddressDefault("");

const std::string ConfigData::ConfigNetmaskDefault("");

const std::string ConfigData::ConfigStateNone("");
const std::string ConfigData::ConfigStateUp("UP");
const std::string ConfigData::ConfigStateDown("DOWN");
const std::string ConfigData::ConfigStateDefault(ConfigStateNone);

ConfigData::ConfigData() :
    zConfig::ConfigData(ConfigPath::ConfigRoot)
{
  ZLOG_DEBUG("InterfaceConfigData::InterfaceConfigData()");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

ConfigData::ConfigData(const zData::Data& data_) :
    zConfig::ConfigData(ConfigPath::ConfigRoot)
{
  this->PutChild(data_);
  ZLOG_DEBUG("InterfaceConfigData::InterfaceConfigData(data_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

ConfigData::ConfigData(const zConfig::ConfigData& config_) :
    zConfig::ConfigData(ConfigPath::ConfigRoot)
{
  this->PutChild(config_.GetData());
  ZLOG_DEBUG("InterfaceConfigData::InterfaceConfigData(config_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

ConfigData::ConfigData(const ConfigData& other_) :
    zConfig::ConfigData(other_)
{
  ZLOG_DEBUG("InterfaceConfigData::InterfaceConfigData(other_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

ConfigData::~ConfigData()
{
}

std::string
ConfigData::Name() const
{
  std::string str;
  ConfigPath path;
  if (!this->GetValue(path.Name(), str))
  {
    str = ConfigNameDefault;
  }
  return (str);
}

bool
ConfigData::Name(const std::string& name_)
{
  ConfigPath path;
  return (this->PutValue(path.Name(), name_));
}

ConfigData::TYPE
ConfigData::Type() const
{
  ConfigData::TYPE type = ConfigData::TYPE_DEF;
  std::string str;
  ConfigPath path;
  if (this->GetValue(path.Type(), str))
  {
    if (str == ConfigData::ConfigTypeNone)
    {
      type = ConfigData::TYPE_NONE;
    }
    else if (str == ConfigData::ConfigTypeLoop)
    {
      type = ConfigData::TYPE_LOOP;
    }
    else if (str == ConfigData::ConfigTypeWired)
    {
      type = ConfigData::TYPE_WIRED;
    }
    else if (str == ConfigData::ConfigTypeWireless)
    {
      type = ConfigData::TYPE_WIRELESS;
    }
    else if (str == ConfigData::ConfigTypeOther)
    {
      type = ConfigData::TYPE_OTHER;
    }
    else if (str == ConfigData::ConfigTypeBond)
    {
      type = ConfigData::TYPE_BOND;
    }
    else if (str == ConfigData::ConfigTypeBridge)
    {
      type = ConfigData::TYPE_BRIDGE;
    }
    else
    {
      type = ConfigData::TYPE_ERR;
    }
  }
  return (type);
}

bool
ConfigData::Type(const ConfigData::TYPE type_)
{
  bool status = true;
  ConfigPath path;
  std::string str;
  switch (type_)
  {
  case ConfigData::TYPE_NONE:
    str = ConfigTypeNone;
    break;
  case ConfigData::TYPE_LOOP:
    str = ConfigTypeLoop;
    break;
  case ConfigData::TYPE_WIRED:
    str = ConfigTypeWired;
    break;
  case ConfigData::TYPE_WIRELESS:
    str = ConfigTypeWireless;
    break;
  case ConfigData::TYPE_OTHER:
    str = ConfigTypeOther;
    break;
  case ConfigData::TYPE_BOND:
    str = ConfigTypeBond;
    break;
  case ConfigData::TYPE_BRIDGE:
    str = ConfigTypeBridge;
    break;
  default:
    status = false;
  }
  return (this->PutValue(path.Type(), str));
}

std::string
ConfigData::HwAddress() const
{
  std::string str;
  ConfigPath path;
  if (!this->GetValue(path.HwAddress(), str))
  {
    str = ConfigHwAddressDefault;
  }
  return (str);
}

bool
ConfigData::HwAddress(const std::string& addr_)
{
  ConfigPath path;
  return (this->PutValue(path.HwAddress(), addr_));
}

std::string
ConfigData::IpAddress() const
{
  std::string str;
  ConfigPath path;
  if (!this->GetValue(path.IpAddress(), str))
  {
    str = ConfigIpAddressDefault;
  }
  return (str);
}

bool
ConfigData::IpAddress(const std::string& addr_)
{
  ConfigPath path;
  return (this->PutValue(path.IpAddress(), addr_));
}

std::string
ConfigData::Netmask() const
{
  std::string str;
  ConfigPath path;
  if (!this->GetValue(path.Netmask(), str))
  {
    str = ConfigNetmaskDefault;
  }
  return (str);
}

bool
ConfigData::Netmask(const std::string& addr_)
{
  ConfigPath path;
  return (this->PutValue(path.Netmask(), addr_));
}

ConfigData::STATE
ConfigData::AdminState() const
{
  ConfigData::STATE state = ConfigData::STATE_DEF;
  std::string str;
  ConfigPath path;
  if (this->GetValue(path.State(), str))
  {
    if (str == ConfigData::ConfigStateUp)
    {
      state = ConfigData::STATE_UP;
    }
    else if (str == ConfigData::ConfigStateDown)
    {
      state = ConfigData::STATE_DOWN;
    }
    else if (str == ConfigData::ConfigStateNone)
    {
      state = ConfigData::STATE_NONE;
    }
    else
    {
      state = ConfigData::STATE_ERR;
    }
  }
  return (state);
}

bool
ConfigData::AdminState(const ConfigData::STATE state_)
{
  bool status = true;
  ConfigPath path;
  std::string str;
  switch (state_)
  {
  case ConfigData::STATE_UP:
    str = ConfigStateUp;
    break;
  case ConfigData::STATE_DOWN:
    str = ConfigStateDown;
    break;
  case ConfigData::STATE_NONE:
    str = ConfigStateDefault;
    break;
  case ConfigData::STATE_UNKNOWN:
    // No break
  default:
    status = false;
  }
  return (this->PutValue(path.State(), str));
}


}
}
