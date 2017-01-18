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

namespace zUtils
{
namespace zInterface
{

// ****************************************************************************
// Class: InterfaceConfigPath
// ****************************************************************************

class InterfaceConfigPath : public zConfig::ConfigPath
{

public:

  static const std::string ConfigRoot;
  static const std::string ConfigNamePath;
  static const std::string ConfigTypePath;
  static const std::string ConfigAddressPath;
  static const std::string ConfigStatePath;

  InterfaceConfigPath();

  virtual
  ~InterfaceConfigPath();

  zConfig::ConfigPath
  Name() const;

  zConfig::ConfigPath
  Type() const;

  zConfig::ConfigPath
  Address() const;

  zConfig::ConfigPath
  State() const;

protected:

private:

};

// ****************************************************************************
// Class: InterfaceConfiguration
// ****************************************************************************

class InterfaceConfigData : public zConfig::ConfigData
{

public:

  enum TYPE
  {
    TYPE_ERR = -1,
    TYPE_NONE = 0,
    TYPE_DEF = 0,
    TYPE_LOOP = 1,
    TYPE_WIRED = 2,
    TYPE_WIRELESS = 3,
    TYPE_OTHER = 4,
    TYPE_BRIDGE = 5,
    TYPE_BOND = 6,
    TYPE_LAST
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

  static const std::string ConfigAddressDefault;

  static const std::string ConfigStateNone;
  static const std::string ConfigStateUp;
  static const std::string ConfigStateDown;
  static const std::string ConfigStateDefault;

  InterfaceConfigData();

  InterfaceConfigData(const zData::Data& data_);

  InterfaceConfigData(const zConfig::ConfigData& config_);

  InterfaceConfigData(const InterfaceConfigData& other_);

  virtual
  ~InterfaceConfigData();

  std::string
  GetName() const;

  bool
  SetName(const std::string& name_);

  InterfaceConfigData::TYPE
  GetType() const;

  bool
  SetType(const InterfaceConfigData::TYPE type_);

  std::string
  GetAddress() const;

  bool
  SetAddress(const std::string& name_);

  InterfaceConfigData::STATE
  GetState() const;

  bool
  SetState(const InterfaceConfigData::STATE state_);

protected:

private:

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

#define INTERFACE_REFRESH_PERIOD_HZ     (1)
#define INTERFACE_REFRESH_PERIOD_USEC	(1000000 / INTERFACE_REFRESH_PERIOD_HZ)

class Interface : public InterfaceConfigData, public zEvent::Event, public zEvent::EventObserver
{

public:

  Interface(const InterfaceConfigData &config_);

  virtual
  ~Interface();

  int
  Index();

  std::string
  HwAddress();

  virtual bool
  Refresh();

  virtual void
  Display(const std::string& prefix_ = std::string(""));

protected:

  virtual bool
  EventHandler(zEvent::EventNotification* notification_);

private:

  zSem::Mutex _lock;
  zTimer::Timer _timer;

  int _index;
  std::string _hw_addr;

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

};

}
}

#endif /* __ZINTERFACE_H__ */
