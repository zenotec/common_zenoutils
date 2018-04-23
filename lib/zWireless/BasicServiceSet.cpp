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
#include <zutils/zLog.h>
#include <zutils/zAccessPointInterface.h>
#include <zutils/zBasicServiceSet.h>
#include <zutils/ieee80211/Beacon.h>
#include <zutils/ieee80211/Probe.h>
#include <zutils/nl80211/GetPhyCommand.h>
#include <zutils/nl80211/SetPhyCommand.h>
#include <zutils/nl80211/NewStationCommand.h>
#include <zutils/nl80211/DelStationCommand.h>
#include <zutils/nl80211/StartApCommand.h>
#include <zutils/nl80211/StopApCommand.h>
using namespace nl80211;

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{

static uint32_t
_htmode2nl(ConfigData::HTMODE mode_)
{
  uint32_t val = 0;
  switch (mode_)
  {
  case ConfigData::HTMODE_NONE:
    // no break
  case ConfigData::HTMODE_NOHT:
    val = NL80211_CHAN_NO_HT;
    break;
  case ConfigData::HTMODE_HT20:
    val = NL80211_CHAN_WIDTH_20;
    break;
  case ConfigData::HTMODE_HT40:
    val = NL80211_CHAN_WIDTH_40;
    break;
  case ConfigData::HTMODE_VHT20:
    val = NL80211_CHAN_WIDTH_20;
    break;
  case ConfigData::HTMODE_VHT40:
    val = NL80211_CHAN_WIDTH_40;
    break;
  case ConfigData::HTMODE_VHT80:
    val = NL80211_CHAN_WIDTH_80;
    break;
  case ConfigData::HTMODE_VHT80PLUS80:
    val = NL80211_CHAN_WIDTH_80P80;
    break;
  case ConfigData::HTMODE_VHT160:
    val = NL80211_CHAN_WIDTH_160;
    break;
  case ConfigData::HTMODE_ERR:
    // no break
  default:
    break;
  }
  return (val);
}

// ****************************************************************************
// Class: BasicServiceSet
// ****************************************************************************

BasicServiceSet::BasicServiceSet(const std::string& ifname_, const std::string& ssid_) :
    AccessPointInterface(ifname_), _ssid(ssid_)
{
}

BasicServiceSet::~BasicServiceSet()
{
}

std::string
BasicServiceSet::GetSsid()
{
  return (this->_ssid);
}

bool
BasicServiceSet::SetSsid(const std::string& ssid_)
{
  this->_ssid = ssid_;
  return (true);
}

std::string
BasicServiceSet::GetBssid()
{
  return (this->_bssid);
}

bool
BasicServiceSet::SetBssid(const std::string& bssid_)
{
  this->_bssid = bssid_;
  return (true);
}

bool
BasicServiceSet::AddStation(const std::string& addr_)
{
  return (true);
}

bool
BasicServiceSet::DelStation(const std::string& addr_)
{
  return (true);
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
  uint8_t buf[512] = { 0 };
  size_t blen = 0;

  if (!this->GetIfIndex() && !AccessPointInterface::Create())
  {
    ZLOG_ERR("Error creating BSS, interface does not exist: " + this->GetIfName());
    return (false);
  }

  // Set interface state to UP
  this->SetHwAddress(this->_bssid);
  this->SetAdminState(zWireless::ConfigData::STATE_UP);
  this->SetPromiscuousMode(zWireless::ConfigData::PROMODE_ENABLED);
  if (!AccessPointInterface::Commit())
  {
    ZLOG_ERR("Error creating BSS, cannot UP interface: " + this->GetIfName());
    return (false);
  }

  // Create new Start AP command
  StartApCommand* cmd = new StartApCommand(this->GetIfIndex());

  // Update beacon
  _update_beacon();

  // Write beacon
  memset(buf, 0, sizeof(buf));
  blen = sizeof(buf);
  if (this->_beacon.Assemble(buf, blen) == NULL)
  {
    return (false);
  }
  cmd->BeaconHead.Set(this->_beacon.Head(), this->_beacon.HeadSize()); // copies buffer
  cmd->BeaconTail.Set(this->_beacon.Tail(), this->_beacon.TailSize()); // copies buffer
  cmd->BeaconInterval(this->_beacon.Interval());

  // Update probe
  _update_probe();

  // Write out probe
  memset(buf, 0, sizeof(buf));
  blen = sizeof(buf);
  if (this->_probe.Assemble(buf, blen) == NULL)
  {
    return (false);
  }
  cmd->ProbeResp.Set(buf, blen); // copies buffer

  cmd->DtimPeriod(1);
  cmd->Ssid(this->_ssid);
  cmd->Channel(this->GetFrequency());
  cmd->ChannelWidth(_htmode2nl(this->GetHtMode()));
  cmd->CenterFrequency1(this->GetCenterFrequency1());
  this->addCommand(cmd);
  cmd->Display();

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
    this->SetPromiscuousMode(zWireless::ConfigData::PROMODE_DISABLED);
    this->SetAdminState(zWireless::ConfigData::STATE_DOWN);
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

void
BasicServiceSet::_update_beacon()
{

  zWireless::Capabilities::BAND band = zWireless::Capabilities::BAND_2_4; // TODO: Should be based on the configured channel

  std::map<int, Capabilities> caps = this->GetCapabilities();
  if (caps.empty() || !caps.count(band))
  {
    return;
  }

  this->_beacon.ReceiverAddress("ff:ff:ff:ff:ff:ff");
  this->_beacon.TransmitterAddress(this->_bssid);
  this->_beacon.Bssid(this->_bssid);
  this->_beacon.Interval(100);
  this->_beacon.Capabilities(0x0421);
  this->_beacon.Ssid(this->_ssid);
  std::vector<uint8_t> rates;
  rates.push_back(0x82);
  rates.push_back(0x84);
  rates.push_back(0x8b);
  rates.push_back(0x96);
  rates.push_back(0x0c);
  rates.push_back(0x12);
  rates.push_back(0x18);
  rates.push_back(0x24);
  this->_beacon.Rates(rates);
//  this->_beacon.Rates(caps[band].GetBitRates());
  this->_beacon.Dsss(this->GetChannel());
  this->_beacon.Country("US");
  this->_beacon.ErpInfo(0);
  if (!caps[band].GetExtBitRates().empty())
  {
    this->_beacon.ExtRates(caps[band].GetExtBitRates());
  }
//  this->_beacon.SuppOpClass(81);
  this->_beacon.HtCaps = this->HtCaps;
//  this->_beacon.HtCaps(caps[band].GetHtCaps());
  this->_beacon.HtInfo = this->HtInfo;
//  this->_beacon.HtInfo(caps[band].GetHtInfo());
  this->_beacon.ExtCaps.SetFlag(ieee80211::ExtCapsTag::EXCAP_EXTENDED_CHANNEL_SWITCHING);
  this->_beacon.ExtCaps.SetFlag(ieee80211::ExtCapsTag::EXCAP_OPERATING_MODE_NOTIFICATION);
  this->_beacon.Display();

}

void
BasicServiceSet::_update_probe()
{

  zWireless::Capabilities::BAND band = zWireless::Capabilities::BAND_2_4; // TODO: Should be based on the configured channel

  std::map<int, Capabilities> caps = this->GetCapabilities();
  if (caps.empty() || !caps.count(band))
  {
    return;
  }

  this->_probe.ReceiverAddress("00:00:00:00:00:00");
  this->_probe.TransmitterAddress(this->_bssid);
  this->_probe.Bssid(this->_bssid);
  this->_probe.Interval(100);
  this->_probe.Capabilities(0x0421);
  this->_probe.Ssid(this->_ssid);
  std::vector<uint8_t> rates;
  rates.push_back(0x82);
  rates.push_back(0x84);
  rates.push_back(0x8b);
  rates.push_back(0x96);
  rates.push_back(0x0c);
  rates.push_back(0x12);
  rates.push_back(0x18);
  rates.push_back(0x24);
  this->_probe.Rates(rates);
//  this->_probe.Rates(caps[band].GetBitRates());
  this->_probe.Dsss(this->GetChannel());
  this->_probe.Country("US");
  this->_probe.ErpInfo(0);
  if (!caps[band].GetExtBitRates().empty())
  {
    this->_probe.ExtRates(caps[band].GetExtBitRates());
  }
  //  this->_beacon.SuppOpClass(81);
    this->_probe.HtCaps = this->HtCaps;
  //  this->_beacon.HtCaps(caps[band].GetHtCaps());
    this->_probe.HtInfo = this->HtInfo;
  //  this->_beacon.HtInfo(caps[band].GetHtInfo());
  this->_probe.ExtCaps.SetFlag(ieee80211::ExtCapsTag::EXCAP_EXTENDED_CHANNEL_SWITCHING);
  this->_probe.ExtCaps.SetFlag(ieee80211::ExtCapsTag::EXCAP_OPERATING_MODE_NOTIFICATION);
  this->_probe.Display();

}

}
}
