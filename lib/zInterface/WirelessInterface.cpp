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

#include <zutils/zCompatibility.h>
#include <zutils/zInterface.h>
#include <zutils/zWirelessInterface.h>

// local includes

#include "GetPhyCommand.h"
#include "GetInterfaceCommand.h"
#include "SetInterfaceCommand.h"
#include "NewInterfaceCommand.h"
#include "DelInterfaceCommand.h"
using namespace nl80211;

namespace zUtils
{
namespace zInterface
{

static const std::string sysfs_root("/sys/class/net/");
static const std::string sysfs_phy("/phy80211/name");

static double
_normalize(double min_, double max_, double val_, float weight_ = 1)
{
  double num = 0.0;

  // Test the value is within the range
  if (val_ < min_)
  {
    num = min_;
  }
  else if (val_ > max_)
  {
    num = max_;
  }
  else
  {
    num = (val_ - min_) / (max_ - min_) * (weight_);
  }

  return (num);
}

int
_dbm2mwatt(int dbm)
{
  return floor(pow(10, (((double) dbm) / 10)));
}

int
_mwatt2dbm(int mwatt)
{
  return ceil(10 * log10(mwatt));
}

static double
_freq2float(const struct iw_freq *freq)
{
  return ((double) freq->m) * pow(10, freq->e);
}

static void
_float2freq(double floatfreq, struct iw_freq *freq)
{
  freq->e = (short) floor(log10(floatfreq));
  if (freq->e > 8)
  {
    freq->m = ((long) (floor(floatfreq / pow(10, freq->e - 6)))) * 100;
    freq->e -= 8;
  }
  else
  {
    freq->m = (long) floatfreq;
    freq->e = 0;
  }
}

static std::string
_get_iwname(const std::string &name_)
{
  int sock = -1;
  struct iwreq iwr = { 0 };
  std::string wname;

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);

    // Query interface flags
    if (ioctl(sock, SIOCGIWNAME, &iwr) == 0)
    {
      wname = std::string(iwr.u.name);
    }
    close(sock);
  }
  return (wname);
}

static std::string
_get_phyname(const std::string &name_)
{
  std::string wname;

  // Construct path for sysfs file
  std::string sysfs_filename = sysfs_root + name_ + sysfs_phy;

  // Read sysfs file
  std::fstream fs;
  fs.open(sysfs_filename.c_str(), std::fstream::in);
  if (fs.is_open())
  {
    fs >> wname;
    fs.close();
  }
  return (wname);
}

static std::string
_get_bssid(const std::string &name_)
{
  int sock = -1;
  struct iwreq iwr = { 0 };
  uint8_t addr[6] = { 0 };
  char str[32] = { 0 };

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);

    // Query MAC address of access point
    if (ioctl(sock, SIOCGIWAP, &iwr) == 0)
    {
      memcpy(addr, iwr.u.ap_addr.sa_data, 6);
    }

    // Close socket
    close(sock);
  }

  snprintf(str, 32, "%02x:%02x:%02x:%02x:%02x:%02x", addr[0], addr[1], addr[2], addr[3], addr[4],
      addr[5]);

  return (std::string(str));

}

static std::string
_get_essid(const std::string &name_)
{
  int sock = -1;
  struct iwreq iwr = { 0 };
  std::string essid;

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);

    char essid_char[IW_ESSID_MAX_SIZE + 1] = { 0 };
    iwr.u.essid.pointer = (caddr_t *) essid_char;
    iwr.u.essid.length = IW_ESSID_MAX_SIZE;
    iwr.u.essid.flags = 0;
    if (ioctl(sock, SIOCGIWESSID, &iwr) == 0)
    {
      essid = std::string(essid_char);
    }

    // Close socket
    close(sock);

  }
  return (essid);
}

static int
__get_link_quality(const std::string &name_)
{
  int sock = -1;
  struct iwreq iwr = { 0 };
  int quality = -1;

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);

    struct iw_statistics iwstats = { 0 };
    iwr.u.data.pointer = &iwstats;
    iwr.u.data.length = sizeof(iwstats);
    if (ioctl(sock, SIOCGIWSTATS, &iwr) == 0)
    {
      if (iwstats.qual.updated & IW_QUAL_QUAL_UPDATED)
      {
        if (iwstats.qual.updated & IW_QUAL_DBM)
        {
          quality = (int) (100.0 * _normalize(0, 70, iwstats.qual.qual, 1));
        }
        else if (iwstats.qual.updated & IW_QUAL_RCPI)
        {
          quality = -1;
        }
        else
        {
          quality = iwstats.qual.qual;
        }
      }
    }

    // Close socket
    close(sock);
  }

  return (quality);

}

static int
__get_signal_level(const std::string &name_)
{
  int sock = -1;
  struct iwreq iwr = { 0 };
  int signal_level = 0;

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);

    struct iw_statistics iwstats = { 0 };
    iwr.u.data.pointer = &iwstats;
    iwr.u.data.length = sizeof(iwstats);
    if (ioctl(sock, SIOCGIWSTATS, &iwr) == 0)
    {
      if (iwstats.qual.updated & IW_QUAL_LEVEL_UPDATED)
      {
        if (iwstats.qual.updated & IW_QUAL_DBM)
        {
          double dbm = iwstats.qual.level - 256;
          signal_level = (int) (100.0 * _normalize(-100, -10, dbm, 1));
        }
        else if (iwstats.qual.updated & IW_QUAL_RCPI)
        {
          signal_level = -1;
        }
        else
        {
          signal_level = iwstats.qual.level;
        }
      }
    }

    // Close socket
    close(sock);
  }

  return (signal_level);

}

static int
__get_noise_level(const std::string &name_)
{
  int sock = -1;
  struct iwreq iwr = { 0 };
  int noise_level = -1;

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);

    struct iw_statistics iwstats = { 0 };
    iwr.u.data.pointer = &iwstats;
    iwr.u.data.length = sizeof(iwstats);
    if (ioctl(sock, SIOCGIWSTATS, &iwr) == 0)
    {
      if (iwstats.qual.updated & IW_QUAL_NOISE_UPDATED)
      {
        if (iwstats.qual.updated & IW_QUAL_DBM)
        {
          double dbm = iwstats.qual.noise - 256;
          noise_level = (int) (100.0 * _normalize(-100, 0, dbm, 1));
        }
        else if (iwstats.qual.updated & IW_QUAL_RCPI)
        {
          noise_level = -1;
        }
        else
        {
          noise_level = iwstats.qual.noise;
        }
      }
    }

    // Close socket
    close(sock);
  }

  return (noise_level);

}

static int
__get_bit_rate(const std::string &name_)
{
  int bit_rate = -1;
  int sock = -1;
  struct iwreq iwr = { 0 };

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);

    if (ioctl(sock, SIOCGIWRATE, &iwr) == 0)
    {
      if (!iwr.u.bitrate.disabled)
      {
        bit_rate = iwr.u.bitrate.value;
      }
    }

    // Close socket
    close(sock);
  }

  return (bit_rate);

}

static float
__get_channel(const std::string &name_)
{
  float chnl = -1;
  int sock = -1;
  struct iwreq iwr = { 0 };

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);

    if (ioctl(sock, SIOCGIWFREQ, &iwr) == 0)
    {
      chnl = iwr.u.freq.m;
    }

    // Close socket
    close(sock);
  }

  return (chnl);

}

static bool
__set_channel(const std::string &name_, float chnl_)
{
  int sock = -1;
  struct iwreq iwr = { 0 };

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);

//    if (ioctl(sock, SIOCSIWTXPOW, &iwr) == 0)
//    {
//      if (!iwr.u.txpower.disabled)
//      {
//        tx_power = iwr.u.txpower.value;
//      }
//    }

// Close socket
    close(sock);
  }

  return (true);
}

static int
__get_tx_power(const std::string &name_)
{
  int tx_power = -1;
  int sock = -1;
  struct iwreq iwr = { 0 };

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);

    if (ioctl(sock, SIOCGIWTXPOW, &iwr) == 0)
    {
      if (!iwr.u.txpower.disabled)
      {
        tx_power = iwr.u.txpower.value;
      }
    }

    // Close socket
    close(sock);
  }

  return (tx_power);

}

static bool
__set_tx_power(const std::string &name_, int tx_power_)
{
  int sock = -1;
  struct iwreq iwr = { 0 };

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
  {
    // Initialize interface request structure with name of interface
    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);

//    if (ioctl(sock, SIOCSIWTXPOW, &iwr) == 0)
//    {
//      if (!iwr.u.txpower.disabled)
//      {
//        tx_power = iwr.u.txpower.value;
//      }
//    }

// Close socket
    close(sock);
  }

  return (true);
}

static int
__get_error_rate(const std::string &name_)
{
  int err_rate = -1;
  return (err_rate);
}

static bool
_is_associated(const std::string &name_)
{
  bool associated = false;

  const std::string zero("00:00:00:00:00:00");
  const std::string bcast("FF:FF:FF:FF:FF:FF");
  const std::string hack("44:44:44:44:44:44");

  std::string addr(_get_bssid(name_));

  if ((addr != zero) && (addr != bcast) && (addr != hack))
  {
    associated = true;
  }

  return (associated);

}

// ****************************************************************************
// Class: WirelessConfigPath
// ****************************************************************************

const std::string WirelessConfigPath::ConfigPhyIndexPath("PhyIndex");
const std::string WirelessConfigPath::ConfigPhyNamePath("PhyName");
const std::string WirelessConfigPath::ConfigBssidPath("Bssid");
const std::string WirelessConfigPath::ConfigEssidPath("Essid");
const std::string WirelessConfigPath::ConfigSsidPath("Ssid");
const std::string WirelessConfigPath::ConfigChannelPath("Channel");
const std::string WirelessConfigPath::ConfigTxPowerPath("TxPower");

WirelessConfigPath::WirelessConfigPath()
{
}

WirelessConfigPath::~WirelessConfigPath()
{
}

zConfig::ConfigPath
WirelessConfigPath::PhyIndex() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigPhyIndexPath);
  return (path);
}

zConfig::ConfigPath
WirelessConfigPath::PhyName() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigPhyNamePath);
  return (path);
}

zConfig::ConfigPath
WirelessConfigPath::Bssid() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigBssidPath);
  return (path);
}

zConfig::ConfigPath
WirelessConfigPath::Essid() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigEssidPath);
  return (path);
}

zConfig::ConfigPath
WirelessConfigPath::Ssid() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigSsidPath);
  return (path);
}

zConfig::ConfigPath
WirelessConfigPath::Channel() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigChannelPath);
  return (path);
}

zConfig::ConfigPath
WirelessConfigPath::TxPower() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigTxPowerPath);
  return (path);
}

// ****************************************************************************
// Class: WirelessConfigData
// ****************************************************************************

const std::string WirelessConfigData::ConfigTypeNone("");
const std::string WirelessConfigData::ConfigTypeA("A");
const std::string WirelessConfigData::ConfigTypeB("B");
const std::string WirelessConfigData::ConfigTypeG("G");
const std::string WirelessConfigData::ConfigTypeN("N");
const std::string WirelessConfigData::ConfigTypeAC("AC");
const std::string WirelessConfigData::ConfigTypeAD("AD");
const std::string WirelessConfigData::ConfigTypeAX("AX");
const std::string WirelessConfigData::ConfigTypeDefault(ConfigTypeNone);

WirelessConfigData::WirelessConfigData() :
    zConfig::ConfigData(InterfaceConfigPath::ConfigRoot)
{
  ZLOG_DEBUG("WirelessConfigData::WirelessConfigData()");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

WirelessConfigData::WirelessConfigData(const zData::Data& data_) :
    zConfig::ConfigData(InterfaceConfigPath::ConfigRoot)
{
  this->PutChild(data_);
  ZLOG_DEBUG("WirelessConfigData::WirelessConfigData(data_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

WirelessConfigData::WirelessConfigData(const zConfig::ConfigData& config_) :
    zConfig::ConfigData(InterfaceConfigPath::ConfigRoot)
{
  this->PutChild(config_.GetData());
  ZLOG_DEBUG("WirelessConfigData::WirelessConfigData(config_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

WirelessConfigData::WirelessConfigData(const WirelessConfigData& other_) :
    zConfig::ConfigData(other_)
{
  ZLOG_DEBUG("WirelessConfigData::WirelessConfigData(other_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

WirelessConfigData::~WirelessConfigData()
{
}

WirelessConfigData::TYPE
WirelessConfigData::GetType() const
{
  WirelessConfigData::TYPE type = WirelessConfigData::TYPE_DEF;
  std::string str;
  InterfaceConfigPath path;
  if (this->GetValue(path.Type(), str))
  {
    if (str == WirelessConfigData::ConfigTypeNone)
    {
      type = WirelessConfigData::TYPE_NONE;
    }
    else if (str == WirelessConfigData::ConfigTypeA)
    {
      type = WirelessConfigData::TYPE_A;
    }
    else if (str == WirelessConfigData::ConfigTypeB)
    {
      type = WirelessConfigData::TYPE_B;
    }
    else if (str == WirelessConfigData::ConfigTypeG)
    {
      type = WirelessConfigData::TYPE_G;
    }
    else if (str == WirelessConfigData::ConfigTypeN)
    {
      type = WirelessConfigData::TYPE_N;
    }
    else if (str == WirelessConfigData::ConfigTypeAC)
    {
      type = WirelessConfigData::TYPE_AC;
    }
    else if (str == WirelessConfigData::ConfigTypeAD)
    {
      type = WirelessConfigData::TYPE_AD;
    }
    else if (str == WirelessConfigData::ConfigTypeAX)
    {
      type = WirelessConfigData::TYPE_AX;
    }
    else
    {
      type = WirelessConfigData::TYPE_ERR;
    }
  }
  return (type);
}

bool
WirelessConfigData::SetType(const WirelessConfigData::TYPE type_)
{
  bool status = true;
  InterfaceConfigPath path;
  std::string str;
  switch (type_)
  {
  case WirelessConfigData::TYPE_NONE:
    str = ConfigTypeNone;
    break;
  case WirelessConfigData::TYPE_A:
    str = ConfigTypeA;
    break;
  case WirelessConfigData::TYPE_B:
    str = ConfigTypeB;
    break;
  case WirelessConfigData::TYPE_G:
    str = ConfigTypeG;
    break;
  case WirelessConfigData::TYPE_N:
    str = ConfigTypeN;
    break;
  case WirelessConfigData::TYPE_AC:
    str = ConfigTypeAC;
    break;
  case WirelessConfigData::TYPE_AD:
    str = ConfigTypeAD;
    break;
  case WirelessConfigData::TYPE_AX:
    str = ConfigTypeAX;
    break;
  default:
    status = false;
  }
  return (this->PutValue(path.Type(), str));
}

// ****************************************************************************
// Class: WirelessInterface
// ****************************************************************************

const int WirelessInterface::ConfigPhyIndexDefault(0);
const std::string WirelessInterface::ConfigPhyNameDefault("");
const std::string WirelessInterface::ConfigBssidDefault("");
const std::string WirelessInterface::ConfigEssidDefault("");
const std::string WirelessInterface::ConfigSsidDefault("");
const float WirelessInterface::ConfigChannelDefault(0.0);
const int WirelessInterface::ConfigTxPowerDefault(0);

WirelessInterface::WirelessInterface(const InterfaceConfigData& config_) :
    Interface(config_), _associated(false), _link_quality(-1),
        _signal_level(0), _noise_level(-1), _bit_rate(0)
{
  ZLOG_DEBUG("WirelessInterface::WirelessInterface(config_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
  this->SetType(InterfaceConfigData::TYPE_WIRELESS);
  this->_lock.Unlock();
}

WirelessInterface::~WirelessInterface()
{
  //  std::cout << "Destroying: " << "WirelessInterface::~WirelessInterface" << std::endl;
  this->_lock.Lock();
}

bool
WirelessInterface::Refresh()
{

  bool status = false;
  const std::string name(this->GetName());
  nl80211::GetInterfaceCommand getifacecmd(name);

  if (!Interface::Refresh() || !getifacecmd.Exec())
  {
    return(false);
  }

  nl80211::GetPhyCommand getphycmd(getifacecmd.PhyIndex());

  if (getphycmd.Exec() && this->_lock.Lock())
  {
    // Initialize
    this->_associated = false;
    this->_link_quality = -1;
    this->_signal_level = 0;
    this->_noise_level = -1;
    this->_bit_rate = -1;

    // Get wireless parameters
    this->_iw_name = _get_iwname(name);

    if (this->GetPhyName() != _get_phyname(name))
    {
      this->SetPhyName(_get_phyname(name));
    }

    if (this->GetState() == InterfaceConfigData::STATE_UP)
    {
      this->_associated = _is_associated(name);

      if (this->GetBssid() != _get_bssid(name))
      {
        this->SetBssid(_get_bssid(name));
      }

      if (this->GetEssid() != _get_essid(name))
      {
        this->SetEssid(_get_essid(name));
      }

      this->_link_quality = this->_get_link_quality();
      this->_signal_level = this->_get_signal_level();
      this->_noise_level = this->_get_noise_level();
      this->_bit_rate = this->_get_bit_rate();

      if (this->GetChannel() != this->_get_channel())
      {
        this->SetChannel(this->_get_channel());
      }

      if (this->GetTxPower() != this->_get_tx_power())
      {
        this->SetTxPower(this->_get_tx_power());
      }
    }
    status = true;

    this->_lock.Unlock();
  }


  return (status);
}

bool
WirelessInterface::Create()
{
  bool status = false;
  return(status);
}

bool
WirelessInterface::Destroy()
{
  bool status = false;
  return(status);
}

void
WirelessInterface::Display(const std::string &prefix_)
{
  this->_lock.Lock();
  Interface::Display(prefix_);
  std::cout << prefix_ << " IWNAME:      \t" << this->_iw_name << std::endl;
  std::cout << prefix_ << "    PHY:      \t" << this->GetPhyName() << std::endl;
  std::cout << prefix_ << "  ESSID:      \t" << this->GetEssid() << std::endl;
  std::cout << prefix_ << "  BSSID:      \t" << this->GetBssid() << std::endl;
  std::cout << prefix_ << "   SSID:      \t" << this->GetBssid() << std::endl;
  std::cout << prefix_ << "CHANNEL:      \t" << this->GetChannel() << std::endl;
  std::cout << prefix_ << "TXPOWER:      \t" << this->GetTxPower() << std::endl;

  if (this->_associated)
  {
    if (this->_bit_rate / 1000000000)
    {
      std::cout << prefix_ << "Bit Rate:    \t" << (this->_bit_rate / 1000000000) << " Gbps"
          << std::endl;
    }
    else if (this->_bit_rate / 1000000)
    {
      std::cout << prefix_ << "Bit Rate:    \t" << (this->_bit_rate / 1000000) << " Mbps"
          << std::endl;
    }
    else if (this->_bit_rate / 1000)
    {
      std::cout << prefix_ << "Bit Rate:    \t" << (this->_bit_rate / 1000) << " Kbps"
          << std::endl;
    }
    else
    {
      std::cout << prefix_ << "Bit Rate:    \t" << this->_bit_rate << " bps" << std::endl;
    }
    std::cout << prefix_ << "Link Quality:\t" << this->_link_quality << std::endl;
    std::cout << prefix_ << "Signal Level:\t" << this->_signal_level << std::endl;
    std::cout << prefix_ << "Noise Level: \t" << this->_noise_level << std::endl;
  }
  else
  {
    std::cout << prefix_ << "Not Associated" << std::endl;
  }
  this->_lock.Unlock();
}

std::string
WirelessInterface::IwName()
{
  std::string iwname;
  this->_lock.Lock();
  iwname = this->_iw_name;
  this->_lock.Unlock();
  return (iwname);
}

std::string
WirelessInterface::GetPhyName() const
{
  std::string str;
  WirelessConfigPath path;
  if (!this->GetValue(path.PhyName(), str))
  {
    str = ConfigPhyNameDefault;
  }
  return (str);
}

bool
WirelessInterface::SetPhyName(const std::string& phy_)
{
  WirelessConfigPath path;
  return (this->PutValue(path.PhyName(), phy_));
}

std::string
WirelessInterface::GetBssid() const
{
  std::string str;
  WirelessConfigPath path;
  if (!this->GetValue(path.Bssid(), str))
  {
    str = ConfigBssidDefault;
  }
  return (str);
}

bool
WirelessInterface::SetBssid(const std::string& bssid_)
{
  WirelessConfigPath path;
  return (this->PutValue(path.Bssid(), bssid_));
}

std::string
WirelessInterface::GetEssid() const
{
  std::string str;
  WirelessConfigPath path;
  if (!this->GetValue(path.Essid(), str))
  {
    str = ConfigEssidDefault;
  }
  return (str);
}

bool
WirelessInterface::SetEssid(const std::string& essid_)
{
  WirelessConfigPath path;
  return (this->PutValue(path.Essid(), essid_));
}

std::string
WirelessInterface::GetSsid() const
{
  std::string str;
  WirelessConfigPath path;
  if (!this->GetValue(path.Ssid(), str))
  {
    str = ConfigSsidDefault;
  }
  return (str);
}

bool
WirelessInterface::SetSsid(const std::string& ssid_)
{
  WirelessConfigPath path;
  return (this->PutValue(path.Ssid(), ssid_));
}

float
WirelessInterface::GetChannel() const
{
  float chnl;
  WirelessConfigPath path;
  if (!this->GetValue(path.Channel(), chnl))
  {
    chnl = ConfigChannelDefault;
  }
  return (chnl);
}

bool
WirelessInterface::SetChannel(const float chnl_)
{
  WirelessConfigPath path;
  return (this->PutValue(path.Channel(), chnl_));
}

int
WirelessInterface::GetTxPower() const
{
  int txpow;
  WirelessConfigPath path;
  if (!this->GetValue(path.TxPower(), txpow))
  {
    txpow = ConfigTxPowerDefault;
  }
  return (txpow);
}

bool
WirelessInterface::SetTxPower(int txpow_)
{
  WirelessConfigPath path;
  return (this->PutValue(path.TxPower(), txpow_));
}

bool
WirelessInterface::Associate(const std::string &essid_, const unsigned int channel_)
{
  return (false);
}

bool
WirelessInterface::Disassociate()
{
  return (false);
}

bool
WirelessInterface::IsAssociated()
{
  bool ass_flag = false;
  this->_lock.Lock();
  ass_flag = this->_associated;
  this->_lock.Unlock();
  return (ass_flag);
}

int
WirelessInterface::LinkQuality()
{
  int qual;
  this->_lock.Lock();
  qual = this->_link_quality;
  this->_lock.Unlock();
  return (qual);
}

int
WirelessInterface::SignalLevel()
{
  int level;
  this->_lock.Lock();
  level = this->_signal_level;
  this->_lock.Unlock();
  return (level);
}

int
WirelessInterface::NoiseLevel()
{
  int level;
  this->_lock.Lock();
  level = this->_noise_level;
  this->_lock.Unlock();
  return (level);
}

int
WirelessInterface::BitRate()
{
  int rate;
  this->_lock.Lock();
  rate = this->_bit_rate;
  this->_lock.Unlock();
  return (rate);
}

int
WirelessInterface::_get_link_quality()
{
  return (__get_link_quality(this->GetName()));
}

int
WirelessInterface::_get_signal_level()
{
  return (__get_signal_level(this->GetName()));
}

int
WirelessInterface::_get_noise_level()
{
  return (__get_noise_level(this->GetName()));
}

int
WirelessInterface::_get_bit_rate()
{
  return (__get_bit_rate(this->GetName()));
}

float
WirelessInterface::_get_channel()
{
  return (__get_channel(this->GetName()));
}

bool
WirelessInterface::_set_channel(float chnl_)
{
  return (__set_channel(this->GetName(), chnl_));
}

int
WirelessInterface::_get_tx_power()
{
  return (__get_tx_power(this->GetName()));
}

bool
WirelessInterface::_set_tx_power(int txpow_)
{
  return (__set_tx_power(this->GetName(), txpow_));
}

}
}
