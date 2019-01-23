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
} __attribute__ ((packed));

struct rsn_oui_format
{
  uint8_t oui_b1;
  uint8_t oui_b2;
  uint8_t oui_b3;
} __attribute__ ((packed));

struct rsn_suite
{
  struct rsn_oui_format cipher_oui;
  uint8_t cipher_suite_type;
} __attribute__ ((packed));

struct rsn_suite_count
{
  uint16_t suite_count;
} __attribute__ ((packed));

/* RSN Capabilities flags, described in 802.11-2016 Sec. 9.4.2.25.4
 * B0 - Preauth
 * B1 - No Pairwise
 * B2,3 - PTKSA Replay Counter
 * B4,5 - GTKSA Replay Counter
 * B6 - Management Frame Protection Required
 * B7 - Management Frame Protection Capable
 * B8 - Joint Multi-Band RSNA
 * B9 - PeerKey Enabled
 * B10 - SPP A-MSDU Capable
 * B11 - SPP A-MSDU Required
 * B12 - PBAC
 * B13 - Extended Key ID for Individually Addressed Frames
 * B14,15 - Reserved
 */
struct rsn_capabilities
{
  uint16_t rsn_cap;
} __attribute__ ((packed));

struct vht_caps
{
  // TODO
  uint8_t tmp;
} __attribute__ ((packed));

enum RSN_PROTOCOL
{
    VER_UNKNOWN = 0,
    VER_80211_2016 = 1,
    VER_LAST
};

// From IEEE 802.11-2016 Section 9.4.2.25.2
enum IEEE_80211_CIPHER_SUITES
{
    USE_GROUP_CIPHER = 0,
    WEP_40 = 1,
    TKIP = 2,
    CIPHER_RESERVED = 3,
    CCMP_128 = 4,
    WEP_104 = 5,
    BIP_CMAC_128 = 6,
    GROUP_TRAFFIC_NOT_ALLOWED = 7,
    GCMP_128 = 8,
    GCMP_256 = 9,
    CCMP_256 = 10,
    BIP_GMAC_128 = 11,
    BIP_GMAC_256 = 12,
    BIP_CMAC_256 = 13,
    CIPHER_UNKNOWN = 255,
    CIPHER_SUITE_LAST
};

// Taken from IEEE 802.11-2016 Sec 9.4.2.25.3
enum AKM_CIPHER_SUITES
{
    AKM_RESERVED = 0,
    IEEE_STD_PMKSA_CACHING = 1,
    PSK = 2,
    FT_AUTH_8021X_SHA256 = 3,
    FT_AUTH_PSK_SHA256 = 4,
    IEEE_STD_PMKSA_CACHING_SHA256 = 5,
    PSK_SHA256 = 6,
    TDLS_SHA256 = 7,
    SAE_SHA256_PMSKA_CACHING = 8,
    FT_AUTH_SAE = 9,
    AP_PEERKEY_AUTH = 10,
    SUITE_B_EAP_SHA256 = 11,
    SUITE_B_EAP_SHA384 = 12,
    FT_AUTH_SHA384 = 13,
    AKM_CIPHER_UNKNOWN = 255,
    AKM_CIPHER_LAST
};

enum IEEE_8021X_PROTO_VER
{
  VER_2001 = 0x01,
  VER_2004 = 0x02,
};

enum IEEE_8021X_PROTO_TYPE
{
  EAP_PACKET = 0x00,
  EAP_START = 0x01,
  EAP_LOGOFF = 0x02,
  EAP_KEY = 0x03,
  EAP_ALERT = 0x04,
};

enum IEEE_8021X_KEY_DESC
{
  RC4_KEY_DESCRIPTOR = 1,
  IEEE_80211_KEY_DESCRIPTOR = 2,
};

enum IEEE_80211_KEY_INFO
{
  HMAC_MD5 = 0x0001,
  HMAC_SHA_1 = 0x0002,
  AES_CMAC = 0x0003,
  PTK_DERIVATION = 0x0008,
  INSTALL = 0x0040,
  KEY_ACK = 0x0080,
  KEY_MIC = 0x0100,
  SECURE = 0x0200,
  ERROR = 0x0400,
  REQUEST = 0x0800,
  ENCRYPTED = 0x1000,
  SMK_MESSAGE = 0x2000,
};

enum INTEGRITY_ALGO
{
  HMAC_SHA_1_128,
  AES_128_CMAC,
  HMAC_SHA_256,
  HMAC_SHA_385,
  ALGO_UNKNOWN,
};

enum KDE_DATA_TYPES
{
  RESERVED = 0,
  KDE_GTK= 1,
  KDE_RES = 2,
  KDE_MAC = 3,
  KDE_PMKID = 4,
  KDE_SMK = 5,
  KDE_NOUNCE = 6,
  KDE_LIFETIME = 7,
  KDE_ERROR = 8,
  KDE_IGTK = 9,
  KDE_KEY_ID = 10,
  KDE_GTK_MULTI = 11,
  KDE_KEY_ID_MULTI = 12,
  KDE_LAST
};

enum KDE_SMK_ERRORS
{
  ERR_STA_NR = 1,
  ERR_STA_NRSN = 2,
  ERR_CPHR_NS = 3,
  ERR_NO_STSL = 4,
  ERR_LAST
};

#endif /* __IEEE80211_H__ */
