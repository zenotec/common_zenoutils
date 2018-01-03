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
#include "Probe.h"
using namespace ieee80211;

namespace zUtils
{
namespace zWireless
{

// ****************************************************************************
// Class: BasicServiceSet
// ****************************************************************************

BasicServiceSet::BasicServiceSet(const std::string& ifname_, const std::string& ssid_) :
    AccessPointInterface(ifname_), _beaconbuf { 0 }, _beaconlen(0)
{
  Beacon beacon;
  size_t blen = sizeof(this->_beaconbuf);
  beacon.ReceiverAddress("ff:ff:ff:ff:ff:ff");
  beacon.TransmitterAddress(this->GetHwAddress());
  beacon.Bssid(this->GetHwAddress());
  beacon.Interval(100);
  beacon.Capabilities(0x0001);
  beacon.Ssid(ssid_);
  beacon.Rates(6);
  beacon.Rates(9);
  beacon.Rates(12);
  beacon.Rates(18);
  beacon.Rates(24);
  beacon.Rates(36);
  beacon.Rates(48);
  beacon.Rates(54);
  beacon.Dsss(1);
  beacon.Tim.Period(2);
//  beacon.Display();
  if (beacon.Assemble(this->_beaconbuf, blen) != NULL)
  {
    this->_beaconlen = (sizeof(this->_beaconbuf) - blen);
  }
}

BasicServiceSet::~BasicServiceSet()
{
}

std::string
BasicServiceSet::GetSsid()
{
  Beacon beacon;
  size_t blen = this->_beaconlen;
  beacon.Disassemble(this->_beaconbuf, blen);
  return (beacon.Ssid());
}

bool
BasicServiceSet::SetSsid(const std::string& ssid_)
{
  bool status = false;
  Beacon beacon;
  size_t blen = this->_beaconlen;

  if (beacon.Disassemble(this->_beaconbuf, blen))
  {
    beacon.Ssid(ssid_);
    blen = sizeof(this->_beaconbuf);
    if (beacon.Assemble(this->_beaconbuf, blen) != NULL)
    {
      this->_beaconlen = (sizeof(this->_beaconbuf) - blen);
      if (this->GetIfIndex())
      {
        SetBeaconCommand* cmd = new SetBeaconCommand(this->GetIfIndex());
        cmd->BeaconHead.PutBuffer(beacon.Head(), beacon.HeadSize());
        cmd->BeaconTail.PutBuffer(beacon.Tail(), beacon.TailSize());
        cmd->BeaconInterval(100);
        cmd->DtimPeriod(beacon.Tim.Period());
        cmd->Ssid.SetString(beacon.Ssid());
        cmd->Channel.SetChannel(1);
//        cmd->Display();
        this->addCommand(cmd);
      }
      status = true;
    }
  }

  return (status);
}

std::string
BasicServiceSet::GetBssid()
{
  Beacon beacon;
  size_t blen = this->_beaconlen;
  beacon.Disassemble(this->_beaconbuf, blen);
  return (beacon.Bssid());
}

bool
BasicServiceSet::SetBssid(const std::string& bssid_)
{
  bool status = false;
  Beacon beacon;
  size_t blen = this->_beaconlen;
  if (beacon.Disassemble(this->_beaconbuf, blen))
  {
    beacon.Bssid(bssid_);
    blen = sizeof(this->_beaconbuf);
    if (beacon.Assemble(this->_beaconbuf, blen) != NULL)
    {
      this->_beaconlen = (sizeof(this->_beaconbuf) - blen);
      SetBeaconCommand* cmd = new SetBeaconCommand(this->GetIfIndex());
      cmd->BeaconHead.PutBuffer(beacon.Head(), beacon.HeadSize());
      cmd->BeaconTail.PutBuffer(beacon.Tail(), beacon.TailSize());
      cmd->BeaconInterval(100);
      cmd->DtimPeriod(beacon.Tim.Period());
      cmd->Ssid.SetString(beacon.Ssid());
      cmd->Channel.SetChannel(1);
//      cmd->Display();
      this->addCommand(cmd);
      status = true;
    }
  }
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
  cmd->Display();
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

  // Set interface state to UP
  this->SetAdminState(zWireless::ConfigData::STATE_UP);

  Beacon beacon;
  size_t blen = this->_beaconlen;
  if (beacon.Disassemble(this->_beaconbuf, blen) != NULL)
  {
    this->SetAdminState(zInterface::ConfigData::STATE_UP);
    StartApCommand* cmd = new StartApCommand(this->GetIfIndex());
    cmd->BeaconHead.PutBuffer(beacon.Head(), beacon.HeadSize());
    cmd->BeaconTail.PutBuffer(beacon.Tail(), beacon.TailSize());
    cmd->BeaconInterval(100);
    cmd->DtimPeriod(beacon.Tim.Period());
    cmd->Ssid.SetString(beacon.Ssid());
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
