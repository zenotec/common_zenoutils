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

namespace zUtils
{
namespace zInterface
{

static const std::string sysfs_root("/sys/class/net/");
static const std::string sysfs_brdir("/bridge");
static const std::string sysfs_bonddir("/bonding");
static const std::string sysfs_ifindex("/ifindex");
static const std::string sysfs_hwaddr("/address");
static const std::string sysfs_flags("/flags");
static const std::string sysfs_link("/carrier");
static const std::string sysfs_state("/operstate");
static const std::string sysfs_speed("/speed");

static int
_get_ifindex(const std::string &name_)
{
  int ifindex = -1;
  std::string val;

  // Construct path for sysfs file
  std::string sysfs_filename = sysfs_root + name_ + sysfs_ifindex;

  // Read sysfs file
  std::fstream fs;
  fs.open(sysfs_filename.c_str(), std::fstream::in);
  if (fs.is_open())
  {
    fs >> val;
    fs.close();
    ifindex = strtol(val.c_str(), NULL, 0);
  }
  return (ifindex);
}

static uint32_t
_get_flags(const std::string &name_)
{
  int sock = -1;
  struct ifreq ifr = { 0 };
  uint32_t flags = 0;

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(ifr.ifr_name, name_.c_str(), IFNAMSIZ);

    // Query interface flags
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0)
    {
      flags = ifr.ifr_flags;
    }
    close(sock);
  }
  return (flags);
}

static bool
_set_flags(const std::string &name_, const uint32_t flags_)
{

  bool status = false;
  int sock = -1;
  struct ifreq ifr = { 0 };
  uint32_t flags = 0;

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(ifr.ifr_name, name_.c_str(), IFNAMSIZ);

    // Query interface flags
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0)
    {
      flags = ifr.ifr_flags;
      flags |= flags_;
      if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0)
      {
        status = true;
      }
    }
    close(sock);
  }
  return (status);
}

static bool
_clr_flags(const std::string &name_, const uint32_t flags_)
{

  bool status = false;
  int sock = -1;
  struct ifreq ifr = { 0 };
  uint32_t flags = 0;

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(ifr.ifr_name, name_.c_str(), IFNAMSIZ);

    // Query interface flags
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0)
    {
      flags = ifr.ifr_flags;
      flags &= ~flags_;
      if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0)
      {
        status = true;
      }
    }
    close(sock);
  }
  return (status);
}

static std::string
_get_hw_addr(const std::string &name_)
{
  std::string val = std::string("00:00:00:00:00:00");

  // Construct path for sysfs file
  std::string sysfs_filename = sysfs_root + name_ + sysfs_hwaddr;

  // Read sysfs file
  std::fstream fs;
  fs.open(sysfs_filename.c_str(), std::fstream::in);
  if (fs.is_open())
  {
    fs >> val;
    fs.close();
  }
  return (val);
}

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

static InterfaceConfigData::STATE
_get_state(const std::string &name_)
{
  InterfaceConfigData::STATE state = InterfaceConfigData::STATE_ERR;
  uint32_t flags = _get_flags(name_);

  if (flags & IFF_UP)
  {
    state = InterfaceConfigData::STATE_UP;
  }
  else
  {
    state = InterfaceConfigData::STATE_DOWN;
  }

  return (state);
}

static bool
_set_state(const std::string &name_, InterfaceConfigData::STATE state_)
{
  bool status = false;

  if (state_ == InterfaceConfigData::STATE_UP)
  {
    status = _set_flags(name_, IFF_UP);
  }
  else if (state_ == InterfaceConfigData::STATE_DOWN)
  {
    status = _clr_flags(name_, IFF_UP);
  }

  return (status);
}

static bool
_is_loopback(const std::string &name_)
{
  bool status = false;
  if (_get_flags(name_) & IFF_LOOPBACK)
  {
    status = true;
  }
  return (status);
}

static bool
_is_bridge(const std::string &name_)
{
  bool status = false;
  struct stat sb;

  // Construct path for sysfs file
  std::string sysfs_filename = sysfs_root + name_ + sysfs_brdir;

  // Query for bridge support
  if (stat(sysfs_filename.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
  {
    status = true;
  }
  return (status);
}

static bool
_is_bond(const std::string &name_)
{
  bool status = false;
  struct stat sb;

  // Construct path for sysfs file
  std::string sysfs_filename = sysfs_root + name_ + sysfs_bonddir;

  // Query for bonding support
  if (stat(sysfs_filename.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
  {
    status = true;
  }
  return (status);
}

static bool
_is_wired(const std::string &name_)
{
  bool status = false;

  // Construct path for sysfs file
  std::string sysfs_filename = sysfs_root + name_ + sysfs_speed;

  // Read sysfs file
  std::fstream fs;
  fs.open(sysfs_filename.c_str(), std::fstream::in);
  if (fs.is_open())
  {
    status = true;
    fs.close();
  }

  return (status);
}

static bool
_is_wireless(const std::string &name_)
{
  int sock = -1;
  struct iwreq iwr = { 0 };
  bool status = false;

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);

    // Query interface flags
    if (ioctl(sock, SIOCGIWNAME, &iwr) == 0)
    {
      status = true;
    }
    close(sock);
  }
  return (status);
}

static InterfaceConfigData::TYPE
_get_type(const std::string &name_)
{
  InterfaceConfigData::TYPE type = InterfaceConfigData::TYPE_ERR;
  std::string val;

  if (_is_loopback(name_))
  {
    type = InterfaceConfigData::TYPE_LOOP;
  }
  else if (_is_bridge(name_))
  {
    type = InterfaceConfigData::TYPE_BRIDGE;
  }
  else if (_is_bond(name_))
  {
    type = InterfaceConfigData::TYPE_BOND;
  }
  else if (_is_wireless(name_))
  {
    type = InterfaceConfigData::TYPE_WIRELESS;
  }
  else if (_is_wired(name_))
  {
    type = InterfaceConfigData::TYPE_WIRED;
  }
  else
  {
    type = InterfaceConfigData::TYPE_NONE;
  }

  return (type);
}

// ****************************************************************************
// Class: Interface
// ****************************************************************************

Interface::Interface(const InterfaceConfigData& config_) :
    InterfaceConfigData(config_), zEvent::Event(zEvent::Event::TYPE_INTERFACE),
        _lock(zSem::Mutex::LOCKED), _index(-1)
{
  ZLOG_DEBUG("Interface::Interface(config_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
  this->_timer.RegisterObserver(this);
  this->_timer.Start(INTERFACE_REFRESH_PERIOD_USEC);
  this->_lock.Unlock();
}

Interface::~Interface()
{
  this->_lock.Lock();
  this->_timer.Stop();
  this->_timer.UnregisterObserver(this);
}

bool
Interface::Refresh()
{

  bool status = false;
  std::string name(this->GetName());

  // Start critical section
  if (this->_lock.Lock())
  {

    if (!name.empty())
    {

      // Query interface index
      this->_index = _get_ifindex(name);

      if (this->GetState() != _get_state(name))
      {
        this->SetState(_get_state(name));
        // TODO Notification
      }

      // Query interface MAC address
      if (this->GetHwAddress() != _get_hw_addr(name))
      {
        this->SetHwAddress(_get_hw_addr(name));
      }

      // Query interface IP address
      if (this->GetIpAddress() != _get_ip_addr(name))
      {
        this->SetIpAddress(_get_ip_addr(name));
      }

      status = true;

    }
    else
    {
      ZLOG_ERR("Interface error: Empty name!");
    }

    // End critical section
    this->_lock.Unlock();
  }

  return (status);
}

int
Interface::Index()
{
  int index;
  if (this->_lock.Lock())
  {
    index = this->_index;
    this->_lock.Unlock();
  }
  return (index);
}

void
Interface::Display(const std::string &prefix_)
{

  std::cout << prefix_ << "Name: \t" << this->GetName() << std::endl;
  std::cout << prefix_ << "Type: \t" << InterfaceConfigData::GetType() << std::endl;

  // Start critical section
  std::cout << prefix_ << "Index:  \t" << this->Index() << std::endl;
  std::cout << prefix_ << "MAC:    \t" << this->GetHwAddress() << std::endl;
  std::cout << prefix_ << "Address:\t" << this->GetIpAddress() << std::endl;
  std::cout << prefix_ << "State:  \t" << this->GetState() << std::endl;

  return;
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
