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

InterfaceHandler::InterfaceHandler() :
    _lock(zSem::Mutex::LOCKED)
{
  this->_timer.RegisterObserver(this);
  this->_lock.Unlock();
}

InterfaceHandler::InterfaceHandler(zConfig::ConfigData &config_) :
    _lock(zSem::Mutex::LOCKED)
{
  this->_timer.RegisterObserver(this);
  this->_lock.Unlock();
  this->SetConfig(config_);
}

InterfaceHandler::~InterfaceHandler()
{
  this->_lock.Lock();
  this->_timer.UnregisterObserver(this);
}

bool
InterfaceHandler::GetConfig(zConfig::ConfigData &config_)
{

  InterfaceConfigPath path;
  zConfig::ConfigData config(path);
  InterfaceTable::iterator it = this->_ifaces.begin();
  InterfaceTable::iterator end = this->_ifaces.end();
  for (; it != end; ++it)
  {
    config.Add(it->second->GetData());
  }
  config_ = config;
  config.DisplayJson();
  config_.DisplayJson();
  return (true);
}

bool
InterfaceHandler::SetConfig(zConfig::ConfigData &config_)
{
  std::cout << std::endl << "SetConfig(): " << config_.Size() << std::endl;
  config_.DisplayJson();

  InterfaceConfigPath path;
  zConfig::ConfigData IfaceConfig(path);

  // Clear out old configuration
  this->_ifaces.clear();

  for (int i = 0; i < config_.Size(); i++)
  {
    IfaceConfig.Put(*config_[i]);
    std::cout << std::endl << "IfaceConfig" << std::endl;
    IfaceConfig.DisplayJson();
  }

  // Begin critical section
  if (this->_lock.Lock())
  {

    // Parse new configuration
    UNIQUE_PTR(zData::Data)IfaceData = config_[0];
    if (IfaceData)
    {
      IfaceData->DisplayJson();

      for (int i = 0; i < IfaceData->Size(); i++)
      {
        UNIQUE_PTR(zData::Data)d = IfaceData->operator [](i);
        InterfaceConfigData config (*d);
        //config.Display();
        SHARED_PTR(Interface) iface = SHARED_PTR(Interface) (new Interface (config));
        switch (iface->Type ())
        {
#if 0
          case Interface::TYPE_WIRED:
          {
            SHARED_PTR(WiredInterface) wired_iface = SHARED_PTR(WiredInterface)(new WiredInterface(*iface));
            this->_ifaces[iface->Name ()] = STATIC_CAST (Interface) (wired_iface);
            break;
          }
          case Interface::TYPE_WIRELESS:
          {
            SHARED_PTR(WirelessInterface) wireless_iface = SHARED_PTR(WirelessInterface)(new WirelessInterface(*iface));
            this->_ifaces[iface->Name ()] = STATIC_CAST (Interface) (wireless_iface);
            break;
          }
          case Interface::TYPE_BOND:
          {
            SHARED_PTR(BondInterface) bond_iface = SHARED_PTR(BondInterface)(new BondInterface(*iface));
            this->_ifaces[iface->Name ()] = STATIC_CAST (Interface) (bond_iface);
            break;
          }
          case Interface::TYPE_LOOP:
          // No break
          case Interface::TYPE_BRIDGE:
          // No break
          case Interface::TYPE_OTHER:
          this->_ifaces[iface->Name ()] = iface;
          break;
#endif
          default:
          break;
        }
      }
    }

    // End critical section
    this->_lock.Unlock();
  }
  return (true);
}

bool
InterfaceHandler::Start()
{
  bool status = false;

  // Begin critical section
  if (this->_lock.Lock())
  {
    if (this->_timer.RegisterObserver(this))
    {
      this->_timer.Start(INTERFACE_REFRESH_PERIOD_USEC);
      status = true;
    }
    this->_lock.Unlock();
  }

  return (status);

}

void
InterfaceHandler::Stop()
{
  // Begin critical section
  if (this->_lock.Lock())
  {
    this->_timer.Stop();
    this->_lock.Unlock();
  }
}

bool
InterfaceHandler::Refresh()
{

  bool status = true;

  // Begin critical section
  if (this->_lock.Lock())
  {

    InterfaceTable::iterator it = this->_ifaces.begin();
    InterfaceTable::iterator end = this->_ifaces.end();
    for (; it != end; ++it)
    {
      if (!it->second->Refresh())
      {
        std::cout << "Failed to refresh interface: " << it->second->GetName() << std::endl;
        status = false;
      }
    }

    // End critical section
    this->_lock.Unlock();
  }

  return (status);

}

void
InterfaceHandler::Display()
{

  std::cout << std::endl;
  std::cout << "####################################################################" << std::endl;
  std::cout << "#                             INTERFACES                           #" << std::endl;
  std::cout << "####################################################################" << std::endl;
  std::cout << std::endl;

  // Begin critical section
  if (this->_lock.Lock())
  {
    InterfaceTable::iterator it = this->_ifaces.begin();
    InterfaceTable::iterator end = this->_ifaces.end();
    for (int i = 0; it != end; i++, ++it)
    {
      std::cout << "[" << i << "] " << std::endl;
      it->second->Display("\t");
    }
    std::cout.flush();

    // End critical section
    this->_lock.Unlock();
  }
}

SHARED_PTR(Interface)
InterfaceHandler::Iface (const std::string &name_)
{
  SHARED_PTR(Interface) iface;

// Begin critical section
  if (this->_lock.Lock())
  {

    iface = this->_ifaces[name_];

    // End critical section
    this->_lock.Unlock ();

  }

  return (iface);

}

InterfaceTable
InterfaceHandler::IfaceList(const Interface::TYPE type_)
{
  InterfaceTable iface_list;

  // Begin critical section
  if (this->_lock.Lock())
  {

    InterfaceTable::iterator it = this->_ifaces.begin();
    InterfaceTable::iterator end = this->_ifaces.end();
    for (int i = 0; it != end; i++, ++it)
    {
      if (it->second->Type() == type_)
      {
        iface_list[it->first] = it->second;
      }
    }

    // End critical section
    this->_lock.Unlock();
  }

  return (iface_list);
}

bool
InterfaceHandler::EventHandler(const zEvent::EventNotification* notification_)
{
  bool status = false;
  const zTimer::TimerNotification *n = NULL;

  if (notification_ && (notification_->Type() == zEvent::Event::TYPE_TIMER))
  {
    status = this->Refresh();
  }

  return (status);

}

}
}
