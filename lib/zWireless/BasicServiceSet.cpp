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

// libzutils includes
#include <zutils/zAccessPointInterface.h>
#include <zutils/zBasicServiceSet.h>

#include "GetInterfaceCommand.h"
#include "NewBeaconCommand.h"
#include "SetBeaconCommand.h"
#include "StartApCommand.h"
#include "StopApCommand.h"
#include "SetBeaconCommand.h"
#include "NewStationCommand.h"
#include "DelStationCommand.h"
using namespace nl80211;

#include "Beacon.h"
using namespace zUtils::zWireless::ieee80211;

namespace zUtils
{
namespace zWireless
{

// ****************************************************************************
// Class: BasicServiceSet
// ****************************************************************************

BasicServiceSet::BasicServiceSet(const std::string& ifname_, const std::string& ssid_) :
    AccessPointInterface(ifname_), _beacon(NULL)
{
  this->_beacon = new ieee80211::Beacon();
  if (this->_beacon)
  {
    this->_beacon->ReceiverAddress("ff:ff:ff:ff:ff:ff");
    this->_beacon->TransmitterAddress(this->GetHwAddress());
    this->_beacon->Bssid(this->GetHwAddress());
    this->_beacon->Interval(100);
    this->_beacon->Capabilities(0x0001);
    this->_beacon->Ssid(ssid_);
    this->_beacon->Rates(0x82);
    this->_beacon->Rates(0x84);
    this->_beacon->Rates(0x8b);
    this->_beacon->Rates(0x96);
    this->_beacon->Rates.AddRateMbsp(6);
    this->_beacon->Rates.AddRateMbsp(9);
    this->_beacon->Rates.AddRateMbsp(12);
    this->_beacon->Rates.AddRateMbsp(18);

    this->_beacon->Dsss(1);
//    this->_beacon->Tim.Period(2);
    this->_beacon->Country("US");

    this->_beacon->ExtRates.AddRateMbsp(24);
    this->_beacon->ExtRates.AddRateMbsp(36);
    this->_beacon->ExtRates.AddRateMbsp(48);
    this->_beacon->ExtRates.AddRateMbsp(54);
//    this->_beacon->Display();
  }
}

BasicServiceSet::~BasicServiceSet()
{
  if (this->_beacon)
  {
    delete (this->_beacon);
    this->_beacon = NULL;
  }
}

std::string
BasicServiceSet::GetSsid()
{
  return (this->_beacon->Ssid());
}

bool
BasicServiceSet::SetSsid(const std::string& ssid_)
{
  return (this->_beacon->Ssid(ssid_));
}

std::string
BasicServiceSet::GetBssid()
{
  return (this->_beacon->Bssid());
}

bool
BasicServiceSet::SetBssid(const std::string& bssid_)
{
  bool status = this->SetHwAddress(bssid_);
  status &= this->_beacon->TransmitterAddress(bssid_);
  status &= this->_beacon->Bssid(bssid_);
  return (status);
}

bool
BasicServiceSet::AddStation(const std::string& addr_)
{
  bool status = false;

  if (!this->GetIfIndex())
  {
    ZLOG_ERR("Error adding stations, interface does not exist: " + this->GetIfName());
    return (false);
  }

  NewStationCommand* cmd = new NewStationCommand(this->GetIfIndex());
  cmd->Mac.SetString(addr_);
  cmd->Display();
  this->addCommand(cmd);

  return (status);
}

bool
BasicServiceSet::DelStation(const std::string& addr_)
{
  bool status = false;

  if (!this->GetIfIndex())
  {
    ZLOG_ERR("Error deleting station, interface does not exist: " + this->GetIfName());
    return (false);
  }

  DelStationCommand* cmd = new DelStationCommand(this->GetIfIndex());
  cmd->Mac.SetString(addr_);
  this->addCommand(cmd);

  return (status);
}

bool
BasicServiceSet::Commit()
{
  return (AccessPointInterface::Commit());
}

bool
BasicServiceSet::Create()
{
  bool status = false;

  if (!this->GetIfIndex() && !AccessPointInterface::Create())
  {
    ZLOG_ERR("Error creating BSS, interface does not exist: " + this->GetIfName());
    return (false);
  }

  this->_beacon->TransmitterAddress(this->GetHwAddress());
  this->_beacon->Bssid(this->GetHwAddress());

  // Set interface state to UP
  this->SetAdminState(zWireless::ConfigData::STATE_UP);
  this->Commit();

  uint8_t buf[512] = { 0 };
  size_t blen = sizeof(buf);
  if (this->_beacon->Assemble(buf, blen) != NULL)
  {
    this->SetAdminState(zInterface::ConfigData::STATE_UP);
    StartApCommand* cmd = new StartApCommand(this->GetIfIndex());
    cmd->BeaconHead.PutBuffer(this->_beacon->Head(), this->_beacon->HeadSize());
    cmd->BeaconTail.PutBuffer(this->_beacon->Tail(), this->_beacon->TailSize());
    cmd->BeaconInterval(100);
    cmd->DtimPeriod(this->_beacon->Tim.Period());
    cmd->Ssid.SetString(this->_beacon->Ssid());
    cmd->Channel.SetChannel(1);
//    cmd->Display();
    this->addCommand(cmd);
  }

  return (AccessPointInterface::Commit());
}

bool
BasicServiceSet::Destroy()
{
  bool status = false;

  if (!this->GetIfIndex())
  {
    ZLOG_ERR("Error destroying BSS, interface does not exist: " + this->GetIfName());
    return (false);
  }

  if (this->GetAdminState() == zInterface::ConfigData::STATE_UP)
  {
    StopApCommand* cmd = new StopApCommand(this->GetIfIndex());
    this->addCommand(cmd);
  }

  // Call base destroy which will eventually execute all commands in order
  return (AccessPointInterface::Destroy());
}

void
BasicServiceSet::Display(const std::string& prefix_)
{
  AccessPointInterface::Display(prefix_);
  std::cout << prefix_ << "---------- Basic Service Set -----------" << std::endl;
}

}
}
