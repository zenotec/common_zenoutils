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

#ifndef __ZBASICSERVICESET_H__
#define __ZBASICSERVICESET_H__

#include <string>

#include <zutils/zAccessPointInterface.h>

namespace zUtils
{
namespace zWireless
{

// ****************************************************************************
// Class: BasicServiceSet
// ****************************************************************************

class BasicServiceSet : public AccessPointInterface
{
public:

  BasicServiceSet(const std::string& ifname_, const std::string &ssid_);

  virtual
  ~BasicServiceSet();

  std::string
  GetSsid();

  bool
  SetSsid(const std::string& ssid_);

  std::string
  GetBssid();

  bool
  SetBssid(const std::string& bssid_);

  bool
  AddStation(const std::string& addr_);

  bool
  DelStation(const std::string& addr_);

  virtual bool
  Commit();

  virtual bool
  Create();

  virtual bool
  Destroy();

  virtual void
  Display(const std::string &prefix_ = std::string(""));

protected:

private:

  uint8_t _beaconbuf[512];
  size_t _beaconlen;

};

}
}
#endif /* __ZBASICSERVICESET_H__ */
