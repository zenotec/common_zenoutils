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

#include <linux/if_ether.h>

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

struct ieee80211_rassreq
{
  uint16_t capabilities;
  uint16_t interval;
  uint8_t currentApMac[ETH_ALEN];
  ieee80211_tag tags;
} __attribute__ ((packed));

struct ieee80211_actionreq
{
	uint8_t _categoryCode;
	uint8_t _actionCode;
	uint8_t _dialogToken;
	uint16_t _blockAckparms;
	uint16_t _blockAckTimeout;
	uint16_t _blockStartingSequence;
} __attribute__ ((packed));

struct ieee80211_actionresp
{
	uint8_t _categoryCode;
	uint8_t _actionCode;
	uint8_t _dialogToken;
	uint16_t _status;
	uint16_t _blockAckparms;
	uint16_t _blockAckTimeout;
} __attribute__ ((packed));

struct ieee80211_disass
{
  uint16_t reasonCode;
  ieee80211_tag tags;
} __attribute__ ((packed));

struct ieee80211_deauth
{
  uint16_t reasonCode;
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

struct ieee80211_rassresp
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
    ieee80211_disass disass;
    ieee80211_deauth deauth;
  } u;
} __attribute__ ((packed));

struct ieee80211_cntlhdr
{
  uint8_t ra[ETH_ALEN];
  uint8_t ta[ETH_ALEN];
} __attribute__ ((packed));

struct ieee80211_data4addr
{
  uint8_t addr4[ETH_ALEN];
} __attribute__ ((packed));

struct ieee80211_qosdata
{
  uint16_t qoscntl;
} __attribute__ ((packed));

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
} __attribute__ ((packed));

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
} __attribute__ ((packed));


//    The first 4 bits indicate which byte, the second which bit in the byte
enum EXTCAPSBIT
{
  EXCAP_20_40_BSS_COEXISTENCE_MANAGEMENT_SUPPORT = 0x00,
  EXCAP_ON_DEMAND_BEACON = 0x01,
  EXCAP_EXTENDED_CHANNEL_SWITCHING = 0x02,
  EXCAP_WAVE_INDICATION = 0x03,
  EXCAP_PSMP_CAPABILITY = 0x04,
  EXCAP_S_PSMP_SUPPORT = 0x06,
  EXCAP_EVENT = 0x07,
  EXCAP_DIAGNOSTICS = 0x10,
  EXCAP_MULTICAST_DIAGNOSTICS = 0x11,
  EXCAP_LOCATION_TRACKING = 0x12,
  EXCAP_FMS = 0x13,
  EXCAP_PROXY_ARP_SERVICE = 0x14,
  EXCAP_COLLOCATED_INTERFERENCE_REPORTING = 0x15,
  EXCAP_CIVIC_LOCATION = 0x16,
  EXCAP_GEOSPATIAL_LOCATION = 0x17,
  EXCAP_TFS = 0x20,
  EXCAP_WNM_SLEEP_MODE = 0x21,
  EXCAP_TIM_BROADCAST = 0x22,
  EXCAP_BSS_TRANSITION = 0x23,
  EXCAP_QOS_TRAFFIC_CAPABILITY = 0x24,
  EXCAP_AC_STATION_COUNT = 0x25,
  EXCAP_MULTIPLE_BSSID = 0x26,
  EXCAP_TIMING_MEASUREMENT = 0x27,
  EXCAP_CHANNEL_USAGE = 0x30,
  EXCAP_SSID_LIST = 0x31,
  EXCAP_DMS = 0x32,
  EXCAP_UTC_TSF_OFFSET = 0x33,
  EXCAP_PEER_U_APSD_BUFFER_STA_SUPPORT = 0x34,
  EXCAP_TDLS_PEER_PSM_SUPPORT = 0x35,
  EXCAP_TDLS_CHANNEL_SWITCHING = 0x36,
  EXCAP_INTERWORKING = 0x37,
  EXCAP_QOS_MAP = 0x40,
  EXCAP_EBR = 0x41,
  EXCAP_SSPN_INTERFACE = 0x42,
  EXCAP_MSGCF_CAPABILITY = 0x44,
  EXCAP_TDLS_SUPPORT = 0x45,
  EXCAP_TDLS_PROHIBITED = 0x46,
  EXCAP_TDLS_CHANNEL_SWITCHING_PROHIBITED = 0x47,
  EXCAP_REJECT_UNADMITTED_FRAME = 0x50,
  EXCAP_IDENTIFIER_LOCATION = 0x04,
  EXCAP_U_APSD_COEXISTENCE = 0x05,
  EXCAP_WNM_NOTIFICATION = 0x06,
  EXCAP_QAB_CAPABILITY = 0x07,
  EXCAP_UTF_8_SSID = 0x60,
  EXCAP_TDLS_WIDER_BANDWIDTH = 0x75,
  EXCAP_OPERATING_MODE_NOTIFICATION = 0x76,
  EXCAP_MAX_NUMBER_OF_MSDUS_IN_A_MSDU = 0x77
};

struct ext_caps_tag
{
  uint8_t caps[8];
};

struct tx_mcs
{
  uint8_t tx_bits;
  uint16_t reserved1;
  uint8_t reserved2;
} __attribute__ ((packed));

struct mcs_set
{
  std::array<uint8_t,10> rx_mcs_bitmask;
  uint16_t rx_highest_rate;
  tx_mcs tx_mcs_fields;
} __attribute__ ((packed));

struct ht_info
{
  uint8_t ht_primary_channel;
  uint8_t ht_subset_1;
  uint16_t ht_subset_2;
  uint16_t ht_subset_3;
  struct mcs_set ht_rx_mcs;
} __attribute__ ((packed));

struct ht_caps
{
  uint16_t ht_cap_info;
  uint8_t ampdu_parms;
  mcs_set supported_mcs_set;
  uint16_t ht_ext_cap;
  uint32_t trans_beam_cap;
  uint8_t asel_cap;
} __attribute__ ((packed));

struct rsn_element
{
  uint16_t version;
};

struct vht_caps
{
  // TODO
  uint8_t tmp;
} __attribute__ ((packed));

#endif /* __IEEE80211_H__ */
