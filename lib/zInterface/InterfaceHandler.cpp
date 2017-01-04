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

InterfaceHandler::InterfaceHandler(zConfig::ConfigData &config_) :
    _lock(zSem::Mutex::LOCKED)
{
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

InterfaceHandler::~InterfaceHandler()
{
  this->Stop();
  this->_lock.Lock();
}

bool
InterfaceHandler::Start()
{
  bool status = false;

  // Begin critical section
  if (this->_lock.Lock())
  {
    status = true;
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
    this->_lock.Unlock();
  }
  return;
}

bool
InterfaceHandler::Refresh()
{

  bool status = true;

  // Begin critical section
  if (this->_lock.Lock())
  {
    FOREACH (auto& iface, this->_ifaces)
    {
      if (!iface.second->Refresh())
      {
        std::cout << "Failed to refresh interface: " << iface.second->GetName() << std::endl;
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
    int i = 0;
    FOREACH (auto& iface, this->_ifaces)
    {
      std::cout << "[" << i++ << "] " << std::endl;
      iface.second->Display("\t");
    }

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
    FOREACH (auto& iface, this->_ifaces)
    {
      if (iface.second->Type() == type_)
      {
        iface_list[iface.first] = iface.second;
      }
    }
    // End critical section
    this->_lock.Unlock();
  }

  return (iface_list);
}

}
}
