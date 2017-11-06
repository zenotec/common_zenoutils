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

#include "GetLinkCommand.h"
#include "SetLinkCommand.h"
#include "RouteLinkEvent.h"
using namespace netlink;

#include <zutils/zInterface.h>

// local includes

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

Interface::Interface(const int index_) :
    zEvent::Event(zEvent::Event::TYPE_INTERFACE), _refreshed(false),
    _getlinkcmd(NULL), _setlinkcmd(NULL), _rtlinkevent(NULL)
{
  this->_getlinkcmd = new GetLinkCommand(index_);
  this->_setlinkcmd = new SetLinkCommand(index_);
  this->_rtlinkevent = new RouteLinkEvent;
  this->_lock.Unlock();
}

Interface::Interface(const std::string& name_) :
    zEvent::Event(zEvent::Event::TYPE_INTERFACE), _refreshed(false),
    _getlinkcmd(NULL), _setlinkcmd(NULL), _rtlinkevent(NULL)

{
  this->_getlinkcmd = new GetLinkCommand(name_);
  this->_setlinkcmd = new SetLinkCommand(name_);
  this->_rtlinkevent = new RouteLinkEvent;
  this->_lock.Unlock();
}

Interface::Interface(const ConfigData& config_) :
		Config(config_), zEvent::Event(zEvent::Event::TYPE_INTERFACE),
		_refreshed(false), _getlinkcmd(NULL), _setlinkcmd(NULL), _rtlinkevent(NULL)

{
  ZLOG_DEBUG("Interface::Interface(config_)");
  ZLOG_DEBUG(this->Config.Path());
  ZLOG_DEBUG(this->Config.GetJson());
  this->_getlinkcmd = new GetLinkCommand(config_.Name());
  this->_setlinkcmd = new SetLinkCommand(config_.Name());
  this->_rtlinkevent = new RouteLinkEvent;
  this->_lock.Unlock();
}

Interface::~Interface()
{
  this->_lock.Lock();
  if (this->_getlinkcmd)
  {
    delete (this->_getlinkcmd);
    this->_getlinkcmd = NULL;
  }
  if (this->_setlinkcmd)
  {
    delete (this->_setlinkcmd);
    this->_setlinkcmd = NULL;
  }
  if (this->_rtlinkevent)
  {
    delete (this->_rtlinkevent);
    this->_rtlinkevent = NULL;
  }
}

bool
Interface::IsRefreshed() const
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
Interface::GetIndex() const
{
  int index = -1;
  if (this->_lock.Lock())
  {
    if (this->_refreshed)
    {
      index = this->_getlinkcmd->Link.IfIndex();
    }
    this->_lock.Unlock();
  }
  return (index);
}

std::string
Interface::GetName() const
{
  std::string name;
  if (this->_lock.Lock())
  {
    if (this->_refreshed)
    {
      name = this->_getlinkcmd->Link.IfName();
    }
    this->_lock.Unlock();
  }
  return (name);
}

bool
Interface::SetName(const std::string& name_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_setlinkcmd)
    {
      this->_refreshed = false;
      this->_setlinkcmd->Link.IfName(name_);
      status = this->_setlinkcmd->Exec();
    }
    this->_lock.Unlock();
  }
  return (status);
}

std::string
Interface::GetHwAddress() const
{
  std::string addr;
  if (this->_lock.Lock())
  {
    if (this->_refreshed)
    {
      addr = this->_getlinkcmd->Link.Mac();
    }
    this->_lock.Unlock();
  }
  return (addr);
}

bool
Interface::SetHwAddress(const std::string& addr_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    if (this->_refreshed)
    {
      if (this->_refreshed && this->_setlinkcmd)
      {
        this->_refreshed = false;
        this->_setlinkcmd->Link.Mac(addr_);
        status = this->_setlinkcmd->Exec();
      }
    }
    this->_lock.Unlock();
  }
  return (status);
}

Interface::STATE
Interface::GetAdminState() const
{
  Interface::STATE state = Interface::STATE_ERR;
  if (this->_lock.Lock())
  {
    if (this->_refreshed)
    {
      uint32_t flags = this->_getlinkcmd->Link.Flags();
      if ((flags & (IFF_UP|IFF_RUNNING)) == (IFF_UP|IFF_RUNNING))
      {
        state = Interface::STATE_UP;
      }
      else
      {
        state = Interface::STATE_DOWN;
      }
    }
    this->_lock.Unlock();
  }
  return (state);
}

bool
Interface::SetAdminState(const Interface::STATE state_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_setlinkcmd)
    {
      this->_refreshed = false;
      if (state_ == Interface::STATE_UP)
      {
        this->_setlinkcmd->Link.SetFlags((IFF_UP|IFF_RUNNING));
      }
      else
      {
        this->_setlinkcmd->Link.ClrFlags((IFF_UP|IFF_RUNNING));
      }
      status = this->_setlinkcmd->Exec();
    }
    this->_lock.Unlock();
  }
  return (status);
}

bool
Interface::Refresh()
{

  if (this->_lock.Lock())
  {
    // Validate and execute get link command
    if (this->_getlinkcmd && this->_getlinkcmd->Exec())
    {
      this->_refreshed = true;
    }
    this->_lock.Unlock();
  }

  return(this->_refreshed);

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
//  std::cout << prefix_ << "Name: \t" << this->Name() << std::endl;
//  std::cout << prefix_ << "Type: \t" << ConfigData::Type() << std::endl;
//  std::cout << prefix_ << "Index:  \t" << this->GetIndex() << std::endl;
//  std::cout << prefix_ << "MAC:    \t" << this->HwAddress() << std::endl;
//  std::cout << prefix_ << "Address:\t" << this->IpAddress() << std::endl;
//  std::cout << prefix_ << "Netmask:\t" << this->Netmask() << std::endl;
//  std::cout << prefix_ << "State:  \t" << this->AdminState() << std::endl;
}

}
}
