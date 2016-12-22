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
  static const std::string ConfigStatePath;
  static const std::string ConfigRatePath;

  InterfaceConfigPath();

  virtual
  ~InterfaceConfigPath();

  zConfig::ConfigPath
  Name() const;

  zConfig::ConfigPath
  Type() const;

  zConfig::ConfigPath
  State() const;

  zConfig::ConfigPath
  Rate() const;

protected:

private:

};

// ****************************************************************************
// Class: InterfaceConfiguration
// ****************************************************************************

class InterfaceConfigData : public zConfig::ConfigData
{

public:

  static const std::string ConfigNameDefault;

  static const std::string ConfigTypeNone;
  static const std::string ConfigTypeLoop;
  static const std::string ConfigTypeWired;
  static const std::string ConfigTypeWireless;
  static const std::string ConfigTypeOther;
  static const std::string ConfigTypeBond;
  static const std::string ConfigTypeBridge;
  static const std::string ConfigTypeDefault;

  static const std::string ConfigStateUp;
  static const std::string ConfigStateDown;
  static const std::string ConfigStateDefault;

  static const uint32_t ConfigRateDefault;

  InterfaceConfigData();

  InterfaceConfigData(zData::Data &data_);

  InterfaceConfigData(zConfig::ConfigData &config_);

  virtual
  ~InterfaceConfigData();

  std::string
  GetName();

  bool
  SetName(const std::string &name_);

  std::string
  GetType();

  bool
  SetType(const std::string &type_);

  std::string
  GetState();

  bool
  SetState(const std::string &state_);

  uint32_t
  GetRate();

  bool
  SetRate(const uint32_t &rate_);

protected:

private:

};

// ****************************************************************************
// Class: Interface
// ****************************************************************************

#define INTERFACE_REFRESH_PERIOD_HZ     (4)
#define INTERFACE_REFRESH_PERIOD_USEC	(1000000 / INTERFACE_REFRESH_PERIOD_HZ)
#define INTERFACE_COST_MAX              (100)
#define INTERFACE_COST_WINDOW           (3 * INTERFACE_REFRESH_PERIOD_HZ)

class Interface : public InterfaceConfigData, public zEvent::Event
{

public:

  enum TYPE
  {
    TYPE_ERR = -1,
    TYPE_NONE = 0,
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
    STATE_UNKNOWN = 1,
    STATE_UP = 2,
    STATE_DOWN = 3,
    STATE_LAST
  };

  Interface();

  Interface(Interface &other_);

  Interface(const Interface &other_);

  Interface(InterfaceConfigData &config_);

  virtual
  ~Interface();

  virtual bool
  Refresh();

  virtual void
  Display(const std::string& prefix_ = std::string(""));

  Interface::TYPE
  Type();

  bool
  Type(const Interface::TYPE& type_);

  Interface::STATE
  State();

  bool
  State(const Interface::STATE& state_);

  int
  Index();

  std::string
  HwAddress();

  std::string
  IpAddress();

protected:

private:

  zSem::Mutex _lock;

  int _index;
  std::string _hw_addr;
  std::string _ip_addr;
  Interface::STATE _state;

};

typedef std::map<std::string, SHARED_PTR(Interface)> InterfaceTable;

// ****************************************************************************
// Class: InterfaceHandler
// ****************************************************************************

class InterfaceHandler : private zEvent::EventObserver
{

public:

  InterfaceHandler();

  InterfaceHandler(zConfig::ConfigData &config_);

  virtual
  ~InterfaceHandler();

  bool
  GetConfig(zConfig::ConfigData &config_);

  bool
  SetConfig(zConfig::ConfigData &config_);

  bool
  Start();

  void
  Stop();

  bool
  Refresh();

  void
  Display();

  SHARED_PTR(Interface)
  Iface (const std::string &name_);

  InterfaceTable
  IfaceList (Interface::TYPE type_);

protected:

  virtual bool
  EventHandler(const zEvent::EventNotification* notification_);

private:

  zSem::Mutex _lock;

  zTimer::Timer _timer;
  InterfaceTable _ifaces;

};

//**********************************************************************
// Class: InterfaceManager
//**********************************************************************

class InterfaceManager : public InterfaceHandler
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
