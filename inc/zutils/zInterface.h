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
  class GetLinkCommand;
  class SetLinkCommand;
  class RouteLinkEvent;
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
  static const std::string ConfigIfNamePath;
  static const std::string ConfigIfTypePath;
  static const std::string ConfigMtuPath;
  static const std::string ConfigHwAddressPath;
  static const std::string ConfigIpAddressPath;
  static const std::string ConfigBroadcastPath;
  static const std::string ConfigNetmaskPath;
  static const std::string ConfigAdminStatePath;

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

class ConfigData : public zConfig::ConfigData
{

public:

  enum IFTYPE
  {
    IFTYPE_ERR = -1,
    IFTYPE_NONE = 0,
    IFTYPE_DEF = 0,
    IFTYPE_LOOP = 1,
    IFTYPE_IEEE8023 = 2,
    IFTYPE_IEEE80211 = 3,
    IFTYPE_BRIDGE = 4,
    IFTYPE_BOND = 5,
    IFTYPE_OTHER = 6,
    IFTYPE_UNKNOWN = 7,
    IFTYPE_LAST
  };

  enum STATE
  {
    STATE_ERR = -1,
    STATE_NONE = 0,
    STATE_DEF = 0,
    STATE_UNKNOWN = 1,
    STATE_UP = 2,
    STATE_DOWN = 3,
    STATE_LAST
  };

  static const std::string ConfigNameDefault;

  static const std::string ConfigTypeNone;
  static const std::string ConfigTypeLoop;
  static const std::string ConfigTypeWired;
  static const std::string ConfigTypeWireless;
  static const std::string ConfigTypeOther;
  static const std::string ConfigTypeBond;
  static const std::string ConfigTypeBridge;
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

  ConfigData(const std::string& name_ = ConfigData::ConfigNameDefault);

  ConfigData(const zConfig::ConfigData& config_);

  virtual
  ~ConfigData();

  std::string
  GetIfName() const;

  virtual bool
  SetIfName(const std::string& name_);

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

  std::string
  GetIpAddress() const;

  bool
  SetIpAddress(const std::string& addr_);

  std::string
  GetNetmask() const;

  bool
  SetNetmask(const std::string& addr_);

  ConfigData::STATE
  GetAdminState() const;

  bool
  SetAdminState(const ConfigData::STATE state_);

protected:

private:

  void
  _init();

};

// ****************************************************************************
// Class: InterfaceNotification
// ****************************************************************************

class InterfaceNotification : public zEvent::EventNotification
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

class Interface : public zEvent::Event
{

public:

  zInterface::ConfigData Config;

  Interface(const std::string& name_);

  Interface(const zInterface::ConfigData& config_);

  virtual
  ~Interface();

  bool
  IsRefreshed() const;

  bool
  IsModified() const;

  unsigned int
  GetIfIndex() const;

  std::string
  GetIfName() const;

  bool
  SetIfName(const std::string& name_);

  ConfigData::IFTYPE
  GetIfType() const;

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

  virtual bool
  Refresh();

  virtual bool
  Commit();

  virtual bool
  Create();

  virtual bool
  Destroy();

  virtual void
  Display(const std::string& prefix_ = std::string(""));

protected:

  mutable zSem::Mutex _lock;
  bool _refreshed;
  bool _modified;

  netlink::GetLinkCommand* _getlinkcmd;
  netlink::SetLinkCommand* _setlinkcmd;
  netlink::RouteLinkEvent* _rtlinkevent;

  void
  _init();

private:

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

class InterfaceManager : public zEvent::EventHandler
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
