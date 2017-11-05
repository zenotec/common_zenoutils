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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/wireless.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>

// libc++ includes

#include <string>
#include <fstream>
#include <iostream>

// libzutils includes

#include <zutils/zCompatibility.h>
#include <zutils/zInterface.h>

// local includes

#include "netlink/GetLinkCommand.h"

namespace zUtils
{
namespace zInterface
{

static std::string
_get_ip_addr(const std::string &name_)
{
  std::string val = std::string("0.0.0.0");
  char str[INET_ADDRSTRLEN] = { 0 };
  int sock = -1;
  struct ifreq ifr = { 0 };

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(ifr.ifr_name, name_.c_str(), IFNAMSIZ);

    // Query interface flags
    if (ioctl(sock, SIOCGIFADDR, &ifr) == 0)
    {
      if (inet_ntop(AF_INET, &((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr, str,
      INET_ADDRSTRLEN) == str)
      {
        val = std::string(str);
      }
    }
    close(sock);
  }
  return (val);
}

static bool
_set_ip_addr(const std::string &name_, const std::string& addr_)
{
  bool status = false;
  int sock = -1;
  struct ifreq ifr = { 0 };

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(ifr.ifr_name, name_.c_str(), IFNAMSIZ);

    // Setup INET address structure
    struct sockaddr_in* sockaddr = (struct sockaddr_in*)&ifr.ifr_addr;
    if (inet_pton( AF_INET, addr_.c_str(), &sockaddr->sin_addr) == 1)
    {
      // Set interface address
      if (ioctl(sock, SIOCSIFADDR, &ifr) == 0)
      {
        status = true;
      }
    }
    close(sock);
  }
  return (status);
}

static std::string
_get_netmask(const std::string &name_)
{
  std::string val = std::string("0.0.0.0");
  char str[INET_ADDRSTRLEN] = { 0 };
  int sock = -1;
  struct ifreq ifr = { 0 };

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(ifr.ifr_name, name_.c_str(), IFNAMSIZ);

    // Query interface flags
    if (ioctl(sock, SIOCGIFNETMASK, &ifr) == 0)
    {
      if (inet_ntop(AF_INET, &((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr, str,
      INET_ADDRSTRLEN) == str)
      {
        val = std::string(str);
      }
    }
    close(sock);
  }
  return (val);
}

static bool
_set_netmask(const std::string &name_, const std::string& addr_)
{
  bool status = false;
  int sock = -1;
  struct ifreq ifr = { 0 };

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(ifr.ifr_name, name_.c_str(), IFNAMSIZ);

    // Setup INET address structure
    struct sockaddr_in* sockaddr = (struct sockaddr_in*)&ifr.ifr_addr;
    if (inet_pton( AF_INET, addr_.c_str(), &sockaddr->sin_addr) == 1)
    {
      // Set interface address
      if (ioctl(sock, SIOCSIFNETMASK, &ifr) == 0)
      {
        status = true;
      }
    }
    close(sock);
  }
  return (status);
}

// ****************************************************************************
// Class: Interface
// ****************************************************************************

Interface::Interface(const InterfaceConfigData& config_) :
    InterfaceConfigData(config_), zEvent::Event(zEvent::Event::TYPE_INTERFACE),
        _lock(zSem::Mutex::LOCKED), _refreshed(false), _index(-1)
{
  ZLOG_DEBUG("Interface::Interface(config_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
  this->_lock.Unlock();
  this->Refresh();
  this->_timer.RegisterObserver(this);
  this->_timer.Start(INTERFACE_REFRESH_PERIOD_USEC);
}

Interface::~Interface()
{
  this->_lock.Lock();
  this->_timer.Stop();
  this->_timer.UnregisterObserver(this);
}

bool
Interface::IsRefreshed()
{
  bool flag = false;
  if (this->_lock.Lock())
  {
    flag = this->_refreshed;
    this->_lock.Unlock();
  }
  return (flag);
}

int
Interface::GetIndex()
{
  int index;
  if (this->_lock.Lock())
  {
    index = this->_index;
    this->_lock.Unlock();
  }
  return (index);
}

bool
Interface::Refresh()
{

  bool status = false;
  std::string name(this->GetName());

  // Look up interface information
  netlink::GetLinkCommand getlinkcmd(name);
  if (name.empty() || !getlinkcmd.Exec())
  {
    return(false);
  }

  // Start critical section
  if (this->_lock.Lock())
  {

    this->_refreshed = true;
    this->_index = getlinkcmd.Link.IfIndex();
    this->SetHwAddress(getlinkcmd.Link.Mac());

    // Update link state
    uint32_t flags = (getlinkcmd.Link.Flags() & (IFF_UP | IFF_RUNNING));
    if ((flags) == (IFF_UP | IFF_RUNNING))
    {
      this->SetState(InterfaceConfigData::STATE_UP);
    }
    else
    {
      this->SetState(InterfaceConfigData::STATE_DOWN);
    }

    this->SetIpAddress(_get_ip_addr(name));
    this->SetNetmask(_get_netmask(name));

    status = true;

    // End critical section
    this->_lock.Unlock();
  }

  return (status);
}

bool
Interface::Create()
{
  return(false);
}

bool
Interface::Destroy()
{
  return(false);
}

void
Interface::Display(const std::string &prefix_)
{
  std::cout << prefix_ << "Name: \t" << this->GetName() << std::endl;
  std::cout << prefix_ << "Type: \t" << InterfaceConfigData::GetType() << std::endl;
  std::cout << prefix_ << "Index:  \t" << this->GetIndex() << std::endl;
  std::cout << prefix_ << "MAC:    \t" << this->GetHwAddress() << std::endl;
  std::cout << prefix_ << "Address:\t" << this->GetIpAddress() << std::endl;
  std::cout << prefix_ << "Netmask:\t" << this->GetNetmask() << std::endl;
  std::cout << prefix_ << "State:  \t" << this->GetState() << std::endl;
}

bool
Interface::EventHandler(zEvent::EventNotification* notification_)
{
  bool status = false;
  zTimer::TimerNotification *n = NULL;

  if (notification_ && (notification_->Type() == zEvent::Event::TYPE_TIMER))
  {
    status = this->Refresh();
  }

  return (status);

}

}
}
