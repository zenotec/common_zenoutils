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
const std::string InterfaceConfigPath::ConfigHwAddressPath("HwAddress");
const std::string InterfaceConfigPath::ConfigIpAddressPath("IpAddress");
const std::string InterfaceConfigPath::ConfigNetmaskPath("Netmask");
const std::string InterfaceConfigPath::ConfigStatePath("State");

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
InterfaceConfigPath::HwAddress() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigHwAddressPath);
  return (path);
}

zConfig::ConfigPath
InterfaceConfigPath::IpAddress() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigIpAddressPath);
  return (path);
}

zConfig::ConfigPath
InterfaceConfigPath::Netmask() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigNetmaskPath);
  return (path);
}

zConfig::ConfigPath
InterfaceConfigPath::State() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigStatePath);
  return (path);
}

// ****************************************************************************
// Class: InterfaceConfiguration
// ****************************************************************************

const std::string InterfaceConfigData::ConfigNameDefault("");

const std::string InterfaceConfigData::ConfigTypeNone("");
const std::string InterfaceConfigData::ConfigTypeLoop("LOOP");
const std::string InterfaceConfigData::ConfigTypeWired("WIRED");
const std::string InterfaceConfigData::ConfigTypeWireless("WIRELESS");
const std::string InterfaceConfigData::ConfigTypeOther("OTHER");
const std::string InterfaceConfigData::ConfigTypeBond("BOND");
const std::string InterfaceConfigData::ConfigTypeBridge("BRIDGE");
const std::string InterfaceConfigData::ConfigTypeDefault(ConfigTypeNone);

const std::string InterfaceConfigData::ConfigHwAddressDefault("");

const std::string InterfaceConfigData::ConfigIpAddressDefault("");

const std::string InterfaceConfigData::ConfigNetmaskDefault("");

const std::string InterfaceConfigData::ConfigStateNone("");
const std::string InterfaceConfigData::ConfigStateUp("UP");
const std::string InterfaceConfigData::ConfigStateDown("DOWN");
const std::string InterfaceConfigData::ConfigStateDefault(ConfigStateNone);

InterfaceConfigData::InterfaceConfigData() :
    zConfig::ConfigData(InterfaceConfigPath::ConfigRoot)
{
  ZLOG_DEBUG("InterfaceConfigData::InterfaceConfigData()");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

InterfaceConfigData::InterfaceConfigData(const zData::Data& data_) :
    zConfig::ConfigData(InterfaceConfigPath::ConfigRoot)
{
  this->PutChild(data_);
  ZLOG_DEBUG("InterfaceConfigData::InterfaceConfigData(data_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

InterfaceConfigData::InterfaceConfigData(const zConfig::ConfigData& config_) :
    zConfig::ConfigData(InterfaceConfigPath::ConfigRoot)
{
  this->PutChild(config_.GetData());
  ZLOG_DEBUG("InterfaceConfigData::InterfaceConfigData(config_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

InterfaceConfigData::InterfaceConfigData(const InterfaceConfigData& other_) :
    zConfig::ConfigData(other_)
{
  ZLOG_DEBUG("InterfaceConfigData::InterfaceConfigData(other_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

InterfaceConfigData::~InterfaceConfigData()
{
}

std::string
InterfaceConfigData::GetName() const
{
  std::string str;
  InterfaceConfigPath path;
  if (!this->GetValue(path.Name(), str))
  {
    str = ConfigNameDefault;
  }
  return (str);
}

bool
InterfaceConfigData::SetName(const std::string& name_)
{
  InterfaceConfigPath path;
  return (this->PutValue(path.Name(), name_));
}

InterfaceConfigData::TYPE
InterfaceConfigData::GetType() const
{
  InterfaceConfigData::TYPE type = InterfaceConfigData::TYPE_DEF;
  std::string str;
  InterfaceConfigPath path;
  if (this->GetValue(path.Type(), str))
  {
    if (str == InterfaceConfigData::ConfigTypeNone)
    {
      type = InterfaceConfigData::TYPE_NONE;
    }
    else if (str == InterfaceConfigData::ConfigTypeLoop)
    {
      type = InterfaceConfigData::TYPE_LOOP;
    }
    else if (str == InterfaceConfigData::ConfigTypeWired)
    {
      type = InterfaceConfigData::TYPE_WIRED;
    }
    else if (str == InterfaceConfigData::ConfigTypeWireless)
    {
      type = InterfaceConfigData::TYPE_WIRELESS;
    }
    else if (str == InterfaceConfigData::ConfigTypeOther)
    {
      type = InterfaceConfigData::TYPE_OTHER;
    }
    else if (str == InterfaceConfigData::ConfigTypeBond)
    {
      type = InterfaceConfigData::TYPE_BOND;
    }
    else if (str == InterfaceConfigData::ConfigTypeBridge)
    {
      type = InterfaceConfigData::TYPE_BRIDGE;
    }
    else
    {
      type = InterfaceConfigData::TYPE_ERR;
    }
  }
  return (type);
}

bool
InterfaceConfigData::SetType(const InterfaceConfigData::TYPE type_)
{
  bool status = true;
  InterfaceConfigPath path;
  std::string str;
  switch (type_)
  {
  case InterfaceConfigData::TYPE_NONE:
    str = ConfigTypeNone;
    break;
  case InterfaceConfigData::TYPE_LOOP:
    str = ConfigTypeLoop;
    break;
  case InterfaceConfigData::TYPE_WIRED:
    str = ConfigTypeWired;
    break;
  case InterfaceConfigData::TYPE_WIRELESS:
    str = ConfigTypeWireless;
    break;
  case InterfaceConfigData::TYPE_OTHER:
    str = ConfigTypeOther;
    break;
  case InterfaceConfigData::TYPE_BOND:
    str = ConfigTypeBond;
    break;
  case InterfaceConfigData::TYPE_BRIDGE:
    str = ConfigTypeBridge;
    break;
  default:
    status = false;
  }
  return (this->PutValue(path.Type(), str));
}

std::string
InterfaceConfigData::GetHwAddress() const
{
  std::string str;
  InterfaceConfigPath path;
  if (!this->GetValue(path.HwAddress(), str))
  {
    str = ConfigHwAddressDefault;
  }
  return (str);
}

bool
InterfaceConfigData::SetHwAddress(const std::string& addr_)
{
  InterfaceConfigPath path;
  return (this->PutValue(path.HwAddress(), addr_));
}

std::string
InterfaceConfigData::GetIpAddress() const
{
  std::string str;
  InterfaceConfigPath path;
  if (!this->GetValue(path.IpAddress(), str))
  {
    str = ConfigIpAddressDefault;
  }
  return (str);
}

bool
InterfaceConfigData::SetIpAddress(const std::string& addr_)
{
  InterfaceConfigPath path;
  return (this->PutValue(path.IpAddress(), addr_));
}

std::string
InterfaceConfigData::GetNetmask() const
{
  std::string str;
  InterfaceConfigPath path;
  if (!this->GetValue(path.Netmask(), str))
  {
    str = ConfigNetmaskDefault;
  }
  return (str);
}

bool
InterfaceConfigData::SetNetmask(const std::string& addr_)
{
  InterfaceConfigPath path;
  return (this->PutValue(path.Netmask(), addr_));
}

InterfaceConfigData::STATE
InterfaceConfigData::GetState() const
{
  InterfaceConfigData::STATE state = InterfaceConfigData::STATE_DEF;
  std::string str;
  InterfaceConfigPath path;
  if (this->GetValue(path.State(), str))
  {
    if (str == InterfaceConfigData::ConfigStateUp)
    {
      state = InterfaceConfigData::STATE_UP;
    }
    else if (str == InterfaceConfigData::ConfigStateDown)
    {
      state = InterfaceConfigData::STATE_DOWN;
    }
    else if (str == InterfaceConfigData::ConfigStateNone)
    {
      state = InterfaceConfigData::STATE_NONE;
    }
    else
    {
      state = InterfaceConfigData::STATE_ERR;
    }
  }
  return (state);
}

bool
InterfaceConfigData::SetState(const InterfaceConfigData::STATE state_)
{
  bool status = true;
  InterfaceConfigPath path;
  std::string str;
  switch (state_)
  {
  case InterfaceConfigData::STATE_UP:
    str = ConfigStateUp;
    break;
  case InterfaceConfigData::STATE_DOWN:
    str = ConfigStateDown;
    break;
  case InterfaceConfigData::STATE_NONE:
    str = ConfigStateDefault;
    break;
  case InterfaceConfigData::STATE_UNKNOWN:
    // No break
  default:
    status = false;
  }
  return (this->PutValue(path.State(), str));
}


}
}
