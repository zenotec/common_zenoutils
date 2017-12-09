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

#include <zutils/zWirelessInterface.h>

namespace zUtils
{
namespace zInterface
{

// ****************************************************************************
// Class: AccessPointInterface
// ****************************************************************************

class AccessPointInterface : public WirelessInterface
{
public:

  AccessPointInterface(const std::string &name_);

  AccessPointInterface(const zInterface::ConfigData& config_);

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
  Commit();

  virtual bool
  Create();

  virtual bool
  Destroy();

  virtual void
  Display(const std::string &prefix_ = std::string(""));

protected:

  virtual bool
  is_modified() const;

  virtual void
  set_modified();

  virtual void
  clr_modified();

private:

  bool _modified;
  uint8_t _beaconbuf[512];

};

}
}
#endif /* __ZACCESSPOINTINTERFACE_H__ */
