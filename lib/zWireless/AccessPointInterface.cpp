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
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <map>

// libzutils includes
#include <zutils/zLog.h>
#include <zutils/nl80211/NewInterfaceCommand.h>
#include <zutils/nl80211/StartApCommand.h>
#include <zutils/nl80211/StopApCommand.h>
#include <zutils/nl80211/SetBeaconCommand.h>
using namespace nl80211;
#include <zutils/zAccessPointInterface.h>


ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{

// ****************************************************************************
// Class: AccessPointInterface
// ****************************************************************************

AccessPointInterface::AccessPointInterface(const std::string& ifname_) :
    Interface(ifname_),
    _running(false)
{
  this->SetOpMode(ConfigData::OPMODE_AP);
}

AccessPointInterface::~AccessPointInterface()
{
  this->Stop();
}

std::string
AccessPointInterface::GetSsid() const
{
  std::string ssid;
  if (this->lock.Lock())
  {
    ssid = this->_getSsid();
    if (ssid.empty())
    {
      ssid = this->stagingConfig.GetSsid();
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
    status = this->stagingConfig.SetSsid(ssid_);
    this->lock.Unlock();
  }
  return (status);
}

std::string
AccessPointInterface::GetBssid() const
{
  std::string bssid;
  if (this->lock.Lock())
  {
    bssid = this->_getBssid();
    if (bssid.empty())
    {
      bssid = this->stagingConfig.GetBssid();
    }
    this->lock.Unlock();
  }
  return (bssid);
}

bool
AccessPointInterface::SetBssid(const std::string& bssid_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->stagingConfig.SetBssid(bssid_);
    this->lock.Unlock();
  }
  return (status);
}

bool
AccessPointInterface::Create()
{

  bool status = false;

  if (this->lock.Lock())
  {
    NewInterfaceCommand *cmd = new NewInterfaceCommand(this->stagingConfig.GetIfName(),
        this->stagingConfig.GetPhyIndex());
    cmd->IfType.Set(this->opmode2nl(this->stagingConfig.GetOpMode()));
    this->addCommand(cmd);
    status = this->execCommands();
    this->lock.Unlock();
  }

  return (status && this->Refresh());

}

bool
AccessPointInterface::Start(ieee80211::Beacon& beacon_, ieee80211::ProbeResponse& probe_)
{

  uint8_t buf[512] = { 0 };
  size_t blen = 0;

  if (!this->GetIfIndex())
  {
    ZLOG_ERR("Error starting AP interface, interface does not exist: " + this->GetIfName());
    return (false);
  }

  if (this->_running)
  {
    ZLOG_ERR("Error updating AP interface, interface is already running: " + this->GetIfName());
    return (false);
  }

  // Create new Start AP command
  StartApCommand* cmd = new StartApCommand(this->GetIfIndex());

  // Write beacon
  memset(buf, 0, sizeof(buf));
  blen = sizeof(buf);
  if (beacon_.Assemble(buf, blen) == NULL)
  {
    return (false);
  }
  cmd->BeaconHead.Set(beacon_.Head(), beacon_.HeadSize()); // copies buffer
  cmd->BeaconTail.Set(beacon_.Tail(), beacon_.TailSize()); // copies buffer
  cmd->BeaconInterval(beacon_.Interval());

  // Write out probe
  memset(buf, 0, sizeof(buf));
  blen = sizeof(buf);
  if (probe_.Assemble(buf, blen) == NULL)
  {
    return (false);
  }
  cmd->ProbeResp.Set(buf, (sizeof(buf) - blen)); // copies buffer

  cmd->DtimPeriod(1);
  cmd->Ssid(this->GetSsid());
  cmd->Channel(this->GetFrequency());
  cmd->ChannelWidth(this->htmode2nl(this->GetHtMode()));
  cmd->CenterFrequency1(this->GetCenterFrequency1());
  cmd->CenterFrequency2(this->GetCenterFrequency2());
  this->addCommand(cmd);

  if (this->execCommands())
  {
    this->_running = true;
  }

  return (this->_running);

}

bool
AccessPointInterface::Update(ieee80211::Beacon& beacon_, ieee80211::ProbeResponse& probe_)
{

  bool status = false;
  uint8_t buf[512] = { 0 };
  size_t blen = 0;

  if (!this->GetIfIndex())
  {
    ZLOG_ERR("Error updating AP interface, interface does not exist: " + this->GetIfName());
    return (false);
  }

  if (!this->_running)
  {
    ZLOG_ERR("Error updating AP interface, interface is not running: " + this->GetIfName());
    return (false);
  }

  // Create new Start AP command
  SetBeaconCommand* cmd = new SetBeaconCommand(this->GetIfIndex());

  // Write beacon
  memset(buf, 0, sizeof(buf));
  blen = sizeof(buf);
  if (beacon_.Assemble(buf, blen) == NULL)
  {
    return (false);
  }
  cmd->BeaconHead.Set(beacon_.Head(), beacon_.HeadSize()); // copies buffer
  cmd->BeaconTail.Set(beacon_.Tail(), beacon_.TailSize()); // copies buffer
  cmd->BeaconInterval(beacon_.Interval());

  // Write out probe
  memset(buf, 0, sizeof(buf));
  blen = sizeof(buf);
  if (probe_.Assemble(buf, blen) == NULL)
  {
    return (false);
  }
  cmd->ProbeResp.Set(buf, (sizeof(buf) - blen)); // copies buffer

  this->addCommand(cmd);

  if (this->execCommands())
  {
    this->_running = true;
  }

  return (this->_running);

}

bool
AccessPointInterface::Stop()
{
  bool status = false;

  if (!this->GetIfIndex())
  {
    ZLOG_ERR("Error stopping AP interface, interface does not exist: " + this->GetIfName());
    return (false);
  }

  if (!this->_running)
  {
    return (true);
  }

  if (this->GetAdminState() == zInterface::ConfigData::STATE_UP)
  {
    this->SetPromiscuousMode(zWireless::ConfigData::PROMODE_DISABLED);
    this->SetAdminState(zWireless::ConfigData::STATE_DOWN);
    StopApCommand* cmd = new StopApCommand(this->GetIfIndex());
    this->addCommand(cmd);
    if (this->Commit())
    {
      this->_running = false;
    }
  }

  return (!this->_running);
}

void
AccessPointInterface::Display(const std::string& prefix_)
{
  Interface::Display(prefix_);
  std::cout << prefix_ << "------ Access Point Interface ----------" << std::endl;
}

std::string
AccessPointInterface::_getSsid() const
{
  std::string ssid;
  if (this->_running)
  {

  }
  return (ssid);
}

std::string
AccessPointInterface::_getBssid() const
{
  std::string bssid;
  if (this->_running)
  {

  }
  return (bssid);
}

}
}
