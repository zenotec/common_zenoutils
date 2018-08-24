/*
 * Copyright (c) 2018 Cable Television Laboratories, Inc. ("CableLabs")
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

#ifndef __IEEE8023_H__
#define __IEEE8023_H__

#include <linux/if_ether.h>

struct ieee8023_addr
{
  uint8_t addr[ETH_ALEN];
};

struct ether_hdr
{
  uint16_t length;
  uint8_t data[0];
} __attribute__ ((packed));

struct ether2_hdr
{
  uint16_t proto;
  uint8_t data[0];
} __attribute__ ((packed));

struct ieee8021q_hdr
{
  uint16_t tpid;
  uint16_t tci;
  uint16_t proto;
  uint8_t data[0];
} __attribute__ ((packed));

struct ieee8021ad_hdr
{
  uint16_t tpid1;
  uint16_t tci1;
  uint16_t tpid2;
  uint16_t tci2;
  uint16_t proto;
  uint8_t data[0];
} __attribute__ ((packed));

struct ieee8022_llc_hdr
{
  uint16_t length;
  uint8_t dst_sap;
  uint8_t src_sap;
  uint8_t cntl;
  uint8_t oui[3];
  uint16_t proto;
  uint8_t data[0];
} __attribute__ ((packed));

struct ieee8022_snap_hdr
{
  uint8_t oui[3];
  uint16_t proto;
  uint8_t data[0];
} __attribute__ ((packed));

struct ieee8023_hdr
{
  struct ieee8023_addr dst;
  struct ieee8023_addr src;
  union
  {
    uint16_t type;
    struct ether_hdr ether;
    struct ether2_hdr ether2;
    struct ieee8021q_hdr vlan;
    struct ieee8021ad_hdr vlan2;
    struct ieee8022_llc_hdr llc;
  } u;
} __attribute__ ((packed));

#endif /* __IEEE80211_H__ */
