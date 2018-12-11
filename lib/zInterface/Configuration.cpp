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
#include <zutils/zUtils.h>
#include <zutils/zLog.h>
#include <zutils/zInterface.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_INTERFACE);

namespace zUtils
{
namespace zInterface
{

static ConfigData::IFTYPE
_str2iftype(const std::string& str_)
{
  ConfigData::IFTYPE type = ConfigData::IFTYPE_ERR;
  if (str_ == ConfigData::ConfigTypeNone)
  {
    type = ConfigData::IFTYPE_NONE;
  }
  else if (str_ == ConfigData::ConfigTypeLoop)
  {
    type = ConfigData::IFTYPE_LOOP;
  }
  else if (str_ == ConfigData::ConfigTypeWired)
  {
    type = ConfigData::IFTYPE_IEEE8023;
  }
  else if (str_ == ConfigData::ConfigTypeWireless)
  {
    type = ConfigData::IFTYPE_IEEE80211;
  }
  else if (str_ == ConfigData::ConfigTypeOther)
  {
    type = ConfigData::IFTYPE_OTHER;
  }
  else if (str_ == ConfigData::ConfigTypeBond)
  {
    type = ConfigData::IFTYPE_BOND;
  }
  else if (str_ == ConfigData::ConfigTypeBridge)
  {
    type = ConfigData::IFTYPE_BRIDGE;
  }
  else
  {
    type = ConfigData::IFTYPE_DEF;
  }
  return (type);
}

static std::string
_iftype2str(const ConfigData::IFTYPE type_)
{
  std::string str;
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
    str = ConfigData::ConfigTypeDefault;
    break;
  }
  return (str);
}

static ConfigData::STATE
_str2state(const std::string& str_)
{
  ConfigData::STATE state = ConfigData::STATE_ERR;
  if (str_ == ConfigData::ConfigAdminStateUp)
  {
    state = ConfigData::STATE_UP;
  }
  else if (str_ == ConfigData::ConfigAdminStateDown)
  {
    state = ConfigData::STATE_DOWN;
  }
  else if (str_ == ConfigData::ConfigAdminStateNone)
  {
    state = ConfigData::STATE_NONE;
  }
  else
  {
    state = ConfigData::STATE_DEF;
  }
  return (state);
}

static std::string
_state2str(const ConfigData::STATE state_)
{
  std::string str;
  switch (state_)
  {
  case ConfigData::STATE_UP:
    str = ConfigData::ConfigAdminStateUp;
    break;
  case ConfigData::STATE_DOWN:
    str = ConfigData::ConfigAdminStateDown;
    break;
  case ConfigData::STATE_NONE:
    str = ConfigData::ConfigAdminStateNone;
    break;
  default:
    str = ConfigData::ConfigAdminStateDefault;
    break;
  }
  return (str);
}

static ConfigData::PROMODE
_str2promode(const std::string& str_)
{
  ConfigData::PROMODE mode = ConfigData::PROMODE_ERR;
  if (str_ == ConfigData::ConfigPromiscuousModeEnable)
  {
    mode = ConfigData::PROMODE_ENABLED;
  }
  else if (str_ == ConfigData::ConfigPromiscuousModeDisable)
  {
    mode = ConfigData::PROMODE_DISABLED;
  }
  else if (str_ == ConfigData::ConfigPromiscuousModeNone)
  {
    mode = ConfigData::PROMODE_NONE;
  }
  else
  {
    mode = ConfigData::PROMODE_DEF;
  }
  return (mode);
}

static std::string
_promode2str(const ConfigData::PROMODE state_)
{
  std::string str;
  switch (state_)
  {
  case ConfigData::PROMODE_ENABLED:
    str = ConfigData::ConfigPromiscuousModeEnable;
    break;
  case ConfigData::PROMODE_DISABLED:
    str = ConfigData::ConfigPromiscuousModeDisable;
    break;
  case ConfigData::PROMODE_NONE:
    str = ConfigData::ConfigPromiscuousModeNone;
    break;
  default:
    str = ConfigData::ConfigPromiscuousModeDefault;
    break;
  }
  return (str);
}

// ****************************************************************************
// Class: ConfigPath
// ****************************************************************************

const std::string ConfigPath::ConfigRoot("zInterface");
const std::string ConfigPath::ConfigIfIndexPath("IfIndex");
const std::string ConfigPath::ConfigMasterIfIndexPath("MasterIfIndex");
const std::string ConfigPath::ConfigIfNamePath("IfName");
const std::string ConfigPath::ConfigMasterIfNamePath("MasterIfName");
const std::string ConfigPath::ConfigIfTypePath("IfType");
const std::string ConfigPath::ConfigHwAddressPath("HwAddress");
const std::string ConfigPath::ConfigMtuPath("MTU");
const std::string ConfigPath::ConfigIpAddressPath("IpAddress");
const std::string ConfigPath::ConfigBroadcastPath("Broadcast");
const std::string ConfigPath::ConfigNetmaskPath("Netmask");
const std::string ConfigPath::ConfigAdminStatePath("AdminState");
const std::string ConfigPath::ConfigPromiscuousModePath("PromiscuousMode");

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

const unsigned int ConfigData::ConfigIfIndexDefault(0);

const unsigned int ConfigData::ConfigMasterIfIndexDefault(0);

const std::string ConfigData::ConfigIfNameDefault("");

const std::string ConfigData::ConfigMasterIfNameDefault("");

const std::string ConfigData::ConfigTypeNone("");
const std::string ConfigData::ConfigTypeLoop("LOOP");
const std::string ConfigData::ConfigTypeWired("WIRED");
const std::string ConfigData::ConfigTypeWireless("WIRELESS");
const std::string ConfigData::ConfigTypeOther("OTHER");
const std::string ConfigData::ConfigTypeBond("BOND");
const std::string ConfigData::ConfigTypeBridge("BRIDGE");
const std::string ConfigData::ConfigTypeVlan("VLAN");
const std::string ConfigData::ConfigTypeMacVlan("MACVLAN");
const std::string ConfigData::ConfigTypeDefault(ConfigData::ConfigTypeNone);

const std::string ConfigData::ConfigHwAddressDefault("");

const unsigned int ConfigData::ConfigMtuDefault(0);

const std::string ConfigData::ConfigIpAddressDefault("");

const std::string ConfigData::ConfigBroadcastDefault("");

const std::string ConfigData::ConfigNetmaskDefault("");

const std::string ConfigData::ConfigAdminStateNone("");
const std::string ConfigData::ConfigAdminStateUp("UP");
const std::string ConfigData::ConfigAdminStateDown("DOWN");
const std::string ConfigData::ConfigAdminStateDefault(ConfigData::ConfigAdminStateNone);

const std::string ConfigData::ConfigPromiscuousModeNone("");
const std::string ConfigData::ConfigPromiscuousModeEnable("ENABLE");
const std::string ConfigData::ConfigPromiscuousModeDisable("DISABLE");
const std::string ConfigData::ConfigPromiscuousModeDefault(ConfigData::ConfigPromiscuousModeNone);

ConfigData::ConfigData(const std::string& name_) :
    _data(NULL)
{
  ZLOG_DEBUG("zInterface::ConfigData::ConfigData(name_)");
  SHARED_PTR(zConfig::ConfigData) data(new zConfig::ConfigData(ConfigPath::ConfigRoot));
  this->SetData(data);
  this->SetIfName(name_);
  this->_init_defaults();
}

ConfigData::ConfigData(SHARED_PTR(zConfig::ConfigData) data_) :
    _data(NULL)
{
  ZLOG_DEBUG("zInterface::ConfigData::ConfigData(data_)");
  this->SetData(data_);
  this->_init_defaults();
}

ConfigData::~ConfigData()
{
}

SHARED_PTR(zConfig::ConfigData)
ConfigData::GetData() const
{
  return (this->_data);
}

bool
ConfigData::SetData(SHARED_PTR(zConfig::ConfigData) data_)
{
  this->_data = data_;
  if (!this->_data)
  {
    std::string errmsg = "NULL interface configuration data pointer";
    ZLOG_CRIT(errmsg);
    throw errmsg;
  }
  ZLOG_DEBUG(this->_data->Path());
  ZLOG_DEBUG(this->_data->GetJson());
  return (true);
}

unsigned int
ConfigData::GetIfIndex(const unsigned int index_) const
{
  unsigned int val = 0;
  ConfigPath path(ConfigPath::ConfigIfIndexPath);
  if (!this->_data->GetValue(path, val))
  {
    val = index_;
  }
  return (val);
}

bool
ConfigData::SetIfIndex(const unsigned int index_)
{
  ConfigPath path(ConfigPath::ConfigIfIndexPath);
  return (this->_data->PutValue(path, index_));
}

std::string
ConfigData::GetIfName(const std::string& name_) const
{
  std::string str;
  ConfigPath path(ConfigPath::ConfigIfNamePath);
  if (!this->_data->GetValue(path, str))
  {
    str = name_;
  }
  return (str);
}

bool
ConfigData::SetIfName(const std::string& name_)
{
  ConfigPath path(ConfigPath::ConfigIfNamePath);
  return (this->_data->PutValue(path, name_));
}

unsigned int
ConfigData::GetMasterIfIndex(const unsigned int index_) const
{
  unsigned int val = 0;
  ConfigPath path(ConfigPath::ConfigMasterIfIndexPath);
  if (!this->_data->GetValue(path, val))
  {
    val = index_;
  }
  return (val);
}

bool
ConfigData::SetMasterIfIndex(const unsigned int index_)
{
  ConfigPath path(ConfigPath::ConfigMasterIfIndexPath);
  return (this->_data->PutValue(path, index_));
}

std::string
ConfigData::GetMasterIfName(const std::string& name_) const
{
  std::string str;
  ConfigPath path(ConfigPath::ConfigMasterIfNamePath);
  if (!this->_data->GetValue(path, str))
  {
    str = name_;
  }
  return (str);
}

bool
ConfigData::SetMasterIfName(const std::string& name_)
{
  ConfigPath path(ConfigPath::ConfigMasterIfNamePath);
  return (this->_data->PutValue(path, name_));
}

ConfigData::IFTYPE
ConfigData::GetIfType(const ConfigData::IFTYPE type_) const
{

  ConfigData::IFTYPE type = type_;
  ConfigPath path(ConfigPath::ConfigIfTypePath);
  std::string str;

  if (this->_data->GetValue(path, str))
  {
    type = _str2iftype(str);
  }

  return (type);
}

bool
ConfigData::SetIfType(const ConfigData::IFTYPE type_)
{
  ConfigPath path(ConfigPath::ConfigIfTypePath);
  std::string str = _iftype2str(type_);
  return (this->_data->PutValue(path, str));

}

unsigned int
ConfigData::GetMtu(const unsigned int mtu_) const
{
  unsigned int val = 0;
  ConfigPath path(ConfigPath::ConfigMtuPath);
  if (!this->_data->GetValue(path, val))
  {
    val = mtu_;
  }
  return (val);
}

bool
ConfigData::SetMtu(const unsigned int mtu_)
{
  ConfigPath path(ConfigPath::ConfigMtuPath);
  return (this->_data->PutValue(path, mtu_));
}

std::string
ConfigData::GetHwAddress(const std::string& addr_) const
{
  std::string str;
  ConfigPath path(ConfigPath::ConfigHwAddressPath);
  if (!this->_data->GetValue(path, str))
  {
    str = addr_;
  }
  return (str);
}

bool
ConfigData::SetHwAddress(const std::string& addr_)
{
  ConfigPath path(ConfigPath::ConfigHwAddressPath);
  return (this->_data->PutValue(path, addr_));
}

std::string
ConfigData::GetIpAddress(const std::string& addr_) const
{
  std::string str;
  ConfigPath path(ConfigPath::ConfigIpAddressPath);
  if (!this->_data->GetValue(path, str))
  {
    str = addr_;
  }
  return (str);
}

bool
ConfigData::SetIpAddress(const std::string& addr_)
{
  ConfigPath path(ConfigPath::ConfigIpAddressPath);
  return (this->_data->PutValue(path, addr_));
}

std::string
ConfigData::GetNetmask(const std::string& addr_) const
{
  std::string str;
  ConfigPath path(ConfigPath::ConfigNetmaskPath);
  if (!this->_data->GetValue(path, str))
  {
    str = addr_;
  }
  return (str);
}

bool
ConfigData::SetNetmask(const std::string& addr_)
{
  ConfigPath path(ConfigPath::ConfigNetmaskPath);
  return (this->_data->PutValue(path, addr_));
}

ConfigData::STATE
ConfigData::GetAdminState(const ConfigData::STATE state_) const
{
  ConfigData::STATE state = state_;
  ConfigPath path(ConfigPath::ConfigAdminStatePath);
  std::string str;
  if (this->_data->GetValue(path, str))
  {
    state = _str2state(str);
  }
  return (state);
}

bool
ConfigData::SetAdminState(const ConfigData::STATE state_)
{
  ConfigPath path(ConfigPath::ConfigAdminStatePath);
  std::string str = _state2str(state_);
  return (this->_data->PutValue(path, str));
}

ConfigData::PROMODE
ConfigData::GetPromiscuousMode(const ConfigData::PROMODE mode_) const
{
  ConfigData::PROMODE mode = mode_;
  ConfigPath path(ConfigPath::ConfigPromiscuousModePath);
  std::string str;
  if (this->_data->GetValue(path, str))
  {
    mode = _str2promode(str);
  }
  return (mode);
}

bool
ConfigData::SetPromiscuousMode(const ConfigData::PROMODE mode_)
{
  ConfigPath path(ConfigPath::ConfigPromiscuousModePath);
  std::string str = _promode2str(mode_);
  return (this->_data->PutValue(path, str));
}

void
ConfigData::_init_defaults()
{
  this->SetIfIndex(this->GetIfIndex());
  this->SetMasterIfIndex(this->GetMasterIfIndex());
  this->SetIfName(this->GetIfName());
  this->SetIfType(this->GetIfType());
  this->SetMtu(this->GetMtu());
  this->SetIpAddress(this->GetIpAddress());
  this->SetNetmask(this->GetNetmask());
  this->SetAdminState(this->GetAdminState());
  this->SetPromiscuousMode(this->GetPromiscuousMode());
}

}
}
