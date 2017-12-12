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
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/wireless.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>

// libc++ includes

#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <map>

// libzutils includes

#include <zutils/zAccessPointInterface.h>

// local includes

#include "GetInterfaceCommand.h"
#include "StartApCommand.h"
#include "StopApCommand.h"
#include "SetBeaconCommand.h"
using namespace nl80211;

#include "Beacon.h"
#include "Probe.h"
using namespace ieee80211;

namespace zUtils
{
namespace zInterface
{

// ****************************************************************************
// Class: AccessPointInterface
// ****************************************************************************

AccessPointInterface::AccessPointInterface(const std::string& name_,
    const unsigned int phyindex_) :
    WirelessInterface(name_, phyindex_), _modified(false), _beaconbuf{0}
{
  this->wconfig.SetOpMode(WirelessInterfaceConfigData::OPMODE_AP);
}

AccessPointInterface::AccessPointInterface(const zInterface::ConfigData& config_) :
    WirelessInterface(config_), _modified(false), _beaconbuf{0}
{
  this->wconfig.SetOpMode(WirelessInterfaceConfigData::OPMODE_AP);
}

AccessPointInterface::~AccessPointInterface()
{
}

std::string
AccessPointInterface::GetSsid() const
{
  std::string ssid;
  if (this->lock.Lock())
  {
    if (this->ifindex)
    {
      GetInterfaceCommand cmd(this->ifindex);
      if (cmd.Exec())
      {
        ssid = cmd.Ssid();
      }
    }
    else
    {
      ssid = this->wconfig.GetSsid();
    }
    this->lock.Unlock();
  }
  return (ssid);
}

bool
AccessPointInterface::SetSsid(const std::string& ssid_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->wconfig.SetSsid(ssid_);
    this->_modified = true;
    this->lock.Unlock();
  }
  return (status);
}

std::string
AccessPointInterface::GetBssid() const
{
  return (this->GetHwAddress());
}

bool
AccessPointInterface::SetBssid(const std::string& bssid_)
{
  return (this->SetHwAddress(bssid_));
}

bool
AccessPointInterface::Commit()
{
  return (WirelessInterface::Commit());
}

bool
AccessPointInterface::Create()
{
#if 0
  bool status = false;

  if (WirelessInterface::Create())
  {
    Beacon beacon;
    size_t blen = sizeof(this->_beaconbuf);
    beacon.ReceiverAddress("ff:ff:ff:ff:ff:ff");
    beacon.TransmitterAddress(this->GetHwAddress());
    beacon.Bssid(this->GetHwAddress());
    beacon.Ssid(this->wconfig.GetSsid());
    beacon.Assemble(this->_beaconbuf, blen);

    StartApCommand cmd(this->ifindex);
    cmd.Ssid(this->wconfig.GetSsid());
    cmd.BeaconHead.PutBuffer(beacon.Head(), beacon.HeadSize());
    cmd.BeaconTail.PutBuffer(beacon.Tail(), beacon.TailSize());
    status = cmd.Exec();
  }

  if (!status)
  {

  }

  return (status);
#else
  return (WirelessInterface::Create());
#endif
}

bool
AccessPointInterface::Destroy()
{
  return (WirelessInterface::Destroy());
}

void
AccessPointInterface::Display(const std::string& prefix_)
{
  WirelessInterface::Display(prefix_);
  std::cout << "------ Access Point Interface ----------" << std::endl;
  std::cout << prefix_ << "SSID:   \t" << this->GetSsid() << std::endl;
  std::cout << prefix_ << "BSSID:  \t" << this->GetBssid() << std::endl;
}

bool
AccessPointInterface::is_modified() const
{
  return (this->_modified);
}

void
AccessPointInterface::set_modified()
{
  this->_modified = true;
}

void
AccessPointInterface::clr_modified()
{
  this->_modified = false;
}

}
}
