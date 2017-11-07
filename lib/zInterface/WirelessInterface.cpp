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

//static const std::string sysfs_root("/sys/class/net/");
//static const std::string sysfs_phy("/phy80211/name");
//
//static double
//_normalize(double min_, double max_, double val_, float weight_ = 1)
//{
//  double num = 0.0;
//
//  // Test the value is within the range
//  if (val_ < min_)
//  {
//    num = min_;
//  }
//  else if (val_ > max_)
//  {
//    num = max_;
//  }
//  else
//  {
//    num = (val_ - min_) / (max_ - min_) * (weight_);
//  }
//
//  return (num);
//}
//
//int
//_dbm2mwatt(int dbm)
//{
//  return floor(pow(10, (((double) dbm) / 10)));
//}
//
//int
//_mwatt2dbm(int mwatt)
//{
//  return ceil(10 * log10(mwatt));
//}
//
//static double
//_freq2float(const struct iw_freq *freq)
//{
//  return ((double) freq->m) * pow(10, freq->e);
//}
//
//static void
//_float2freq(double floatfreq, struct iw_freq *freq)
//{
//  freq->e = (short) floor(log10(floatfreq));
//  if (freq->e > 8)
//  {
//    freq->m = ((long) (floor(floatfreq / pow(10, freq->e - 6)))) * 100;
//    freq->e -= 8;
//  }
//  else
//  {
//    freq->m = (long) floatfreq;
//    freq->e = 0;
//  }
//}
//
//static std::string
//_get_iwname(const std::string &name_)
//{
//  int sock = -1;
//  struct iwreq iwr = { 0 };
//  std::string wname;
//
//  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
//  {
//    // Initialize interface request structure with name of interface
//    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);
//
//    // Query interface flags
//    if (ioctl(sock, SIOCGIWNAME, &iwr) == 0)
//    {
//      wname = std::string(iwr.u.name);
//    }
//    close(sock);
//  }
//  return (wname);
//}
//
//static std::string
//_get_phyname(const std::string &name_)
//{
//  std::string wname;
//
//  // Construct path for sysfs file
//  std::string sysfs_filename = sysfs_root + name_ + sysfs_phy;
//
//  // Read sysfs file
//  std::fstream fs;
//  fs.open(sysfs_filename.c_str(), std::fstream::in);
//  if (fs.is_open())
//  {
//    fs >> wname;
//    fs.close();
//  }
//  return (wname);
//}
//
//static std::string
//_get_bssid(const std::string &name_)
//{
//  int sock = -1;
//  struct iwreq iwr = { 0 };
//  uint8_t addr[6] = { 0 };
//  char str[32] = { 0 };
//
//  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
//  {
//    // Initialize interface request structure with name of interface
//    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);
//
//    // Query MAC address of access point
//    if (ioctl(sock, SIOCGIWAP, &iwr) == 0)
//    {
//      memcpy(addr, iwr.u.ap_addr.sa_data, 6);
//    }
//
//    // Close socket
//    close(sock);
//  }
//
//  snprintf(str, 32, "%02x:%02x:%02x:%02x:%02x:%02x", addr[0], addr[1], addr[2], addr[3], addr[4],
//      addr[5]);
//
//  return (std::string(str));
//
//}
//
//static std::string
//_get_essid(const std::string &name_)
//{
//  int sock = -1;
//  struct iwreq iwr = { 0 };
//  std::string essid;
//
//  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
//  {
//    // Initialize interface request structure with name of interface
//    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);
//
//    char essid_char[IW_ESSID_MAX_SIZE + 1] = { 0 };
//    iwr.u.essid.pointer = (caddr_t *) essid_char;
//    iwr.u.essid.length = IW_ESSID_MAX_SIZE;
//    iwr.u.essid.flags = 0;
//    if (ioctl(sock, SIOCGIWESSID, &iwr) == 0)
//    {
//      essid = std::string(essid_char);
//    }
//
//    // Close socket
//    close(sock);
//
//  }
//  return (essid);
//}
//
//static int
//__get_link_quality(const std::string &name_)
//{
//  int sock = -1;
//  struct iwreq iwr = { 0 };
//  int quality = -1;
//
//  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
//  {
//    // Initialize interface request structure with name of interface
//    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);
//
//    struct iw_statistics iwstats = { 0 };
//    iwr.u.data.pointer = &iwstats;
//    iwr.u.data.length = sizeof(iwstats);
//    if (ioctl(sock, SIOCGIWSTATS, &iwr) == 0)
//    {
//      if (iwstats.qual.updated & IW_QUAL_QUAL_UPDATED)
//      {
//        if (iwstats.qual.updated & IW_QUAL_DBM)
//        {
//          quality = (int) (100.0 * _normalize(0, 70, iwstats.qual.qual, 1));
//        }
//        else if (iwstats.qual.updated & IW_QUAL_RCPI)
//        {
//          quality = -1;
//        }
//        else
//        {
//          quality = iwstats.qual.qual;
//        }
//      }
//    }
//
//    // Close socket
//    close(sock);
//  }
//
//  return (quality);
//
//}
//
//static int
//__get_signal_level(const std::string &name_)
//{
//  int sock = -1;
//  struct iwreq iwr = { 0 };
//  int signal_level = 0;
//
//  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
//  {
//    // Initialize interface request structure with name of interface
//    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);
//
//    struct iw_statistics iwstats = { 0 };
//    iwr.u.data.pointer = &iwstats;
//    iwr.u.data.length = sizeof(iwstats);
//    if (ioctl(sock, SIOCGIWSTATS, &iwr) == 0)
//    {
//      if (iwstats.qual.updated & IW_QUAL_LEVEL_UPDATED)
//      {
//        if (iwstats.qual.updated & IW_QUAL_DBM)
//        {
//          double dbm = iwstats.qual.level - 256;
//          signal_level = (int) (100.0 * _normalize(-100, -10, dbm, 1));
//        }
//        else if (iwstats.qual.updated & IW_QUAL_RCPI)
//        {
//          signal_level = -1;
//        }
//        else
//        {
//          signal_level = iwstats.qual.level;
//        }
//      }
//    }
//
//    // Close socket
//    close(sock);
//  }
//
//  return (signal_level);
//
//}
//
//static int
//__get_noise_level(const std::string &name_)
//{
//  int sock = -1;
//  struct iwreq iwr = { 0 };
//  int noise_level = -1;
//
//  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
//  {
//    // Initialize interface request structure with name of interface
//    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);
//
//    struct iw_statistics iwstats = { 0 };
//    iwr.u.data.pointer = &iwstats;
//    iwr.u.data.length = sizeof(iwstats);
//    if (ioctl(sock, SIOCGIWSTATS, &iwr) == 0)
//    {
//      if (iwstats.qual.updated & IW_QUAL_NOISE_UPDATED)
//      {
//        if (iwstats.qual.updated & IW_QUAL_DBM)
//        {
//          double dbm = iwstats.qual.noise - 256;
//          noise_level = (int) (100.0 * _normalize(-100, 0, dbm, 1));
//        }
//        else if (iwstats.qual.updated & IW_QUAL_RCPI)
//        {
//          noise_level = -1;
//        }
//        else
//        {
//          noise_level = iwstats.qual.noise;
//        }
//      }
//    }
//
//    // Close socket
//    close(sock);
//  }
//
//  return (noise_level);
//
//}
//
//static int
//__get_bit_rate(const std::string &name_)
//{
//  int bit_rate = -1;
//  int sock = -1;
//  struct iwreq iwr = { 0 };
//
//  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
//  {
//    // Initialize interface request structure with name of interface
//    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);
//
//    if (ioctl(sock, SIOCGIWRATE, &iwr) == 0)
//    {
//      if (!iwr.u.bitrate.disabled)
//      {
//        bit_rate = iwr.u.bitrate.value;
//      }
//    }
//
//    // Close socket
//    close(sock);
//  }
//
//  return (bit_rate);
//
//}
//
//static float
//__get_channel(const std::string &name_)
//{
//  float chnl = -1;
//  int sock = -1;
//  struct iwreq iwr = { 0 };
//
//  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
//  {
//    // Initialize interface request structure with name of interface
//    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);
//
//    if (ioctl(sock, SIOCGIWFREQ, &iwr) == 0)
//    {
//      chnl = iwr.u.freq.m;
//    }
//
//    // Close socket
//    close(sock);
//  }
//
//  return (chnl);
//
//}
//
//static bool
//__set_channel(const std::string &name_, float chnl_)
//{
//  int sock = -1;
//  struct iwreq iwr = { 0 };
//
//  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
//  {
//    // Initialize interface request structure with name of interface
//    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);
//
////    if (ioctl(sock, SIOCSIWTXPOW, &iwr) == 0)
////    {
////      if (!iwr.u.txpower.disabled)
////      {
////        tx_power = iwr.u.txpower.value;
////      }
////    }
//
//// Close socket
//    close(sock);
//  }
//
//  return (true);
//}
//
//static int
//__get_tx_power(const std::string &name_)
//{
//  int tx_power = -1;
//  int sock = -1;
//  struct iwreq iwr = { 0 };
//
//  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
//  {
//    // Initialize interface request structure with name of interface
//    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);
//
//    if (ioctl(sock, SIOCGIWTXPOW, &iwr) == 0)
//    {
//      if (!iwr.u.txpower.disabled)
//      {
//        tx_power = iwr.u.txpower.value;
//      }
//    }
//
//    // Close socket
//    close(sock);
//  }
//
//  return (tx_power);
//
//}
//
//static bool
//__set_tx_power(const std::string &name_, int tx_power_)
//{
//  int sock = -1;
//  struct iwreq iwr = { 0 };
//
//  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) > 0)
//  {
//    // Initialize interface request structure with name of interface
//    strncpy(iwr.ifr_name, name_.c_str(), IFNAMSIZ);
//
////    if (ioctl(sock, SIOCSIWTXPOW, &iwr) == 0)
////    {
////      if (!iwr.u.txpower.disabled)
////      {
////        tx_power = iwr.u.txpower.value;
////      }
////    }
//
//// Close socket
//    close(sock);
//  }
//
//  return (true);
//}
//
//static int
//__get_error_rate(const std::string &name_)
//{
//  int err_rate = -1;
//  return (err_rate);
//}
//
//static bool
//_is_associated(const std::string &name_)
//{
//  bool associated = false;
//
//  const std::string zero("00:00:00:00:00:00");
//  const std::string bcast("FF:FF:FF:FF:FF:FF");
//  const std::string hack("44:44:44:44:44:44");
//
//  std::string addr(_get_bssid(name_));
//
//  if ((addr != zero) && (addr != bcast) && (addr != hack))
//  {
//    associated = true;
//  }
//
//  return (associated);
//
//}
//

// ****************************************************************************
// Class: WirelessInterfaceConfigPath
// ****************************************************************************

const std::string WirelessInterfaceConfigPath::ConfigPhyIndexPath("PhyIndex");
const std::string WirelessInterfaceConfigPath::ConfigPhyNamePath("PhyName");
const std::string WirelessInterfaceConfigPath::ConfigHwModePath("HwMode");
const std::string WirelessInterfaceConfigPath::ConfigHtModePath("HtMode");
const std::string WirelessInterfaceConfigPath::ConfigOpModePath("OpMode");
//const std::string WirelessInterfaceConfigPath::ConfigBssidPath("Bssid");
//const std::string WirelessInterfaceConfigPath::ConfigEssidPath("Essid");
//const std::string WirelessInterfaceConfigPath::ConfigSsidPath("Ssid");
//const std::string WirelessInterfaceConfigPath::ConfigChannelPath("Channel");
//const std::string WirelessInterfaceConfigPath::ConfigTxPowerPath("TxPower");

WirelessInterfaceConfigPath::WirelessInterfaceConfigPath(const std::string& root_)
{
  if (!root_.empty())
  {
    this->Append(root_);
  }
}

WirelessInterfaceConfigPath::WirelessInterfaceConfigPath(const WirelessInterfaceConfigPath& other_) :
    zInterface::ConfigPath(other_)
{
}

WirelessInterfaceConfigPath::WirelessInterfaceConfigPath(const zData::DataPath& path_)
{
}

WirelessInterfaceConfigPath::~WirelessInterfaceConfigPath()
{
}

// ****************************************************************************
// Class: WirelessInterfaceConfigData
// ****************************************************************************

const unsigned int WirelessInterfaceConfigData::ConfigPhyIndexDefault(0);

const std::string WirelessInterfaceConfigData::ConfigPhyNameDefault("");

const std::string WirelessInterfaceConfigData::ConfigHwModeNone("");
const std::string WirelessInterfaceConfigData::ConfigHwModeA("A");
const std::string WirelessInterfaceConfigData::ConfigHwModeB("B");
const std::string WirelessInterfaceConfigData::ConfigHwModeG("G");
const std::string WirelessInterfaceConfigData::ConfigHwModeN("N");
const std::string WirelessInterfaceConfigData::ConfigHwModeAC("AC");
const std::string WirelessInterfaceConfigData::ConfigHwModeAD("AD");
const std::string WirelessInterfaceConfigData::ConfigHwModeAX("AX");
const std::string WirelessInterfaceConfigData::ConfigHwModeDefault(WirelessInterfaceConfigData::ConfigHwModeNone);

const std::string WirelessInterfaceConfigData::ConfigOpModeNone("");
const std::string WirelessInterfaceConfigData::ConfigOpModeSTA("STATION");
const std::string WirelessInterfaceConfigData::ConfigOpModeAP("AP");
const std::string WirelessInterfaceConfigData::ConfigOpModeAdhoc("ADHOC");
const std::string WirelessInterfaceConfigData::ConfigOpModeMonitor("MONITOR");
const std::string WirelessInterfaceConfigData::ConfigOpModeMesh("MESH");
const std::string WirelessInterfaceConfigData::ConfigOpModeDefault(WirelessInterfaceConfigData::ConfigOpModeNone);

//const std::string WirelessInterfaceConfigData::ConfigBssidDefault("");
//const std::string WirelessInterfaceConfigData::ConfigEssidDefault("");
//const std::string WirelessInterfaceConfigData::ConfigSsidDefault("");
//const float WirelessInterfaceConfigData::ConfigChannelDefault(0.0);
//const int WirelessInterfaceConfigData::ConfigTxPowerDefault(0);

WirelessInterfaceConfigData::WirelessInterfaceConfigData() :
    zConfig::ConfigData(ConfigPath::ConfigRoot)
{
  ZLOG_DEBUG("WirelessConfigData::WirelessConfigData()");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

WirelessInterfaceConfigData::WirelessInterfaceConfigData(const zData::Data& data_) :
    zConfig::ConfigData(ConfigPath::ConfigRoot)
{
  this->PutChild(data_);
  ZLOG_DEBUG("WirelessConfigData::WirelessConfigData(data_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

WirelessInterfaceConfigData::WirelessInterfaceConfigData(const zConfig::ConfigData& config_) :
    zConfig::ConfigData(ConfigPath::ConfigRoot)
{
  this->PutChild(config_.GetData());
  ZLOG_DEBUG("WirelessConfigData::WirelessConfigData(config_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

WirelessInterfaceConfigData::WirelessInterfaceConfigData(const WirelessInterfaceConfigData& other_) :
    zConfig::ConfigData(other_)
{
  ZLOG_DEBUG("WirelessConfigData::WirelessConfigData(other_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

WirelessInterfaceConfigData::~WirelessInterfaceConfigData()
{
}

unsigned int
WirelessInterfaceConfigData::PhyIndex() const
{
  unsigned int val = 0;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigPhyIndexPath);
  if (!this->GetValue<unsigned int>(path, val))
  {
    val = ConfigPhyIndexDefault;
  }
  return (val);
}

bool
WirelessInterfaceConfigData::PhyIndex(const unsigned int index_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigPhyIndexPath);
  return (this->PutValue(path, index_));
}

std::string
WirelessInterfaceConfigData::PhyName() const
{
  std::string str;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigPhyNamePath);
  if (!this->GetValue(path, str))
  {
    str = ConfigPhyNameDefault;
  }
  return (str);
}

bool
WirelessInterfaceConfigData::PhyName(const std::string& name_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigPhyNamePath);
  return (this->PutValue(path, name_));
}

std::string
WirelessInterfaceConfigData::HwMode() const
{
  std::string str;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigHwModePath);
  if (!this->GetValue(path, str))
  {
    str = ConfigHwModeDefault;
  }
  return (str);

}

bool
WirelessInterfaceConfigData::HwMode(const std::string& mode_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigHwModePath);
  return (this->PutValue(path, mode_));
}

std::string
WirelessInterfaceConfigData::OpMode() const
{
  std::string str;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigOpModePath);
  if (!this->GetValue(path, str))
  {
    str = ConfigOpModeDefault;
  }
  return (str);

}

bool
WirelessInterfaceConfigData::OpMode(const std::string& mode_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigOpModePath);
  return (this->PutValue(path, mode_));
}
//
//WirelessInterfaceConfigData::TYPE
//WirelessInterfaceConfigData::GetType() const
//{
//  WirelessInterfaceConfigData::TYPE type = WirelessInterfaceConfigData::TYPE_DEF;
//  std::string str;
//  ConfigPath path;
//  if (this->GetValue(path.Type(), str))
//  {
//    if (str == WirelessInterfaceConfigData::ConfigTypeNone)
//    {
//      type = WirelessInterfaceConfigData::TYPE_NONE;
//    }
//    else if (str == WirelessInterfaceConfigData::ConfigTypeA)
//    {
//      type = WirelessInterfaceConfigData::TYPE_A;
//    }
//    else if (str == WirelessInterfaceConfigData::ConfigTypeB)
//    {
//      type = WirelessInterfaceConfigData::TYPE_B;
//    }
//    else if (str == WirelessInterfaceConfigData::ConfigTypeG)
//    {
//      type = WirelessInterfaceConfigData::TYPE_G;
//    }
//    else if (str == WirelessInterfaceConfigData::ConfigTypeN)
//    {
//      type = WirelessInterfaceConfigData::TYPE_N;
//    }
//    else if (str == WirelessInterfaceConfigData::ConfigTypeAC)
//    {
//      type = WirelessInterfaceConfigData::TYPE_AC;
//    }
//    else if (str == WirelessInterfaceConfigData::ConfigTypeAD)
//    {
//      type = WirelessInterfaceConfigData::TYPE_AD;
//    }
//    else if (str == WirelessInterfaceConfigData::ConfigTypeAX)
//    {
//      type = WirelessInterfaceConfigData::TYPE_AX;
//    }
//    else
//    {
//      type = WirelessInterfaceConfigData::TYPE_ERR;
//    }
//  }
//  return (type);
//}
//
//bool
//WirelessInterfaceConfigData::SetType(const WirelessInterfaceConfigData::TYPE type_)
//{
//  bool status = true;
//  ConfigPath path;
//  std::string str;
//  switch (type_)
//  {
//  case WirelessInterfaceConfigData::TYPE_NONE:
//    str = ConfigTypeNone;
//    break;
//  case WirelessInterfaceConfigData::TYPE_A:
//    str = ConfigTypeA;
//    break;
//  case WirelessInterfaceConfigData::TYPE_B:
//    str = ConfigTypeB;
//    break;
//  case WirelessInterfaceConfigData::TYPE_G:
//    str = ConfigTypeG;
//    break;
//  case WirelessInterfaceConfigData::TYPE_N:
//    str = ConfigTypeN;
//    break;
//  case WirelessInterfaceConfigData::TYPE_AC:
//    str = ConfigTypeAC;
//    break;
//  case WirelessInterfaceConfigData::TYPE_AD:
//    str = ConfigTypeAD;
//    break;
//  case WirelessInterfaceConfigData::TYPE_AX:
//    str = ConfigTypeAX;
//    break;
//  default:
//    status = false;
//  }
//  return (this->PutValue(path.Type(), str));
//}

// ****************************************************************************
// Class: WirelessInterface
// ****************************************************************************

WirelessInterface::WirelessInterface(const int index_) :
    Interface(index_), _getphycmd(NULL), _getifacecmd(NULL), _setifacecmd(NULL),
    _newifacecmd(NULL), _delifacecmd(NULL)
{
  this->Config.Type(zInterface::ConfigData::ConfigTypeWireless);
  this->_getphycmd = new GetPhyCommand;
  this->_getifacecmd = new GetInterfaceCommand(index_);
  this->_setifacecmd = new SetInterfaceCommand(index_);
  this->_delifacecmd = new DelInterfaceCommand(index_);
}

WirelessInterface::WirelessInterface(const std::string& name_) :
    Interface(name_), _getphycmd(NULL), _getifacecmd(NULL), _setifacecmd(NULL),
    _newifacecmd(NULL), _delifacecmd(NULL)
{
  this->Config.Type(zInterface::ConfigData::ConfigTypeWireless);
  this->_getphycmd = new GetPhyCommand;
  this->_getifacecmd = new GetInterfaceCommand(name_);
  this->_setifacecmd = new SetInterfaceCommand(name_);
  this->_newifacecmd = new NewInterfaceCommand(name_);
  this->_delifacecmd = new DelInterfaceCommand(name_);
}

WirelessInterface::WirelessInterface(const WirelessInterfaceConfigData& config_) :
    Interface(config_), WiConfig(config_), _getphycmd(NULL), _getifacecmd(NULL),
    _setifacecmd(NULL), _newifacecmd(NULL), _delifacecmd(NULL)
{
  this->_getphycmd = new GetPhyCommand(this->WiConfig.PhyIndex());
  this->_getifacecmd = new GetInterfaceCommand(this->Config.Name());
  this->_setifacecmd = new SetInterfaceCommand(this->Config.Name());
  this->_newifacecmd = new NewInterfaceCommand(this->Config.Name());
  this->_delifacecmd = new DelInterfaceCommand(this->Config.Name());
}

WirelessInterface::~WirelessInterface()
{
  if (this->_getphycmd)
  {
    delete (this->_getphycmd);
    this->_getphycmd = NULL;
  }
  if (this->_getifacecmd)
  {
    delete (this->_getifacecmd);
    this->_getifacecmd = NULL;
  }
  if (this->_setifacecmd)
  {
    delete (this->_setifacecmd);
    this->_setifacecmd = NULL;
  }
  if (this->_newifacecmd)
  {
    delete (this->_newifacecmd);
    this->_newifacecmd = NULL;
  }
  if (this->_delifacecmd)
  {
    delete (this->_delifacecmd);
    this->_delifacecmd = NULL;
  }
}

unsigned int
WirelessInterface::GetPhyIndex() const
{
  int index = -1;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_getphycmd)
    {
      index = this->_getphycmd->PhyIndex();
    }
    this->_lock.Unlock();
  }
  return (index);
}

std::string
WirelessInterface::GetPhyName() const
{
  std::string name;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_getphycmd)
    {
      name = this->_getphycmd->PhyName();
    }
    this->_lock.Unlock();
  }
  return (name);
}

WirelessInterface::HWMODE
WirelessInterface::GetHwMode() const
{
  WirelessInterface::HWMODE mode = WirelessInterface::HWMODE_ERR;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_getifacecmd)
    {
    }
    this->_lock.Unlock();
  }
  return (mode);
}

bool
WirelessInterface::SetHwMode(const WirelessInterface::HWMODE mode_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    if (this->_refreshed)
    {
      if (this->_refreshed && this->_setlinkcmd)
      {
      }
    }
    this->_lock.Unlock();
  }
  return (status);
}

WirelessInterface::HTMODE
WirelessInterface::GetHtMode() const
{
  WirelessInterface::HTMODE mode = WirelessInterface::HTMODE_ERR;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_getifacecmd)
    {
      switch(this->_getifacecmd->ChannelType())
      {
      case NL80211_CHAN_NO_HT:
        mode = WirelessInterface::HTMODE_NOHT;
        break;
      case NL80211_CHAN_HT20:
        mode = WirelessInterface::HTMODE_HT20;
        break;
      case NL80211_CHAN_HT40MINUS:
        mode = WirelessInterface::HTMODE_HT40MINUS;
        break;
      case NL80211_CHAN_HT40PLUS:
        mode = WirelessInterface::HTMODE_HT40PLUS;
        break;
      default:
        break;
      }
      switch(this->_getifacecmd->ChannelWidth())
      {
      case NL80211_CHAN_WIDTH_20_NOHT:
        mode = WirelessInterface::HTMODE_NOHT;
        break;
      case NL80211_CHAN_WIDTH_20:
        mode = WirelessInterface::HTMODE_VHT20;
        break;
      case NL80211_CHAN_WIDTH_40:
        mode = WirelessInterface::HTMODE_VHT40;
        break;
      case NL80211_CHAN_WIDTH_80:
        mode = WirelessInterface::HTMODE_VHT80;
        break;
      case NL80211_CHAN_WIDTH_80P80:
        mode = WirelessInterface::HTMODE_VHT80PLUS80;
        break;
      case NL80211_CHAN_WIDTH_160:
        mode = WirelessInterface::HTMODE_VHT160;
        break;
      default:
        break;
      }
    }
    this->_lock.Unlock();
  }
  return (mode);
}

bool
WirelessInterface::SetHtMode(const WirelessInterface::HTMODE mode_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    if (this->_refreshed)
    {
      if (this->_refreshed && this->_setifacecmd)
      {
      }
    }
    this->_lock.Unlock();
  }
  return (status);
}

WirelessInterface::OPMODE
WirelessInterface::GetOpMode() const
{
  WirelessInterface::OPMODE mode = WirelessInterface::OPMODE_ERR;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_getifacecmd)
    {
      switch (this->_getifacecmd->ChannelType())
      {
      case NL80211_IFTYPE_STATION:
        mode = WirelessInterface::OPMODE_STA;
        break;
      case NL80211_IFTYPE_AP:
        mode = WirelessInterface::OPMODE_AP;
        break;
      case NL80211_IFTYPE_MONITOR:
        mode = WirelessInterface::OPMODE_MONITOR;
        break;
      case NL80211_IFTYPE_ADHOC:
        mode = WirelessInterface::OPMODE_ADHOC;
        break;
      default:
        break;
      }
    }
    this->_lock.Unlock();
  }
  return (mode);
}

bool
WirelessInterface::SetOpMode(const WirelessInterface::OPMODE mode_)
{
  bool status = false;
  int iftype = -1;
  if (this->_lock.Lock())
  {
    if (this->_refreshed)
    {
      if (this->_refreshed && this->_setifacecmd)
      {
        switch(mode_)
        {
        case WirelessInterface::OPMODE_STA:
          iftype = NL80211_IFTYPE_STATION;
          break;
        case WirelessInterface::OPMODE_AP:
          iftype = NL80211_IFTYPE_AP;
          break;
        case WirelessInterface::OPMODE_MONITOR:
          iftype = NL80211_IFTYPE_MONITOR;
          break;
        case WirelessInterface::OPMODE_ADHOC:
          iftype = NL80211_IFTYPE_ADHOC;
          break;
        default:
          break;
        }
        if (iftype != -1)
        {
          this->_setifacecmd->IfType(iftype);
          status = this->_setifacecmd->Exec();
        }
      }
    }
    this->_lock.Unlock();
  }
  return (status);
}

bool
WirelessInterface::Refresh()
{

  bool status = Interface::Refresh();

  if (status && this->_lock.Lock())
  {
    this->_refreshed = false;
    if (this->_getphycmd && this->_getifacecmd && this->_getifacecmd->Exec())
    {
      this->_getphycmd->PhyIndex(this->_getifacecmd->PhyIndex());
      status = this->_refreshed = this->_getphycmd->Exec();
    }
    this->_lock.Unlock();
  }

  return (status);

}

bool
WirelessInterface::Create()
{

  bool status = false;

  if (this->_lock.Lock())
  {
    if (this->_newifacecmd)
    {
      this->_newifacecmd->PhyIndex(this->WiConfig.PhyIndex());
      if (this->WiConfig.OpMode() == WirelessInterfaceConfigData::ConfigOpModeSTA)
      {
        this->_newifacecmd->IfType(NL80211_IFTYPE_STATION);
      }
      else if (this->WiConfig.OpMode() == WirelessInterfaceConfigData::ConfigOpModeAP)
      {
        this->_newifacecmd->IfType(NL80211_IFTYPE_AP);
      }
      else if (this->WiConfig.OpMode() == WirelessInterfaceConfigData::ConfigOpModeAdhoc)
      {
        this->_newifacecmd->IfType(NL80211_IFTYPE_ADHOC);
      }
      else if (this->WiConfig.OpMode() == WirelessInterfaceConfigData::ConfigOpModeMonitor)
      {
        this->_newifacecmd->IfType(NL80211_IFTYPE_MONITOR);
      }
      else
      {
        this->_newifacecmd->IfType(NL80211_IFTYPE_UNSPECIFIED);
      }
      status = this->_newifacecmd->Exec();
    }
    this->_lock.Unlock();
  }

  return(status);

}

bool
WirelessInterface::Destroy()
{

  bool status = false;

  if (this->_lock.Lock())
  {
    this->_lock.Unlock();
  }

  return(status);

}

void
WirelessInterface::Display(const std::string &prefix_)
{
  Interface::Display(prefix_);
}

}
}
