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

BasicServiceSet::BasicServiceSet(AccessPointInterface& iface_) :
    _iface(iface_), _config(iface_.GetConfig())
{
  this->_caps = this->_iface.GetCapabilities();

  // Update beacon and probe
  _init_beacon();
  _init_probe();

}

BasicServiceSet::~BasicServiceSet()
{
}

std::string
BasicServiceSet::GetSsid() const
{
  return (this->_config.GetSsid());
}

bool
BasicServiceSet::SetSsid(const std::string& ssid_)
{
  bool status = this->_config.SetSsid(ssid_);
  if (status)
  {
    this->_iface.SetSsid(ssid_);
    this->_beacon.Ssid(ssid_);
    this->_probe.Ssid(ssid_);
  }
  return (status);
}

std::string
BasicServiceSet::GetBssid() const
{
  return (this->_config.GetBssid());
}

bool
BasicServiceSet::SetBssid(const std::string& bssid_)
{
  bool status = this->_config.SetBssid(bssid_);
  if (status)
  {
    this->_iface.SetBssid(bssid_);
    this->_iface.SetHwAddress(bssid_);
    this->_beacon.Bssid(bssid_);
    this->_beacon.TransmitterAddress(this->_config.GetBssid());
    this->_probe.Bssid(bssid_);
    this->_probe.TransmitterAddress(this->_config.GetBssid());
    this->_assoc.Bssid(bssid_);
    this->_assoc.TransmitterAddress(this->_config.GetBssid());
  }
  return (status);
}

ConfigData::HTMODE
BasicServiceSet::GetHtMode() const
{
  return (this->_config.GetHtMode());
}

bool
BasicServiceSet::SetHtMode(const ConfigData::HTMODE mode_)
{
  bool status = this->_config.SetHtMode(mode_);
  if (status)
  {
    this->_iface.SetHtMode(mode_);
  }
  return (status);
}

unsigned int
BasicServiceSet::GetChannel() const
{
  return (this->_config.GetChannel());
}

bool
BasicServiceSet::SetChannel(const unsigned int channel_)
{
  bool status = this->_config.SetChannel(channel_);
  if (status)
  {
    this->_iface.SetChannel(channel_);
    this->_beacon.Dsss(channel_);
    this->_probe.Dsss(channel_);
  }
  return (status);
}

unsigned int
BasicServiceSet::GetFrequency() const
{
  return (this->_config.GetFrequency());
}

bool
BasicServiceSet::SetFrequency(const unsigned int freq_)
{
  bool status = this->_config.SetFrequency(freq_);
  if (status)
  {
    this->_iface.SetFrequency(freq_);
    this->_beacon.Dsss(this->_config.GetChannel());
    this->_probe.Dsss(this->_config.GetChannel());
  }
  return (status);
}

unsigned int
BasicServiceSet::GetCenterFrequency1() const
{
  return (this->_config.GetCenterFrequency1());
}

bool
BasicServiceSet::SetCenterFrequency1(const unsigned int freq_)
{
  bool status = this->_config.SetCenterFrequency1(freq_);
  if (status)
  {
    this->_iface.SetCenterFrequency1(freq_);
  }
  return (status);
}

unsigned int
BasicServiceSet::GetCenterFrequency2() const
{
  return (this->_config.GetCenterFrequency2());
}

bool
BasicServiceSet::SetCenterFrequency2(const unsigned int freq_)
{
  bool status = this->_config.SetCenterFrequency2(freq_);
  if (status)
  {
    this->_iface.SetCenterFrequency1(freq_);
  }
  return (status);
}

zWireless::Capabilities&
BasicServiceSet::Capabilities(zWireless::Capabilities::BAND band_)
{
  return (this->_caps[band_]);
}

ieee80211::Beacon&
BasicServiceSet::Beacon()
{
  return (this->_beacon);
}

ieee80211::ProbeResponse&
BasicServiceSet::ProbeResponse()
{
  return (this->_probe);
}

ieee80211::AssociationResponse&
BasicServiceSet::AssociationResponse()
{
  return (this->_assoc);
}

bool
BasicServiceSet::Start()
{

  // Verify interface exists
  if (!this->_iface.GetIfIndex())
  {
    ZLOG_ERR("Error starting AP interface: interface does not exist: " + this->_iface.GetIfName());
    return (false);
  }

  // Configure interface
  this->_iface.SetHwAddress(this->_config.GetBssid());
  this->_iface.SetAdminState(zWireless::ConfigData::STATE_UP);
  this->_iface.SetPromiscuousMode(zWireless::ConfigData::PROMODE_ENABLED);
  if (!this->_iface.Commit())
  {
    ZLOG_ERR("Error starting AP interface: cannot UP interface" + this->_iface.GetIfName());
    return (false);
  }

  // Update configuration
  this->_iface.SetSsid(this->_config.GetSsid());
  this->_iface.SetBssid(this->_config.GetBssid());
  this->_iface.SetHtMode(this->_config.GetHtMode());
  this->_iface.SetFrequency(this->_config.GetFrequency());
  this->_iface.SetCenterFrequency1(this->_config.GetCenterFrequency1());
  this->_iface.SetCenterFrequency2(this->_config.GetCenterFrequency2());

  // Start AP and return status
  return (this->_iface.Start(this->_beacon, this->_probe));

}

bool
BasicServiceSet::Stop()
{
  return (this->_iface.Stop());
}

bool
BasicServiceSet::Update()
{

  // Update configuration
  this->_iface.SetSsid(this->_config.GetSsid());
  this->_iface.SetBssid(this->_config.GetBssid());
  this->_iface.SetHtMode(this->_config.GetHtMode());
  this->_iface.SetFrequency(this->_config.GetFrequency());
  this->_iface.SetCenterFrequency1(this->_config.GetCenterFrequency1());
  this->_iface.SetCenterFrequency2(this->_config.GetCenterFrequency2());

  // Update AP and return status
  return (this->_iface.Update(this->_beacon, this->_probe));
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
  this->_iface.Display();
  this->_beacon.Display();
  this->_probe.Display();
  this->_assoc.Display();
}

void
BasicServiceSet::_init_beacon()
{
  zWireless::Capabilities::BAND band = zWireless::Capabilities::BAND_2_4;
  if (this->_config.GetFrequency() >= 5000)
  {
    zWireless::Capabilities::BAND band = zWireless::Capabilities::BAND_5;
  }

  this->_beacon.ReceiverAddress("ff:ff:ff:ff:ff:ff");
  this->_beacon.TransmitterAddress(this->_config.GetBssid());
  this->_beacon.Bssid(this->_config.GetBssid());
  this->_beacon.Interval(100);
  this->_beacon.Capabilities(0x0421);
  this->_beacon.Ssid(this->_config.GetSsid());
  this->_beacon.Rates(this->_caps[band].GetBitRates());
  this->_beacon.Dsss(this->_config.GetChannel());
  ieee80211::country_tag country = { 'U', 'S', 0x20, 1, 11, 30 };
  this->_beacon.Country(country);
  this->_beacon.ErpInfo(0);
  if (!this->_caps[band].GetExtBitRates().empty())
  {
    this->_beacon.ExtRates(this->_caps[band].GetExtBitRates());
  }
  this->_beacon.SuppOpClass(81);
  this->_beacon.HtCaps(this->_caps[band].GetHtCaps());
  this->_beacon.HtInfo(this->_caps[band].GetHtInfo());
  this->_beacon.ExtCaps.SetFlag(ieee80211::ExtCapsTag::EXCAP_EXTENDED_CHANNEL_SWITCHING);
  this->_beacon.ExtCaps.SetFlag(ieee80211::ExtCapsTag::EXCAP_OPERATING_MODE_NOTIFICATION);

  ieee80211::WmmWmeTag::ac_parms ac0 = {0x03, 0xa4, 0x0000};
  ieee80211::WmmWmeTag::ac_parms ac1 = {0x27, 0xa4, 0x0000};
  ieee80211::WmmWmeTag::ac_parms ac2 = {0x42, 0x43, 0x005e};
  ieee80211::WmmWmeTag::ac_parms ac3 = {0x62, 0x32, 0x002f};
  ieee80211::WmmWmeTag::wmm_wme wmmwme = {0x01, 0x01, 0x80, 0x00, ac0, ac1, ac2, ac3};
  this->_beacon.WmmWme(wmmwme);

}

void
BasicServiceSet::_init_probe()
{
  zWireless::Capabilities::BAND band = zWireless::Capabilities::BAND_2_4;
  if (this->_config.GetFrequency() >= 5000)
  {
    zWireless::Capabilities::BAND band = zWireless::Capabilities::BAND_5;
  }

  this->_probe.ReceiverAddress("00:00:00:00:00:00");
  this->_probe.TransmitterAddress(this->_config.GetBssid());
  this->_probe.Bssid(this->_config.GetBssid());
  this->_probe.Interval(100);
  this->_probe.Capabilities(0x0421);
  this->_probe.Ssid(this->_config.GetSsid());
  this->_probe.Rates(this->_caps[band].GetBitRates());
  this->_probe.Dsss(this->_config.GetChannel());
  ieee80211::country_tag country; // TODO: Country tag is hard coded for now
  this->_probe.Country(country);
  this->_probe.ErpInfo(0);
  if (!this->_caps[band].GetExtBitRates().empty())
  {
    this->_probe.ExtRates(this->_caps[band].GetExtBitRates());
  }
  this->_probe.SuppOpClass(81);
  this->_probe.HtCaps(this->_caps[band].GetHtCaps());
  this->_probe.HtInfo(this->_caps[band].GetHtInfo());
  this->_probe.ExtCaps.SetFlag(ieee80211::ExtCapsTag::EXCAP_EXTENDED_CHANNEL_SWITCHING);
  this->_probe.ExtCaps.SetFlag(ieee80211::ExtCapsTag::EXCAP_OPERATING_MODE_NOTIFICATION);

  ieee80211::WmmWmeTag::ac_parms ac0 = {0x03, 0xa4, 0x0000};
  ieee80211::WmmWmeTag::ac_parms ac1 = {0x27, 0xa4, 0x0000};
  ieee80211::WmmWmeTag::ac_parms ac2 = {0x42, 0x43, 0x005e};
  ieee80211::WmmWmeTag::ac_parms ac3 = {0x62, 0x32, 0x002f};
  ieee80211::WmmWmeTag::wmm_wme wmmwme = {0x01, 0x01, 0x80, 0x00, ac0, ac1, ac2, ac3};
  this->_probe.WmmWme(wmmwme);

}

}
}
