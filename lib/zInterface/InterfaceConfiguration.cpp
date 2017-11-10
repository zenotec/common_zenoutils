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
const std::string ConfigPath::ConfigNamePath("Name");
const std::string ConfigPath::ConfigTypePath("Type");
const std::string ConfigPath::ConfigHwAddressPath("HwAddress");
const std::string ConfigPath::ConfigMtuPath("MTU");
const std::string ConfigPath::ConfigIpAddressPath("IpAddress");
const std::string ConfigPath::ConfigBroadcastPath("Broadcast");
const std::string ConfigPath::ConfigNetmaskPath("Netmask");
const std::string ConfigPath::ConfigAdminStatePath("AdminState");

ConfigPath::ConfigPath(const std::string& root_) :
    zConfig::ConfigPath(ConfigRoot)
{
  if (!root_.empty())
  {
    this->Append(root_);
  }
}

ConfigPath::ConfigPath(const ConfigPath& other_) :
    zConfig::ConfigPath(other_)
{
}

ConfigPath::ConfigPath(const zData::DataPath& path_) :
    zConfig::ConfigPath(ConfigRoot)
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
const std::string ConfigData::ConfigTypeDefault(ConfigTypeNone);

const std::string ConfigData::ConfigHwAddressDefault("");

const unsigned int ConfigData::ConfigMtuDefault(1500);

const std::string ConfigData::ConfigIpAddressDefault("");

const std::string ConfigData::ConfigBroadcastDefault("255.255.255.0");

const std::string ConfigData::ConfigNetmaskDefault("");

const std::string ConfigData::ConfigAdminStateNone("");
const std::string ConfigData::ConfigAdminStateUp("UP");
const std::string ConfigData::ConfigAdminStateDown("DOWN");
const std::string ConfigData::ConfigAdminStateDefault(ConfigAdminStateNone);

ConfigData::ConfigData() :
    zConfig::ConfigData(ConfigPath::ConfigRoot)
{
  ZLOG_DEBUG("InterfaceConfigData::InterfaceConfigData()");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
  this->Name(this->Name());
  this->Type(this->Type());
  this->HwAddress(this->HwAddress());
  this->Mtu(this->Mtu());
  this->IpAddress(this->IpAddress());
  this->Broadcast(this->Broadcast());
  this->Netmask(this->Netmask());
  this->AdminState(this->AdminState());
}

ConfigData::ConfigData(const zData::Data& data_) :
    zConfig::ConfigData(ConfigPath::ConfigRoot)
{
  ConfigPath path;
  this->PutChild(path, path, data_);
  ZLOG_DEBUG("InterfaceConfigData::InterfaceConfigData(data_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
  this->Name(this->Name());
  this->Type(this->Type());
  this->HwAddress(this->HwAddress());
  this->Mtu(this->Mtu());
  this->IpAddress(this->IpAddress());
  this->Broadcast(this->Broadcast());
  this->Netmask(this->Netmask());
  this->AdminState(this->AdminState());
}

ConfigData::ConfigData(const zConfig::ConfigData& config_) :
    zConfig::ConfigData(ConfigPath::ConfigRoot)
{
  ConfigPath path;
  this->PutChild(path, path, config_);
  ZLOG_DEBUG("InterfaceConfigData::InterfaceConfigData(config_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
  this->Name(this->Name());
  this->Type(this->Type());
  this->HwAddress(this->HwAddress());
  this->Mtu(this->Mtu());
  this->IpAddress(this->IpAddress());
  this->Broadcast(this->Broadcast());
  this->Netmask(this->Netmask());
  this->AdminState(this->AdminState());
}

ConfigData::ConfigData(const ConfigData& other_) :
    zConfig::ConfigData(other_)
{
  ZLOG_DEBUG("InterfaceConfigData::InterfaceConfigData(other_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
  this->Name(this->Name());
  this->Type(this->Type());
  this->HwAddress(this->HwAddress());
  this->Mtu(this->Mtu());
  this->IpAddress(this->IpAddress());
  this->Broadcast(this->Broadcast());
  this->Netmask(this->Netmask());
  this->AdminState(this->AdminState());
}

ConfigData::~ConfigData()
{
}

std::string
ConfigData::Name() const
{
  std::string str;
  ConfigPath path(ConfigPath::ConfigNamePath);
  if (!this->GetValue(path, str))
  {
    str = ConfigNameDefault;
  }
  return (str);
}

bool
ConfigData::Name(const std::string& name_)
{
  ConfigPath path(ConfigPath::ConfigNamePath);
  return (this->PutValue(path, name_));
}

std::string
ConfigData::Type() const
{
  std::string str;
  ConfigPath path(ConfigPath::ConfigTypePath);
  if (!this->GetValue(path, str))
  {
    str = ConfigTypeDefault;
  }
  return (str);
}

bool
ConfigData::Type(const std::string& type_)
{
  ConfigPath path(ConfigPath::ConfigTypePath);
  return (this->PutValue(path, type_));
}

std::string
ConfigData::HwAddress() const
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
ConfigData::HwAddress(const std::string& addr_)
{
  ConfigPath path(ConfigPath::ConfigHwAddressPath);
  return (this->PutValue(path, addr_));
}

unsigned int
ConfigData::Mtu() const
{
  unsigned int val = 0;
  ConfigPath path(ConfigPath::ConfigMtuPath);
  if (!this->GetValue<unsigned int>(path, val))
  {
    val = ConfigMtuDefault;
  }
  return (val);
}

bool
ConfigData::Mtu(const unsigned int mtu_)
{
  ConfigPath path(ConfigPath::ConfigMtuPath);
  return (this->PutValue(path, mtu_));
}

std::string
ConfigData::IpAddress() const
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
ConfigData::IpAddress(const std::string& addr_)
{
  ConfigPath path(ConfigPath::ConfigIpAddressPath);
  return (this->PutValue(path, addr_));
}

std::string
ConfigData::Broadcast() const
{
  std::string str;
  ConfigPath path(ConfigPath::ConfigBroadcastPath);
  if (!this->GetValue(path, str))
  {
    str = ConfigBroadcastDefault;
  }
  return (str);
}

bool
ConfigData::Broadcast(const std::string& addr_)
{
  ConfigPath path(ConfigPath::ConfigBroadcastPath);
  return (this->PutValue(path, addr_));
}

std::string
ConfigData::Netmask() const
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
ConfigData::Netmask(const std::string& addr_)
{
  ConfigPath path(ConfigPath::ConfigNetmaskPath);
  return (this->PutValue(path, addr_));
}

std::string
ConfigData::AdminState() const
{
  std::string str;
  ConfigPath path(ConfigPath::ConfigAdminStatePath);
  if (!this->GetValue(path, str))
  {
    str = ConfigAdminStateDefault;
  }
  return (str);
}

bool
ConfigData::AdminState(const std::string& state_)
{
  ConfigPath path(ConfigPath::ConfigAdminStatePath);
  return (this->PutValue(path, state_));
}


}
}
