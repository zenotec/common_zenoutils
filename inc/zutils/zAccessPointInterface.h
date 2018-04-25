/*
 * Copyright (c) 2017 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
 *
 * Copyright (c) 2014-2017 ZenoTec LLC (http://www.zenotec.net)
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

#ifndef __ZACCESSPOINTINTERFACE_H__
#define __ZACCESSPOINTINTERFACE_H__

#include <zutils/zInterface.h>
using namespace zUtils::zInterface;

#include <zutils/zWireless.h>
#include <zutils/ieee80211/Beacon.h>
#include <zutils/ieee80211/Probe.h>

namespace zUtils
{
namespace zWireless
{

// ****************************************************************************
// Class: AccessPointInterface
// ****************************************************************************

class AccessPointInterface : public Interface
{
public:

  AccessPointInterface(const std::string &name_);

  virtual
  ~AccessPointInterface();

  std::string
  GetSsid() const;

  bool
  SetSsid(const std::string& ssid_);

  std::string
  GetBssid() const;

  bool
  SetBssid(const std::string& bssid_);

  virtual bool
  Create();

  bool
  Start(ieee80211::Beacon& beacon_, ieee80211::ProbeResponse& probe_);

  bool
  Stop();

  virtual void
  Display(const std::string &prefix_ = std::string(""));

protected:

private:

  std::string
  _getSsid() const;

  std::string
  _getBssid() const;

};

}
}
#endif /* __ZACCESSPOINTINTERFACE_H__ */
