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

  // Begin critical section
  if (this->_lock.Lock())
  {
    // Clear out old configuration
    this->_ifaces.clear();

    for (int i = 0; i < config_.Size(); i++)
    {
      InterfaceConfigData IfaceConfig;
      config_[i]->Get(IfaceConfig.GetData());

      SHARED_PTR(Interface)Iface = InterfaceFactory::Create(IfaceConfig);

      if (Iface && Iface->Refresh())
      {
        this->_ifaces[IfaceConfig.GetName()] = Iface;
      }
    }
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
