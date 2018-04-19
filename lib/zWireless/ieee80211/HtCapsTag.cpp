/*
 * Copyright (c) 2017 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
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

#include <zutils/zLog.h>
using namespace zUtils;
#include <zutils/ieee80211/HtCapsTag.h>

ZLOG_MODULE_INIT(zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: HtCapsTag
//*****************************************************************************

void
HtCapsTag::Display() const
{
  Tag::Display();
  struct ht_caps caps = this->operator ()();
  printf("\tht_cap_info: \t0x%04x\n", caps.ht_cap_info);
  printf("\tampdu_parms:  \t0x%02x\n", caps.ampdu_parms);
  printf("\tht_ext_cap: \t0x%04x\n", caps.ht_ext_cap);
  printf("\ttrans_beam_cap: \t0x%08x\n", caps.trans_beam_cap);
  printf("\tasel_cap: \t0x%02x\n", caps.asel_cap);
}


}
}
}
