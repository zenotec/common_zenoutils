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

#ifndef __ZWIRELESSINTERFACE_H__
#define __ZWIRELESSINTERFACE_H__

namespace zUtils
{
namespace zInterface
{

// ****************************************************************************
// Class: WirelessInterface
// ****************************************************************************

class WirelessInterface : public Interface
{
public:

  WirelessInterface(const std::string &name_);

  WirelessInterface(const InterfaceConfigData& config_);

  virtual
  ~WirelessInterface();

  virtual bool
  Refresh();

  virtual void
  Display(const std::string &prefix_);

  std::string
  IwName();

  std::string
  PhyName();

  bool
  IsAssociated();

  bool
  Associate(const std::string &essid_, const unsigned int channel_);

  bool
  Disassociate();

  std::string
  ApAddress();

  std::string
  Essid();

  int
  Channel();

  bool
  Channel(const int channel_);

  int
  LinkQuality();

  int
  SignalLevel();

  int
  NoiseLevel();

  int
  BitRate();

protected:

  int _link_quality;
  int _signal_level;
  int _noise_level;
  int _bit_rate;

  virtual int
  _get_link_quality();

  virtual int
  _get_signal_level();

  virtual int
  _get_noise_level();

  virtual int
  _get_bit_rate();

private:

  zSem::Mutex _lock;
  std::string _iw_name;
  std::string _phy_name;
  bool _associated;
  std::string _ap_addr;
  std::string _essid;

};

}
}
#endif /* __ZWIRELESSINTERFACE_H__ */
