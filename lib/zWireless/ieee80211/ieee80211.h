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

#ifndef __IEEE80211_H__
#define __IEEE80211_H__

#ifndef ETH_ALEN
#define ETH_ALEN    6
#endif

struct ieee80211_tag
{
  uint8_t tag;
  uint8_t len;
  uint8_t val[0];
} __attribute__ ((packed));

struct ieee80211_beacon
{
  uint64_t timestamp;
  uint16_t interval;
  uint16_t capabilities;
  ieee80211_tag tags;
} __attribute__ ((packed));

struct ieee80211_probereq
{
  ieee80211_tag tags;
} __attribute__ ((packed));

struct ieee80211_proberesp
{
  uint64_t timestamp;
  uint16_t interval;
  uint16_t capabilities;
  ieee80211_tag tags;
} __attribute__ ((packed));

struct ieee80211_assreq
{
  uint16_t capabilities;
  uint16_t interval;
  ieee80211_tag tags;
} __attribute__ ((packed));

struct ieee80211_auth
{
  uint16_t algorithm;
  uint16_t seqNumber;
  uint16_t statusCode;
  ieee80211_tag tags;
} __attribute__ ((packed));

struct ieee80211_assresp
{
  uint16_t capabilities;
  uint16_t status;
  uint16_t aid;
  ieee80211_tag tags;
} __attribute__ ((packed));

struct ieee80211_genhdr
{
  uint8_t addr1[ETH_ALEN];
  uint8_t addr2[ETH_ALEN];
  uint8_t addr3[ETH_ALEN];
  uint16_t seqcntl;
  uint8_t addr4[ETH_ALEN];
  uint16_t qoscntl;
  uint32_t htcntl;
} __attribute__ ((packed));

struct ieee80211_mgmthdr
{
  uint8_t ra[ETH_ALEN];
  uint8_t ta[ETH_ALEN];
  uint8_t bssid[ETH_ALEN];
  uint16_t seqcntl;
  union
  {
    ieee80211_beacon beacon;
    ieee80211_probereq probereq;
    ieee80211_proberesp proberesp;
    ieee80211_assreq assreq;
    ieee80211_assresp assresp;
  } u;
}__attribute__ ((packed));

struct ieee80211_cntlhdr
{
  uint8_t ra[ETH_ALEN];
  uint8_t ta[ETH_ALEN];
}__attribute__ ((packed));

struct ieee80211_data4addr
{
  uint8_t addr4[ETH_ALEN];
}__attribute__ ((packed));

struct ieee80211_qosdata
{
  uint16_t qoscntl;
}__attribute__ ((packed));

struct ieee80211_datahdr
{
  uint8_t addr1[ETH_ALEN];
  uint8_t addr2[ETH_ALEN];
  uint8_t addr3[ETH_ALEN];
  uint16_t seqcntl;
  union
  {
    ieee80211_data4addr data4addr;
    ieee80211_qosdata qosdata;
  } u;
}__attribute__ ((packed));

struct ieee80211_hdr
{
  uint16_t fc;
  uint16_t duration;
  union
  {
    ieee80211_genhdr gen;
    ieee80211_mgmthdr mgmt;
    ieee80211_cntlhdr cntl;
    ieee80211_datahdr data;
  } u;
}__attribute__ ((packed));

#endif /* __IEEE80211_H__ */
