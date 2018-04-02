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

#ifndef __ZINTERFACE_H__
#define __ZINTERFACE_H__

#include <map>

#include <zutils/zSem.h>
#include <zutils/zData.h>
#include <zutils/zConfig.h>
#include <zutils/zTimer.h>

namespace netlink
{
  class Command;
}

namespace zUtils
{
namespace zInterface
{

// ****************************************************************************
// Class: ConfigPath
// ****************************************************************************

class ConfigPath : public zConfig::ConfigPath
{

public:

  static const std::string ConfigRoot;
  static const std::string ConfigIfIndexPath;
  static const std::string ConfigMasterIfIndexPath;
  static const std::string ConfigIfNamePath;
  static const std::string ConfigMasterIfNamePath;
  static const std::string ConfigIfTypePath;
  static const std::string ConfigMtuPath;
  static const std::string ConfigHwAddressPath;
  static const std::string ConfigIpAddressPath;
  static const std::string ConfigBroadcastPath;
  static const std::string ConfigNetmaskPath;
  static const std::string ConfigAdminStatePath;
  static const std::string ConfigPromiscuousModePath;

  ConfigPath(const std::string& root_ = std::string(""));

  ConfigPath(const zData::DataPath& path_);

  ConfigPath(const ConfigPath& other_);

  virtual
  ~ConfigPath();

protected:

private:

};

// ****************************************************************************
// Class: ConfigData
// ****************************************************************************

class ConfigData
{

public:

  enum IFTYPE
  {
    IFTYPE_ERR = -1,
    IFTYPE_NONE = 0,
    IFTYPE_DEF = IFTYPE_NONE,
    IFTYPE_LOOP,
    IFTYPE_IEEE8023,
    IFTYPE_IEEE80211,
    IFTYPE_BRIDGE,
    IFTYPE_BOND,
    IFTYPE_VLAN,
    IFTYPE_MACVLAN,
    IFTYPE_OTHER,
    IFTYPE_UNKNOWN,
    IFTYPE_LAST
  };

  enum STATE
  {
    STATE_ERR = -1,
    STATE_NONE = 0,
    STATE_DEF = STATE_NONE,
    STATE_UNKNOWN = 1,
    STATE_UP = 2,
    STATE_DOWN = 3,
    STATE_LAST
  };

  enum PROMODE
  {
    PROMODE_ERR = -1,
    PROMODE_NONE = 0,
    PROMODE_DEF = 0,
    PROMODE_DISABLED = 1,
    PROMODE_ENABLED = 2,
    PROMODE_LAST
  };

  static const unsigned int ConfigIfIndexDefault;

  static const unsigned int ConfigMasterIfIndexDefault;

  static const std::string ConfigIfNameDefault;

  static const std::string ConfigMasterIfNameDefault;

  static const std::string ConfigTypeNone;
  static const std::string ConfigTypeLoop;
  static const std::string ConfigTypeWired;
  static const std::string ConfigTypeWireless;
  static const std::string ConfigTypeOther;
  static const std::string ConfigTypeBond;
  static const std::string ConfigTypeBridge;
  static const std::string ConfigTypeVlan;
  static const std::string ConfigTypeMacVlan;
  static const std::string ConfigTypeDefault;

  static const unsigned int ConfigMtuDefault;

  static const std::string ConfigHwAddressDefault;

  static const std::string ConfigIpAddressDefault;

  static const std::string ConfigBroadcastDefault;

  static const std::string ConfigNetmaskDefault;

  static const std::string ConfigAdminStateNone;
  static const std::string ConfigAdminStateUp;
  static const std::string ConfigAdminStateDown;
  static const std::string ConfigAdminStateDefault;

  static const std::string ConfigPromiscuousModeNone;
  static const std::string ConfigPromiscuousModeEnable;
  static const std::string ConfigPromiscuousModeDisable;
  static const std::string ConfigPromiscuousModeDefault;

  ConfigData(const std::string& name_ = ConfigIfNameDefault);

  ConfigData(SHARED_PTR(zConfig::ConfigData) data_);

  virtual
  ~ConfigData();

  zInterface::ConfigData&
  operator=(const zInterface::ConfigData& other_);

  SHARED_PTR(zConfig::ConfigData)
  GetData() const;

  bool
  SetData(SHARED_PTR(zConfig::ConfigData) data_);

  unsigned int
  GetIfIndex(const unsigned int mtu_ = ConfigIfIndexDefault) const;

  bool
  SetIfIndex(const unsigned int mtu_ = ConfigIfIndexDefault);

  unsigned int
  GetMasterIfIndex(const unsigned int mtu_ = ConfigMasterIfIndexDefault) const;

  bool
  SetMasterIfIndex(const unsigned int mtu_ = ConfigMasterIfIndexDefault);

  std::string
  GetIfName(const std::string& name_ = ConfigIfNameDefault) const;

  virtual bool
  SetIfName(const std::string& name_ = ConfigIfNameDefault);

  std::string
  GetMasterIfName(const std::string& name_ = ConfigMasterIfNameDefault) const;

  virtual bool
  SetMasterIfName(const std::string& name_ = ConfigMasterIfNameDefault);

  ConfigData::IFTYPE
  GetIfType(const ConfigData::IFTYPE type_ = ConfigData::IFTYPE_DEF) const;

  bool
  SetIfType(const ConfigData::IFTYPE type_ = ConfigData::IFTYPE_DEF);

  std::string
  GetHwAddress(const std::string& addr_ = ConfigHwAddressDefault) const;

  bool
  SetHwAddress(const std::string& addr_ = ConfigHwAddressDefault);

  unsigned int
  GetMtu(const unsigned int mtu_ = ConfigMtuDefault) const;

  bool
  SetMtu(const unsigned int mtu_ = ConfigMtuDefault);

  std::string
  GetIpAddress(const std::string& addr_ = ConfigIpAddressDefault) const;

  bool
  SetIpAddress(const std::string& addr_ = ConfigIpAddressDefault);

  std::string
  GetNetmask(const std::string& addr_ = ConfigNetmaskDefault) const;

  bool
  SetNetmask(const std::string& addr_ = ConfigNetmaskDefault);

  ConfigData::STATE
  GetAdminState(const ConfigData::STATE state_ = ConfigData::STATE_DEF) const;

  bool
  SetAdminState(const ConfigData::STATE state_ = ConfigData::STATE_DEF);

  ConfigData::PROMODE
  GetPromiscuousMode(const ConfigData::PROMODE mode_ = ConfigData::PROMODE_DEF) const;

  bool
  SetPromiscuousMode(const ConfigData::PROMODE mode_ = ConfigData::PROMODE_DEF);

protected:

private:

  SHARED_PTR(zConfig::ConfigData) _data;

};

// ****************************************************************************
// Class: InterfaceNotification
// ****************************************************************************

class InterfaceNotification : public zEvent::Notification
{

public:

  InterfaceNotification();

  virtual
  ~InterfaceNotification();

protected:

private:

};

// ****************************************************************************
// Class: Interface
// ****************************************************************************

class Interface
{

public:

  Interface(const std::string& name_);

  virtual
  ~Interface();

  zInterface::ConfigData
  GetConfig() const;

  bool
  SetConfig(zInterface::ConfigData config_);

  unsigned int
  GetIfIndex() const;

  bool
  SetIfIndex(const unsigned int ifindex_);

  std::string
  GetIfName() const;

  bool
  SetIfName(const std::string& name_);

  unsigned int
  GetMasterIfIndex() const;

  bool
  SetMasterIfIndex(const unsigned int ifindex_);

  std::string
  GetMasterIfName() const;

  bool
  SetMasterIfName(const std::string& name_);

  ConfigData::IFTYPE
  GetIfType() const;

  bool
  SetIfType(const ConfigData::IFTYPE type_);

  std::string
  GetHwAddress() const;

  bool
  SetHwAddress(const std::string& addr_);

  unsigned int
  GetMtu() const;

  bool
  SetMtu(const unsigned int mtu_);

  ConfigData::STATE
  GetAdminState() const;

  bool
  SetAdminState(const ConfigData::STATE state_);

  ConfigData::PROMODE
  GetPromiscuousMode() const;

  bool
  SetPromiscuousMode(const ConfigData::PROMODE mode_ );

  std::string
  GetIpAddress() const;

  bool
  SetIpAddress(const std::string& addr_);

  std::string
  GetNetmask() const;

  bool
  SetNetmask(const std::string& addr_);

  virtual bool
  Refresh();

  bool
  Commit();

  virtual bool
  Create();

  virtual bool
  Destroy();

  virtual void
  Display(const std::string& prefix_ = std::string(""));

protected:

  mutable zSem::Mutex lock;

  zInterface::ConfigData stagingConfig; // Contains the desired interface configuration
  zInterface::ConfigData workingConfig; // Contains the current interface configuration

  size_t
  addCommand(netlink::Command* cmd_, size_t index_ = -1); // 0 - begin of list, -1 == end of list

  bool
  execCommands();

  bool
  clrCommands();

  unsigned int
  getIfIndex() const;

  bool
  setIfIndex(const unsigned int ifindex_);

  unsigned int
  getMasterIfIndex() const;

  bool
  setMasterIfIndex(const unsigned int ifindex_);

  std::string
  getIfName() const;

  bool
  setIfName(const std::string& name_);

  std::string
  getMasterIfName() const;

  bool
  setMasterIfName(const std::string& name_);

  ConfigData::IFTYPE
  getIfType() const;

  bool
  setIfType(const ConfigData::IFTYPE type_);

  std::string
  getHwAddress() const;

  bool
  setHwAddress(const std::string& addr_);

  unsigned int
  getMtu() const;

  bool
  setMtu(const unsigned int mtu_);

  ConfigData::STATE
  getAdminState() const;

  bool
  setAdminState(const ConfigData::STATE state_);

  ConfigData::PROMODE
  getPromiscuousMode() const;

  bool
  setPromiscuousMode(const ConfigData::PROMODE mode_ );

  std::string
  getIpAddress() const;

  bool
  setIpAddress(const std::string& addr_);

  std::string
  getNetmask() const;

  bool
  setNetmask(const std::string& addr_);

private:

  std::list<netlink::Command*> _cmds; // List of commands to invoke during commit

};

// ****************************************************************************
// Class: InterfaceTable
// ****************************************************************************

typedef std::map<std::string, SHARED_PTR(Interface)> InterfaceTable;

// ****************************************************************************
// Class: InterfaceFactory
// ****************************************************************************

class InterfaceFactory
{

public:

  static InterfaceTable
  Create(const zConfig::ConfigData& config_);

protected:

private:

};

//**********************************************************************
// Class: InterfaceManager
//**********************************************************************

class InterfaceManager : public zEvent::Handler
{

public:

  static InterfaceManager&
  Instance()
  {
    static InterfaceManager instance;
    return instance;
  }

protected:

private:

  InterfaceManager()
  {
  }

  InterfaceManager(InterfaceManager const&);

  void
  operator=(InterfaceManager const&);

  InterfaceTable _ifaces;

};

}
}

#endif /* __ZINTERFACE_H__ */
