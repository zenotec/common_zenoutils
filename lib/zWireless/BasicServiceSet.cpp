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
#include <zutils/ieee80211/WmmWmeTag.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{

// ****************************************************************************
// Class: BasicServiceSet
// ****************************************************************************

BasicServiceSet::BasicServiceSet(const std::string& ifname_, const std::string& ssid_) :
    _iface(ifname_)
{
  this->SetSsid(ssid_);
}

BasicServiceSet::~BasicServiceSet()
{
}

bool
BasicServiceSet::Start()
{

  // Configure interface
  this->_iface.SetPhyIndex(this->GetPhyIndex());

  // Create interface if it does not already exist
  if (!this->_iface.GetIfIndex() && !this->_iface.Create())
  {
    ZLOG_ERR("Error creating AP interface: " + this->_iface.GetIfName());
    return (false);
  }

  // Configure interface
  this->_iface.SetHwAddress(this->GetBssid());
  this->_iface.SetAdminState(zWireless::ConfigData::STATE_UP);
  this->_iface.SetPromiscuousMode(zWireless::ConfigData::PROMODE_ENABLED);
  if (!this->_iface.Commit())
  {
    ZLOG_ERR("Error creating AP interface: " + this->_iface.GetIfName());
    return (false);
  }

  // Update configuration
  this->_iface.SetSsid(this->GetSsid());
  this->_iface.SetBssid(this->GetBssid());
  this->_iface.SetHtMode(this->GetHtMode());
  this->_iface.SetFrequency(this->GetFrequency());
  this->_iface.SetCenterFrequency1(this->GetCenterFrequency1());
  this->_iface.SetCenterFrequency2(this->GetCenterFrequency2());


  // Update beacon and probe
  _update_beacon();
  _update_probe();

  // Start AP and return status
  return (this->_iface.Start(this->_beacon, this->_probe));

}

bool
BasicServiceSet::Stop()
{
  return (this->_iface.Stop());
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

void
BasicServiceSet::Display(const std::string& prefix_)
{
  this->_iface.Display(prefix_);
  std::cout << prefix_ << "---------- Basic Service Set -----------" << std::endl;
}

void
BasicServiceSet::_update_beacon()
{

  zWireless::Capabilities::BAND band = zWireless::Capabilities::BAND_2_4; // TODO: Should be based on the configured channel

  std::map<int, Capabilities> caps = this->_iface.GetCapabilities();
  if (caps.empty() || !caps.count(band))
  {
    return;
  }

  this->_beacon.ReceiverAddress("ff:ff:ff:ff:ff:ff");
  this->_beacon.TransmitterAddress(this->GetBssid());
  this->_beacon.Bssid(this->GetBssid());
  this->_beacon.Interval(100);
  this->_beacon.Capabilities(0x0421);
  this->_beacon.Ssid(this->GetSsid());
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
  this->_beacon.Dsss(this->_iface.GetChannel());
//  ieee80211::country_tag country; // TODO: Country tag is hardcoded for now

  ieee80211::country_tag country = { 'U', 'S', 0x20, 1, 11, 30 };
  this->_beacon.Country(country);
  this->_beacon.ErpInfo(0);
  if (!caps[band].GetExtBitRates().empty())
  {
    this->_beacon.ExtRates(caps[band].GetExtBitRates());
  }
  this->_beacon.SuppOpClass(81);
  this->_beacon.HtCaps = this->HtCaps;
//  this->_beacon.HtCaps(caps[band].GetHtCaps());
  this->_beacon.HtInfo = this->HtInfo;
//  this->_beacon.HtInfo(caps[band].GetHtInfo());
  this->_beacon.ExtCaps.SetFlag(ieee80211::ExtCapsTag::EXCAP_EXTENDED_CHANNEL_SWITCHING);
  this->_beacon.ExtCaps.SetFlag(ieee80211::ExtCapsTag::EXCAP_OPERATING_MODE_NOTIFICATION);

  ieee80211::WmmWmeTag::ac_parms ac0 = {0x03, 0xa4, 0x0000};
  ieee80211::WmmWmeTag::ac_parms ac1 = {0x27, 0xa4, 0x0000};
  ieee80211::WmmWmeTag::ac_parms ac2 = {0x42, 0x43, 0x5e00};
  ieee80211::WmmWmeTag::ac_parms ac3 = {0x62, 0x32, 0x2f00};
  ieee80211::WmmWmeTag::wmm_wme wmmwme = {0x01, 0x01, 0x84, 0x00, ac0, ac1, ac2, ac3};
  this->_beacon.WmmWme(wmmwme);

  this->_beacon.Display();

}

void
BasicServiceSet::_update_probe()
{

  zWireless::Capabilities::BAND band = zWireless::Capabilities::BAND_2_4; // TODO: Should be based on the configured channel

  std::map<int, Capabilities> caps = this->_iface.GetCapabilities();
  if (caps.empty() || !caps.count(band))
  {
    return;
  }

  this->_probe.ReceiverAddress("00:00:00:00:00:00");
  this->_probe.TransmitterAddress(this->GetBssid());
  this->_probe.Bssid(this->GetBssid());
  this->_probe.Interval(100);
  this->_probe.Capabilities(0x0421);
  this->_probe.Ssid(this->GetSsid());
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
  this->_probe.Dsss(this->_iface.GetChannel());
  ieee80211::country_tag country; // TODO: Country tag is hard coded for now
  this->_probe.Country(country);
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
