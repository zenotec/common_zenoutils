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
// Class: ConfigPath
// ****************************************************************************

const std::string ConfigPath::ConfigRoot("zInterface");
const std::string ConfigPath::ConfigIfNamePath("IfName");
const std::string ConfigPath::ConfigIfTypePath("IfType");
const std::string ConfigPath::ConfigHwAddressPath("HwAddress");
const std::string ConfigPath::ConfigMtuPath("MTU");
const std::string ConfigPath::ConfigIpAddressPath("IpAddress");
const std::string ConfigPath::ConfigBroadcastPath("Broadcast");
const std::string ConfigPath::ConfigNetmaskPath("Netmask");
const std::string ConfigPath::ConfigAdminStatePath("AdminState");

ConfigPath::ConfigPath(const std::string& root_) :
    zConfig::ConfigPath(ConfigRoot)
{
  this->Append(root_);
}

ConfigPath::ConfigPath(const zData::DataPath& path_) :
    zConfig::ConfigPath(ConfigRoot)
{
}

ConfigPath::ConfigPath(const ConfigPath& other_) :
    zConfig::ConfigPath(other_)
{
}

ConfigPath::~ConfigPath()
{
}

// ****************************************************************************
// Class: ConfigData
// ****************************************************************************

const std::string ConfigData::ConfigNameDefault("");

const std::string ConfigData::ConfigTypeNone("");
const std::string ConfigData::ConfigTypeLoop("LOOP");
const std::string ConfigData::ConfigTypeWired("WIRED");
const std::string ConfigData::ConfigTypeWireless("WIRELESS");
const std::string ConfigData::ConfigTypeOther("OTHER");
const std::string ConfigData::ConfigTypeBond("BOND");
const std::string ConfigData::ConfigTypeBridge("BRIDGE");
const std::string ConfigData::ConfigTypeDefault(ConfigData::ConfigTypeNone);

const std::string ConfigData::ConfigHwAddressDefault("00:00:00:00:00:00");

const unsigned int ConfigData::ConfigMtuDefault(1500);

const std::string ConfigData::ConfigIpAddressDefault("");

const std::string ConfigData::ConfigBroadcastDefault("");

const std::string ConfigData::ConfigNetmaskDefault("255.255.255.0");

const std::string ConfigData::ConfigAdminStateNone("");
const std::string ConfigData::ConfigAdminStateUp("UP");
const std::string ConfigData::ConfigAdminStateDown("DOWN");
const std::string ConfigData::ConfigAdminStateDefault(ConfigData::ConfigAdminStateNone);

ConfigData::ConfigData(const std::string& name_) :
    zConfig::ConfigData(ConfigPath::ConfigRoot)
{
  ZLOG_DEBUG("zInterface::ConfigData::ConfigData(name_)");
  this->SetIfName(name_);
  this->_init();
}

ConfigData::ConfigData(const zConfig::ConfigData& config_) :
    zConfig::ConfigData(ConfigPath::ConfigRoot)
{
  ZLOG_DEBUG("zInterface::ConfigData::ConfigData(config_)");
  this->PutChild(ConfigPath(), ConfigPath(), config_);
  this->_init();
}

ConfigData::~ConfigData()
{
}

std::string
ConfigData::GetIfName() const
{
  std::string str;
  ConfigPath path(ConfigPath::ConfigIfNamePath);
  if (!this->GetValue(path, str))
  {
    str = ConfigNameDefault;
  }
  return (str);
}

bool
ConfigData::SetIfName(const std::string& name_)
{
  ConfigPath path(ConfigPath::ConfigIfNamePath);
  return (this->PutValue(path, name_));
}

ConfigData::IFTYPE
ConfigData::GetIfType() const
{

  ConfigData::IFTYPE type = ConfigData::IFTYPE_DEF;
  std::string str;

  ConfigPath path(ConfigPath::ConfigIfTypePath);
  if (!this->GetValue(path, str))
  {
    str = ConfigTypeDefault;
  }

  if (str == ConfigData::ConfigTypeNone)
  {
    type = ConfigData::IFTYPE_NONE;
  }
  else if (str == ConfigData::ConfigTypeLoop)
  {
    type = ConfigData::IFTYPE_LOOP;
  }
  else if (str == ConfigData::ConfigTypeWired)
  {
    type = ConfigData::IFTYPE_IEEE8023;
  }
  else if (str == ConfigData::ConfigTypeWireless)
  {
    type = ConfigData::IFTYPE_IEEE80211;
  }
  else if (str == ConfigData::ConfigTypeOther)
  {
    type = ConfigData::IFTYPE_OTHER;
  }
  else if (str == ConfigData::ConfigTypeBond)
  {
    type = ConfigData::IFTYPE_BOND;
  }
  else if (str == ConfigData::ConfigTypeBridge)
  {
    type = ConfigData::IFTYPE_BRIDGE;
  }

  return (type);
}

bool
ConfigData::SetIfType(const ConfigData::IFTYPE type_)
{

  std::string str;
  ConfigPath path(ConfigPath::ConfigIfTypePath);

  switch (type_)
  {
  case ConfigData::IFTYPE_NONE:
    str = ConfigData::ConfigTypeNone;
    break;
  case ConfigData::IFTYPE_LOOP:
    str = ConfigData::ConfigTypeLoop;
    break;
  case ConfigData::IFTYPE_IEEE8023:
    str = ConfigData::ConfigTypeWired;
    break;
  case ConfigData::IFTYPE_IEEE80211:
    str = ConfigData::ConfigTypeWireless;
    break;
  case ConfigData::IFTYPE_OTHER:
    str = ConfigData::ConfigTypeOther;
    break;
  case ConfigData::IFTYPE_BOND:
    str = ConfigData::ConfigTypeBond;
    break;
  case ConfigData::IFTYPE_BRIDGE:
    str = ConfigData::ConfigTypeBridge;
    break;
  default:
    return (false);
  }

  return (this->PutValue(path, str));

}

unsigned int
ConfigData::GetMtu() const
{
  unsigned int val = 0;
  ConfigPath path(ConfigPath::ConfigMtuPath);
  if (!this->GetValue(path, val))
  {
    val = ConfigMtuDefault;
  }
  return (val);
}

bool
ConfigData::SetMtu(const unsigned int mtu_)
{
  ConfigPath path(ConfigPath::ConfigMtuPath);
  return (this->PutValue(path, mtu_));
}

std::string
ConfigData::GetHwAddress() const
{
  std::string str;
  ConfigPath path(ConfigPath::ConfigHwAddressPath);
  if (!this->GetValue(path, str))
  {
    str = ConfigHwAddressDefault;
  }
  return (str);
}

bool
ConfigData::SetHwAddress(const std::string& addr_)
{
  ConfigPath path(ConfigPath::ConfigHwAddressPath);
  return (this->PutValue(path, addr_));
}

std::string
ConfigData::GetIpAddress() const
{
  std::string str;
  ConfigPath path(ConfigPath::ConfigIpAddressPath);
  if (!this->GetValue(path, str))
  {
    str = ConfigIpAddressDefault;
  }
  return (str);
}

bool
ConfigData::SetIpAddress(const std::string& addr_)
{
  ConfigPath path(ConfigPath::ConfigIpAddressPath);
  return (this->PutValue(path, addr_));
}

std::string
ConfigData::GetNetmask() const
{
  std::string str;
  ConfigPath path(ConfigPath::ConfigNetmaskPath);
  if (!this->GetValue(path, str))
  {
    str = ConfigNetmaskDefault;
  }
  return (str);
}

bool
ConfigData::SetNetmask(const std::string& addr_)
{
  ConfigPath path(ConfigPath::ConfigNetmaskPath);
  return (this->PutValue(path, addr_));
}

ConfigData::STATE
ConfigData::GetAdminState() const
{
  ConfigData::STATE state = ConfigData::STATE_ERR;
  std::string str;

  ConfigPath path(ConfigPath::ConfigAdminStatePath);
  if (!this->GetValue(path, str))
  {
    str = ConfigData::ConfigAdminStateDefault;
  }

  if (str == ConfigData::ConfigAdminStateUp)
  {
    state = ConfigData::STATE_UP;
  }
  else if (str == ConfigData::ConfigAdminStateDown)
  {
    state = ConfigData::STATE_DOWN;
  }
  else if (str == ConfigData::ConfigAdminStateNone)
  {
    state = ConfigData::STATE_NONE;
  }

  return (state);

}

bool
ConfigData::SetAdminState(const ConfigData::STATE state_)
{

  ConfigPath path(ConfigPath::ConfigAdminStatePath);
  std::string str;

  switch (state_)
  {
  case ConfigData::STATE_UP:
    str = ConfigData::ConfigAdminStateUp;
    break;
  case ConfigData::STATE_DOWN:
    str = ConfigData::ConfigAdminStateDown;
    break;
  default:
    return (false);
  }

  return (this->PutValue(path, str));

}

void
ConfigData::_init()
{
  this->SetIfName(this->GetIfName());
  this->SetIfType(this->GetIfType());
  this->SetHwAddress(this->GetHwAddress());
  this->SetMtu(this->GetMtu());
  this->SetIpAddress(this->GetIpAddress());
  this->SetNetmask(this->GetNetmask());
  this->SetAdminState(this->GetAdminState());
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

}
}
