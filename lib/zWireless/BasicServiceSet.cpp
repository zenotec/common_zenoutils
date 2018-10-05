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
#include <zutils/ieee80211/ChannelSwitchTag.h>
#include <zutils/zStation.h>
#include <zutils/nl80211/NewStationCommand.h>
#include <zutils/nl80211/DelStationCommand.h>
using namespace nl80211;

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{

// ****************************************************************************
// Class: BasicServiceSet
// ****************************************************************************

BasicServiceSet::BasicServiceSet(const std::string& ifname_) :
    _iface(ifname_)
{

  // Initialize beacon and probe frame templates
  if (this->_iface.Refresh())
  {
    _init_beacon();
    _init_probe();
  }
  else
  {
    ZLOG_CRIT("Error creating BSS, interface does not exist: " + ifname_);
  }
  return;

}

BasicServiceSet::~BasicServiceSet()
{
}

const zWireless::AccessPointInterface&
BasicServiceSet::GetInterface() const
{
  return (this->_iface);
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

    std::map<int, zWireless::Capabilities> caps = this->_iface.GetCapabilities();
    zWireless::Capabilities::BAND band = (channel_ <= 11) ? zWireless::Capabilities::BAND_2_4 : zWireless::Capabilities::BAND_5;
    struct ht_info htinfo = this->_beacon.HtInfo();
    ieee80211::country_tag country = this->_beacon.Country();
    uint16_t supOp = 0;

    htinfo.ht_primary_channel = channel_;
    country.first_channel = channel_;

    if (channel_ < 36)
    {
    	  country.first_channel = 1;
      	  country.num_channels = 11;
  		  country.max_power = 21;
      	  supOp = 81;
    }
    else
    {
  	  country.first_channel = 36;
  	  country.num_channels = 24;
  	  supOp = 115;
  	  if (channel_ <= 104)
  	  {
  		  country.max_power = 20;
  	  }
  	  else if (channel_ <= 120)
  	  {
  		  country.max_power = 19;
  	  }
  	  else if (channel_ <= 149)
  	  {
  		  country.max_power = 18;
  	  }
  	  else
  	  {
  		  country.max_power = 17;
  	  }
    }

    if (!caps[band].GetExtBitRates().empty())
    {
    	this->_beacon.ExtRates(caps[band].GetExtBitRates());
    	this->_probe.ExtRates(caps[band].GetExtBitRates());
    }

    this->_beacon.Dsss(channel_);
    this->_beacon.HtInfo(htinfo);
    this->_beacon.HtCaps(caps[band].GetHtCaps());
    this->_beacon.Country(country);
    this->_beacon.SuppOpClass(supOp);
    this->_beacon.Rates(caps[band].GetBitRates());

    this->_probe.Dsss(channel_);
    this->_probe.HtInfo(htinfo);
    this->_probe.HtCaps(caps[band].GetHtCaps());
    this->_probe.Country(country);
    this->_probe.SuppOpClass(supOp);
    this->_probe.Rates(caps[band].GetBitRates());
  }
  return (status);
}

bool
BasicServiceSet::AnnounceChannelSwitch(const unsigned int mode_, const unsigned int channel_, const unsigned int count_)
{
    ieee80211::ChannelSwitchTag::channel_switch_tag chanSwitch = { uint8_t(mode_), uint8_t(channel_), uint8_t(count_)};
    this->_beacon.ChannelSwitch(chanSwitch);
    this->_probe.ChannelSwitch(chanSwitch);

    // TODO: need to remove the channel switch tag from the probe & beacon after we switch...

    return true;
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
	status = SetChannel(this->_config.GetChannel());
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
BasicServiceSet::AddStation(const zWireless::Station& station_)
{
  bool status = true;

  if (!this->_iface.GetIfIndex())
  {
    ZLOG_ERR("Error adding Station: interface does not exist: " + this->_iface.GetIfName());
    return (false);
  }


  // Create add STA command
  NewStationCommand* cmd = new NewStationCommand(this->_iface.GetIfIndex());

  cmd->IfIndex(this->_iface.GetIfIndex());
  cmd->IfName(this->_iface.GetIfName());
  cmd->ListenInterval(station_.GetListenInterval());
  cmd->Mac(station_.GetAddress());
  cmd->StaAid(station_.GetAssociationId());
  cmd->StaSupportedRates(station_.GetSupportedRates());
  struct nl80211_sta_flag_update sta_flags = { station_.GetFlags(), station_.GetFlags() };
  cmd->StaFlags(&sta_flags);
  cmd->HtCapabilties(station_.GetHtCapabilities());

  if (!cmd->Exec())
  {
    status = false;
    ZLOG_ERR("Cannot execute AddStation command");
  }
  delete (cmd);
  return (status);
}

bool
BasicServiceSet::DelStation(const zWireless::Station& station_)
{
  bool status = true;
  if (!this->_iface.GetIfIndex())
  {
    ZLOG_ERR("Error deleting Sta: interface does not exist: " + this->_iface.GetIfName());
    return (false);
  }

  // Create del STA command
  DelStationCommand* cmd = new DelStationCommand(this->_iface.GetIfIndex());

  cmd->Mac(station_.GetAddress());

  if (!cmd->Exec())
  {
    status = false;
    ZLOG_ERR("Cannot execute AddStation command");
  }
  delete (cmd);
  return (status);
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
    ZLOG_ERR("Error starting AP interface: cannot UP interface: " + this->_iface.GetIfName());
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
	// Initialize for 2.4ghz, channel 1

  std::map<int, zWireless::Capabilities> caps = this->_iface.GetCapabilities();

  uint8_t channel = 1;
  zWireless::Capabilities::BAND band = zWireless::Capabilities::BAND_2_4;
  uint16_t supportedOpClass = 81;
  ieee80211::country_tag country = { 'U', 'S', 0x20, 1, 11, 21 };

  this->_beacon.ReceiverAddress("ff:ff:ff:ff:ff:ff");
  this->_beacon.TransmitterAddress(this->_config.GetBssid());
  this->_beacon.Bssid(this->_config.GetBssid());
  this->_beacon.Interval(100);
  this->_beacon.Capabilities(0x0021);
  this->_beacon.Ssid(this->_config.GetSsid());
  this->_beacon.Rates(caps[band].GetBitRates());
  this->_beacon.Dsss(channel);
  this->_beacon.Country(country);
  this->_beacon.ErpInfo(0);
  this->_beacon.SuppOpClass(supportedOpClass);
  this->_beacon.HtCaps(caps[band].GetHtCaps());
  if (!caps[band].GetExtBitRates().empty())
  {
    this->_beacon.ExtRates(caps[band].GetExtBitRates());
  }

  // Get HT information
  struct ht_info htinfo = { 0 };
  htinfo.ht_primary_channel = channel;
  htinfo.ht_subset_1 = 0x00;
  htinfo.ht_subset_2 = 0x0000;
  htinfo.ht_subset_3 = 0x0000;
  htinfo.ht_rx_mcs.rx_mcs_bitmask = { 0 };
  htinfo.ht_rx_mcs.rx_highest_rate = 0x0000;
  htinfo.ht_rx_mcs.tx_mcs_fields.tx_bits = 0x00;
  this->_beacon.HtInfo(htinfo);

  this->_beacon.ExtCaps.SetFlag(EXCAP_EXTENDED_CHANNEL_SWITCHING);
  this->_beacon.ExtCaps.SetFlag(EXCAP_OPERATING_MODE_NOTIFICATION);

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
  std::map<int, zWireless::Capabilities> caps = this->_iface.GetCapabilities();

  uint8_t channel = 1;
  zWireless::Capabilities::BAND band = zWireless::Capabilities::BAND_2_4;
  uint16_t supportedOpClass = 81;
  ieee80211::country_tag country = { 'U', 'S', 0x20, 1, 11, 21 };

  this->_probe.ReceiverAddress("00:00:00:00:00:00");
  this->_probe.TransmitterAddress(this->_config.GetBssid());
  this->_probe.Bssid(this->_config.GetBssid());
  this->_probe.Interval(100);
  this->_probe.Capabilities(0x0021);
  this->_probe.Ssid(this->_config.GetSsid());
  this->_probe.Rates(caps[band].GetBitRates());
  this->_probe.Dsss(this->_config.GetChannel());
  this->_probe.Country(country);
  this->_probe.ErpInfo(0);
  this->_probe.SuppOpClass(supportedOpClass);
  this->_probe.HtCaps(caps[band].GetHtCaps());
  if (!caps[band].GetExtBitRates().empty())
  {
    this->_probe.ExtRates(caps[band].GetExtBitRates());
  }

  // Get HT information
  struct ht_info htinfo = { 0 };
  htinfo.ht_primary_channel = channel;
  htinfo.ht_subset_1 = 0x00;
  htinfo.ht_subset_2 = 0x0000;
  htinfo.ht_subset_3 = 0x0000;
  htinfo.ht_rx_mcs.rx_mcs_bitmask = { 0 };
  htinfo.ht_rx_mcs.rx_highest_rate = 0x0000;
  htinfo.ht_rx_mcs.tx_mcs_fields.tx_bits = 0x00;
  this->_probe.HtInfo(htinfo);

  this->_probe.ExtCaps.SetFlag(EXCAP_EXTENDED_CHANNEL_SWITCHING);
  this->_probe.ExtCaps.SetFlag(EXCAP_OPERATING_MODE_NOTIFICATION);

  ieee80211::WmmWmeTag::ac_parms ac0 = {0x03, 0xa4, 0x0000};
  ieee80211::WmmWmeTag::ac_parms ac1 = {0x27, 0xa4, 0x0000};
  ieee80211::WmmWmeTag::ac_parms ac2 = {0x42, 0x43, 0x005e};
  ieee80211::WmmWmeTag::ac_parms ac3 = {0x62, 0x32, 0x002f};
  ieee80211::WmmWmeTag::wmm_wme wmmwme = {0x01, 0x01, 0x80, 0x00, ac0, ac1, ac2, ac3};
  this->_probe.WmmWme(wmmwme);

}


}
}
