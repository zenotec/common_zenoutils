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

// libc includes
#include <string.h>

#include <netlink/netlink.h>
#include <netlink/attr.h>

#include <linux/nl80211.h>

// libc++ includes

// libzutils includes
#include <zutils/zLog.h>
#include <zutils/netlink/Attribute.h>
using namespace zUtils;

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_INTERFACE);

namespace netlink
{

std::string
__nlattrstr(const unsigned int id_)
{
  std::string str;
  switch (id_)
  {
  case NL80211_ATTR_WIPHY:
    str = "NL80211_ATTR_WIPHY";
    break;
  case NL80211_ATTR_WIPHY_NAME:
    str = "NL80211_ATTR_WIPHY_NAME";
    break;
  case NL80211_ATTR_IFINDEX:
    str = "NL80211_ATTR_IFINDEX";
    break;
  case NL80211_ATTR_IFNAME:
    str = "NL80211_ATTR_IFNAME";
    break;
  case NL80211_ATTR_IFTYPE:
    str = "NL80211_ATTR_IFTYPE";
    break;
  case NL80211_ATTR_MAC:
    str = "NL80211_ATTR_MAC";
    break;

//    case NL80211_ATTR_KEY_DATA,
    //    str = "NL80211_ATTR_KEY_DATA";
    //    break;
//    case NL80211_ATTR_KEY_IDX,
    //    str = "NL80211_ATTR_KEY_IDX";
    //    break;
//    case NL80211_ATTR_KEY_CIPHER,
    //    str = "NL80211_ATTR_KEY_CIPHER";
    //    break;
//    case NL80211_ATTR_KEY_SEQ,
    //    str = "NL80211_ATTR_KEY_SEQ";
    //    break;
//    case NL80211_ATTR_KEY_DEFAULT,
    //    str = "NL80211_ATTR_KEY_DEFAULT";
    //    break;
  case NL80211_ATTR_BEACON_INTERVAL:
    str = "NL80211_ATTR_BEACON_INTERVAL";
    break;
  case NL80211_ATTR_DTIM_PERIOD:
    str = "NL80211_ATTR_DTIM_PERIOD";
    break;
  case NL80211_ATTR_BEACON_HEAD:
    str = "NL80211_ATTR_BEACON_HEAD";
    break;
  case NL80211_ATTR_BEACON_TAIL:
    str = "NL80211_ATTR_BEACON_TAIL";
    break;
  case NL80211_ATTR_STA_AID:
    str = "NL80211_ATTR_STA_AID";
    break;
//    case NL80211_ATTR_STA_FLAGS,
    //    str = "NL80211_ATTR_STA_FLAGS";
    //    break;
//    case NL80211_ATTR_STA_LISTEN_INTERVAL,
    //    str = "NL80211_ATTR_STA_LISTEN_INTERVAL";
    //    break;
//    case NL80211_ATTR_STA_SUPPORTED_RATES,
    //    str = "NL80211_ATTR_STA_SUPPORTED_RATES";
    //    break;
//    case NL80211_ATTR_STA_VLAN,
    //    str = "NL80211_ATTR_STA_VLAN";
    //    break;
//    case NL80211_ATTR_STA_INFO,
    //    str = "NL80211_ATTR_STA_INFO";
    //    break;
  case NL80211_ATTR_WIPHY_BANDS:
    str = "NL80211_ATTR_WIPHY_BANDS";
    break;
//    case NL80211_ATTR_MNTR_FLAGS,
    //    str = "NL80211_ATTR_MNTR_FLAGS";
    //    break;
//    case NL80211_ATTR_MESH_ID,
    //    str = "NL80211_ATTR_MESH_ID";
    //    break;
//    case NL80211_ATTR_STA_PLINK_ACTION,
    //    str = "NL80211_ATTR_STA_PLINK_ACTION";
    //    break;
//    case NL80211_ATTR_MPATH_NEXT_HOP,
    //    str = "NL80211_ATTR_MPATH_NEXT_HOP";
    //    break;
//    case NL80211_ATTR_MPATH_INFO,
    //    str = "NL80211_ATTR_MPATH_INFO";
    //    break;
//    case NL80211_ATTR_BSS_CTS_PROT,
    //    str = "NL80211_ATTR_BSS_CTS_PROT";
    //    break;
//    case NL80211_ATTR_BSS_SHORT_PREAMBLE,
    //    str = "NL80211_ATTR_BSS_SHORT_PREAMBLE";
    //    break;
//    case NL80211_ATTR_BSS_SHORT_SLOT_TIME,
    //    str = "NL80211_ATTR_BSS_SHORT_SLOT_TIME";
    //    break;
  case NL80211_ATTR_HT_CAPABILITY:
    str = "NL80211_ATTR_HT_CAPABILITY";
    break;
//
  case NL80211_ATTR_SUPPORTED_IFTYPES:
    str = "NL80211_ATTR_SUPPORTED_IFTYPES";
    break;
//    case NL80211_ATTR_REG_ALPHA2,
    //    str = "NL80211_ATTR_REG_ALPHA2";
    //    break;
//    case NL80211_ATTR_REG_RULES,
    //    str = "NL80211_ATTR_REG_RULES";
    //    break;
//    case NL80211_ATTR_MESH_CONFIG,
    //    str = "NL80211_ATTR_MESH_CONFIG";
    //    break;
  case NL80211_ATTR_BSS_BASIC_RATES:
    str = "NL80211_ATTR_BSS_BASIC_RATES";
    break;
  case NL80211_ATTR_WIPHY_TXQ_PARAMS:
    str = "NL80211_ATTR_WIPHY_TXQ_PARAMS";
    break;
  case NL80211_ATTR_WIPHY_FREQ:
    str = "NL80211_ATTR_WIPHY_FREQ";
    break;
  case NL80211_ATTR_WIPHY_CHANNEL_TYPE:
    str = "NL80211_ATTR_WIPHY_CHANNEL_TYPE";
    break;
  case NL80211_ATTR_KEY_DEFAULT_MGMT:
    str = "NL80211_ATTR_KEY_DEFAULT_MGMT";
    break;
  case NL80211_ATTR_MGMT_SUBTYPE:
    str = "NL80211_ATTR_MGMT_SUBTYPE";
    break;
  case NL80211_ATTR_IE:
    str = "NL80211_ATTR_IE";
    break;
  case NL80211_ATTR_MAX_NUM_SCAN_SSIDS:
    str = "NL80211_ATTR_MAX_NUM_SCAN_SSIDS";
    break;
  case NL80211_ATTR_SCAN_FREQUENCIES:
    str = "NL80211_ATTR_SCAN_FREQUENCIES";
    break;
  case NL80211_ATTR_SCAN_SSIDS:
    str = "NL80211_ATTR_SCAN_SSIDS";
    break;
  case NL80211_ATTR_GENERATION:
    str = "NL80211_ATTR_GENERATION";
    break;
  case NL80211_ATTR_BSS:
    str = "NL80211_ATTR_BSS";
    break;
  case NL80211_ATTR_REG_INITIATOR:
    str = "NL80211_ATTR_REG_INITIATOR";
    break;
  case NL80211_ATTR_REG_TYPE:
    str = "NL80211_ATTR_REG_TYPE";
    break;
  case NL80211_ATTR_SUPPORTED_COMMANDS:
    str = "NL80211_ATTR_SUPPORTED_COMMANDS";
    break;
  case NL80211_ATTR_FRAME:
    str = "NL80211_ATTR_FRAME";
    break;
//    case NL80211_ATTR_SSID,
    //    str = "NL80211_ATTR_SSID";
    //    break;
//    case NL80211_ATTR_AUTH_TYPE,
    //    str = "NL80211_ATTR_AUTH_TYPE";
    //    break;
//    case NL80211_ATTR_REASON_CODE,
    //    str = "NL80211_ATTR_REASON_CODE";
    //    break;
//    case NL80211_ATTR_KEY_TYPE,
    //    str = "NL80211_ATTR_KEY_TYPE";
    //    break;
  case NL80211_ATTR_MAX_SCAN_IE_LEN:
    str = "NL80211_ATTR_MAX_SCAN_IE_LEN";
    break;
  case NL80211_ATTR_CIPHER_SUITES:
    str = "NL80211_ATTR_CIPHER_SUITES";
    break;
  case NL80211_ATTR_FREQ_BEFORE:
    str = "NL80211_ATTR_FREQ_BEFORE";
    break;
  case NL80211_ATTR_FREQ_AFTER:
    str = "NL80211_ATTR_FREQ_AFTER";
    break;
  case NL80211_ATTR_FREQ_FIXED:
    str = "NL80211_ATTR_FREQ_FIXED";
    break;
  case NL80211_ATTR_WIPHY_RETRY_SHORT:
    str = "NL80211_ATTR_WIPHY_RETRY_SHORT";
    break;
  case NL80211_ATTR_WIPHY_RETRY_LONG:
    str = "NL80211_ATTR_WIPHY_RETRY_LONG";
    break;
  case NL80211_ATTR_WIPHY_FRAG_THRESHOLD:
    str = "NL80211_ATTR_WIPHY_FRAG_THRESHOLD";
    break;
  case NL80211_ATTR_WIPHY_RTS_THRESHOLD:
    str = "NL80211_ATTR_WIPHY_RTS_THRESHOLD";
    break;
  case NL80211_ATTR_TIMED_OUT:
     str = "NL80211_ATTR_TIMED_OUT";
     break;
  case NL80211_ATTR_USE_MFP:
     str = "NL80211_ATTR_USE_MFP";
     break;
  case  NL80211_ATTR_STA_FLAGS2:
     str = "NL80211_ATTR_STA_FLAGS2";
     break;
  case  NL80211_ATTR_CONTROL_PORT:
     str = "NL80211_ATTR_CONTROL_PORT";
     break;
  case   NL80211_ATTR_TESTDATA:
     str = "NL80211_ATTR_TESTDATA";
     break;
  case   NL80211_ATTR_PRIVACY:
     str = "NL80211_ATTR_PRIVACY";
     break;
  case   NL80211_ATTR_DISCONNECTED_BY_AP:
     str = "NL80211_ATTR_DISCONNECTED_BY_AP";
     break;
  case   NL80211_ATTR_STATUS_CODE:
     str = "NL80211_ATTR_STATUS_CODE";
     break;
  case NL80211_ATTR_CIPHER_SUITES_PAIRWISE:
     str = "NL80211_ATTR_CIPHER_SUITES_PAIRWISE";
     break;
  case NL80211_ATTR_CIPHER_SUITE_GROUP:
     str = "NL80211_ATTR_CIPHER_SUITE_GROUP";
     break;
  case NL80211_ATTR_WPA_VERSIONS:
     str = "NL80211_ATTR_WPA_VERSIONS";
     break;
  case NL80211_ATTR_AKM_SUITES:
     str = "NL80211_ATTR_AKM_SUITES";
     break;
  case NL80211_ATTR_REQ_IE:
     str = "NL80211_ATTR_REQ_IE";
     break;
  case NL80211_ATTR_RESP_IE:
     str = "NL80211_ATTR_RESP_IE";
     break;
  case NL80211_ATTR_PREV_BSSID:
     str = "NL80211_ATTR_PREV_BSSID";
     break;
  case NL80211_ATTR_KEY:
     str = "NL80211_ATTR_KEY";
     break;
  case NL80211_ATTR_KEYS:
     str = "NL80211_ATTR_KEYS";
     break;
  case NL80211_ATTR_PID:
     str = "NL80211_ATTR_PID";
     break;
  case NL80211_ATTR_4ADDR:
     str = "NL80211_ATTR_4ADDR";
     break;
  case NL80211_ATTR_SURVEY_INFO:
     str = "NL80211_ATTR_SURVEY_INFO";
     break;
  case NL80211_ATTR_PMKID:
     str = "NL80211_ATTR_PMKID";
     break;
  case NL80211_ATTR_MAX_NUM_PMKIDS:
     str = "NL80211_ATTR_MAX_NUM_PMKIDS";
     break;
  case NL80211_ATTR_DURATION:
     str = "NL80211_ATTR_DURATION";
     break;
  case NL80211_ATTR_COOKIE:
     str = "NL80211_ATTR_COOKIE";
     break;
  case NL80211_ATTR_WIPHY_COVERAGE_CLASS:
     str = "NL80211_ATTR_WIPHY_COVERAGE_CLASS";
     break;
  case NL80211_ATTR_TX_RATES:
     str = "NL80211_ATTR_TX_RATES";
     break;
  case NL80211_ATTR_FRAME_MATCH:
     str = "NL80211_ATTR_FRAME_MATCH";
     break;
  case  NL80211_ATTR_ACK:
     str = "NL80211_ATTR_ACK";
     break;
  case NL80211_ATTR_PS_STATE:
     str = "NL80211_ATTR_PS_STATE";
     break;
  case  NL80211_ATTR_CQM:
     str = "NL80211_ATTR_CQM";
     break;
  case  NL80211_ATTR_LOCAL_STATE_CHANGE:
     str = "NL80211_ATTR_LOCAL_STATE_CHANGE";
     break;
  case   NL80211_ATTR_AP_ISOLATE:
     str = "NL80211_ATTR_AP_ISOLATE";
     break;
  case   NL80211_ATTR_WIPHY_TX_POWER_SETTING:
     str = "NL80211_ATTR_WIPHY_TX_POWER_SETTING";
     break;
  case  NL80211_ATTR_WIPHY_TX_POWER_LEVEL:
     str = "NL80211_ATTR_WIPHY_TX_POWER_LEVEL";
     break;
  case  NL80211_ATTR_TX_FRAME_TYPES:
     str = "NL80211_ATTR_TX_FRAME_TYPES";
     break;
  case  NL80211_ATTR_RX_FRAME_TYPES:
     str = "NL80211_ATTR_RX_FRAME_TYPES";
     break;
  case  NL80211_ATTR_FRAME_TYPE:
     str = "NL80211_ATTR_FRAME_TYPE";
     break;
  case   NL80211_ATTR_CONTROL_PORT_ETHERTYPE:
     str = "NL80211_ATTR_CONTROL_PORT_ETHERTYPE";
     break;
  case   NL80211_ATTR_CONTROL_PORT_NO_ENCRYPT:
     str = "NL80211_ATTR_CONTROL_PORT_NO_ENCRYPT";
     break;
  case  NL80211_ATTR_SUPPORT_IBSS_RSN:
     str = "NL80211_ATTR_SUPPORT_IBSS_RSN";
     break;
  case  NL80211_ATTR_WIPHY_ANTENNA_TX:
     str = "NL80211_ATTR_WIPHY_ANTENNA_TX";
     break;
  case  NL80211_ATTR_WIPHY_ANTENNA_RX:
     str = "NL80211_ATTR_WIPHY_ANTENNA_RX";
     break;
  case  NL80211_ATTR_MCAST_RATE:
     str = "NL80211_ATTR_MCAST_RATE";
     break;
  case  NL80211_ATTR_OFFCHANNEL_TX_OK:
     str = "NL80211_ATTR_OFFCHANNEL_TX_OK";
     break;
  case   NL80211_ATTR_BSS_HT_OPMODE:
     str = "NL80211_ATTR_BSS_HT_OPMODE";
     break;
  case   NL80211_ATTR_KEY_DEFAULT_TYPES:
     str = "NL80211_ATTR_KEY_DEFAULT_TYPES";
     break;
  case  NL80211_ATTR_MAX_REMAIN_ON_CHANNEL_DURATION:
     str = "NL80211_ATTR_MAX_REMAIN_ON_CHANNEL_DURATION";
     break;
  case  NL80211_ATTR_MESH_SETUP:
     str = "NL80211_ATTR_MESH_SETUP";
     break;
  case  NL80211_ATTR_WIPHY_ANTENNA_AVAIL_TX:
     str = "NL80211_ATTR_WIPHY_ANTENNA_AVAIL_TX";
     break;
  case   NL80211_ATTR_WIPHY_ANTENNA_AVAIL_RX:
     str = "NL80211_ATTR_WIPHY_ANTENNA_AVAIL_RX";
     break;
  case   NL80211_ATTR_SUPPORT_MESH_AUTH:
     str = "NL80211_ATTR_SUPPORT_MESH_AUTH";
     break;
  case NL80211_ATTR_STA_PLINK_STATE:
    str = "NL80211_ATTR_STA_PLINK_STATE";
    break;
  case NL80211_ATTR_WOWLAN_TRIGGERS:
    str = "NL80211_ATTR_WOWLAN_TRIGGERS";
    break;
  case NL80211_ATTR_WOWLAN_TRIGGERS_SUPPORTED:
    str = "NL80211_ATTR_WOWLAN_TRIGGERS_SUPPORTED";
    break;
  case NL80211_ATTR_SCHED_SCAN_INTERVAL:
    str = "NL80211_ATTR_SCHED_SCAN_INTERVAL";
    break;
  case NL80211_ATTR_INTERFACE_COMBINATIONS:
    str = "NL80211_ATTR_INTERFACE_COMBINATIONS";
    break;
  case NL80211_ATTR_SOFTWARE_IFTYPES:
    str = "NL80211_ATTR_SOFTWARE_IFTYPES";
    break;
  case NL80211_ATTR_REKEY_DATA:
    str = "NL80211_ATTR_REKEY_DATA";
    break;
  case NL80211_ATTR_MAX_NUM_SCHED_SCAN_SSIDS:
    str = "NL80211_ATTR_MAX_NUM_SCHED_SCAN_SSIDS";
    break;
  case NL80211_ATTR_MAX_SCHED_SCAN_IE_LEN:
    str = "NL80211_ATTR_MAX_SCHED_SCAN_IE_LEN";
    break;
  case NL80211_ATTR_SCAN_SUPP_RATES:
    str = "NL80211_ATTR_SCAN_SUPP_RATES";
    break;
  case NL80211_ATTR_HIDDEN_SSID:
    str = "NL80211_ATTR_HIDDEN_SSID";
    break;
  case NL80211_ATTR_IE_PROBE_RESP:
    str = "NL80211_ATTR_IE_PROBE_RESP";
    break;
  case NL80211_ATTR_IE_ASSOC_RESP:
    str = "NL80211_ATTR_IE_ASSOC_RESP";
    break;
  case NL80211_ATTR_STA_WME:
    str = "NL80211_ATTR_STA_WME";
    break;
  case NL80211_ATTR_SUPPORT_AP_UAPSD:
    str = "NL80211_ATTR_SUPPORT_AP_UAPSD";
    break;
  case NL80211_ATTR_ROAM_SUPPORT:
    str = "NL80211_ATTR_ROAM_SUPPORT";
    break;
  case NL80211_ATTR_SCHED_SCAN_MATCH:
    str = "NL80211_ATTR_SCHED_SCAN_MATCH";
    break;
  case NL80211_ATTR_MAX_MATCH_SETS:
    str = "NL80211_ATTR_MAX_MATCH_SETS";
    break;
  case NL80211_ATTR_PMKSA_CANDIDATE:
    str = "NL80211_ATTR_PMKSA_CANDIDATE";
    break;
  case NL80211_ATTR_TX_NO_CCK_RATE:
    str = "NL80211_ATTR_TX_NO_CCK_RATE";
    break;
  case NL80211_ATTR_TDLS_ACTION:
    str = "NL80211_ATTR_TDLS_ACTION";
    break;
  case NL80211_ATTR_TDLS_DIALOG_TOKEN:
    str = "NL80211_ATTR_TDLS_DIALOG_TOKEN";
    break;
  case NL80211_ATTR_TDLS_OPERATION:
    str = "NL80211_ATTR_TDLS_OPERATION";
    break;
  case NL80211_ATTR_TDLS_SUPPORT:
    str = "NL80211_ATTR_TDLS_SUPPORT";
    break;
  case NL80211_ATTR_TDLS_EXTERNAL_SETUP:
    str = "NL80211_ATTR_TDLS_EXTERNAL_SETUP";
    break;
  case NL80211_ATTR_DEVICE_AP_SME:
    str = "NL80211_ATTR_DEVICE_AP_SME";
    break;
  case NL80211_ATTR_DONT_WAIT_FOR_ACK:
    str = "NL80211_ATTR_DONT_WAIT_FOR_ACK";
    break;
  case NL80211_ATTR_FEATURE_FLAGS:
    str = "NL80211_ATTR_FEATURE_FLAGS";
    break;
  case NL80211_ATTR_PROBE_RESP_OFFLOAD:
    str = "NL80211_ATTR_PROBE_RESP_OFFLOAD";
    break;
  case NL80211_ATTR_PROBE_RESP:
    str = "NL80211_ATTR_PROBE_RESP";
    break;
  case NL80211_ATTR_DFS_REGION:
    str = "NL80211_ATTR_DFS_REGION";
    break;
  case NL80211_ATTR_DISABLE_HT:
    str = "NL80211_ATTR_DISABLE_HT";
    break;
  case NL80211_ATTR_HT_CAPABILITY_MASK:
    str = "NL80211_ATTR_HT_CAPABILITY_MASK";
    break;
  case NL80211_ATTR_NOACK_MAP:
    str = "NL80211_ATTR_NOACK_MAP";
    break;
  case NL80211_ATTR_INACTIVITY_TIMEOUT:
    str = "NL80211_ATTR_INACTIVITY_TIMEOUT";
    break;
  case NL80211_ATTR_RX_SIGNAL_DBM:
    str = "NL80211_ATTR_RX_SIGNAL_DBM";
    break;
  case NL80211_ATTR_BG_SCAN_PERIOD:
    str = "NL80211_ATTR_BG_SCAN_PERIOD";
    break;
  case NL80211_ATTR_WDEV:
    str = "NL80211_ATTR_WDEV";
    break;
//    case NL80211_ATTR_USER_REG_HINT_TYPE:
    // str = "";
    // break;
//    case NL80211_ATTR_CONN_FAILED_REASON:
    // str = "";
    // break;
//    case NL80211_ATTR_SAE_DATA:
    // str = "";
    // break;
//    case NL80211_ATTR_VHT_CAPABILITY:
    // str = "";
    // break;
//    case NL80211_ATTR_SCAN_FLAGS:
    // str = "";
    // break;
//    case NL80211_ATTR_CHANNEL_WIDTH:
    // str = "";
    // break;
//    case NL80211_ATTR_CENTER_FREQ1:
    // str = "";
    // break;
//    case NL80211_ATTR_CENTER_FREQ2:
    // str = "";
    // break;
//    case NL80211_ATTR_P2P_CTWINDOW:
    // str = "";
    // break;
//    case NL80211_ATTR_P2P_OPPPS:
    // str = "";
    // break;
//    case NL80211_ATTR_LOCAL_MESH_POWER_MODE:
    // str = "";
    // break;
//    case NL80211_ATTR_ACL_POLICY:
    // str = "";
    // break;
//    case NL80211_ATTR_MAC_ADDRS:
    // str = "";
    // break;
//    case NL80211_ATTR_MAC_ACL_MAX:
    // str = "";
    // break;
//    case NL80211_ATTR_RADAR_EVENT:
    // str = "";
    // break;
//    case NL80211_ATTR_EXT_CAPA:
    // str = "";
    // break;
//    case NL80211_ATTR_EXT_CAPA_MASK:
    // str = "";
    // break;
//    case NL80211_ATTR_STA_CAPABILITY:
    // str = "";
    // break;
//    case NL80211_ATTR_STA_EXT_CAPABILITY:
    // str = "";
    // break;
//    case NL80211_ATTR_PROTOCOL_FEATURES:
    // str = "";
    // break;
//    case NL80211_ATTR_SPLIT_WIPHY_DUMP:
    // str = "";
    // break;
//    case NL80211_ATTR_DISABLE_VHT:
    // str = "";
    // break;
//    case NL80211_ATTR_VHT_CAPABILITY_MASK:
    // str = "";
    // break;
//    case NL80211_ATTR_MDID:
//    case NL80211_ATTR_IE_RIC:
//
//    case NL80211_ATTR_CRIT_PROT_ID:
//    case NL80211_ATTR_MAX_CRIT_PROT_DURATION:
//
//    case NL80211_ATTR_PEER_AID:
//
//    case NL80211_ATTR_COALESCE_RULE:
//
//    case NL80211_ATTR_CH_SWITCH_COUNT:
//    case NL80211_ATTR_CH_SWITCH_BLOCK_TX:
//    case NL80211_ATTR_CSA_IES:
//    case NL80211_ATTR_CSA_C_OFF_BEACON:
//    case NL80211_ATTR_CSA_C_OFF_PRESP:
//
//    case NL80211_ATTR_RXMGMT_FLAGS:
//
//    case NL80211_ATTR_STA_SUPPORTED_CHANNELS:
//
//    case NL80211_ATTR_STA_SUPPORTED_OPER_CLASSES:
//
//    case NL80211_ATTR_HANDLE_DFS:
//
//    case NL80211_ATTR_SUPPORT_5_MHZ:
//    case NL80211_ATTR_SUPPORT_10_MHZ:
//
//    case NL80211_ATTR_OPMODE_NOTIF:
//
//    case NL80211_ATTR_VENDOR_ID:
//    case NL80211_ATTR_VENDOR_SUBCMD:
//    case NL80211_ATTR_VENDOR_DATA:
//    case NL80211_ATTR_VENDOR_EVENTS:
//
//    case NL80211_ATTR_QOS_MAP:
//
//    case NL80211_ATTR_MAC_HINT:
//    case NL80211_ATTR_WIPHY_FREQ_HINT:
//
//    case NL80211_ATTR_MAX_AP_ASSOC_STA:
//
//    case NL80211_ATTR_TDLS_PEER_CAPABILITY:
//
//    case NL80211_ATTR_SOCKET_OWNER:
//
//    case NL80211_ATTR_CSA_C_OFFSETS_TX:
//    case NL80211_ATTR_MAX_CSA_COUNTERS:
//
//    case NL80211_ATTR_TDLS_INITIATOR:
//
//    case NL80211_ATTR_USE_RRM:
//
//    case NL80211_ATTR_WIPHY_DYN_ACK:
//
//    case NL80211_ATTR_TSID:
//    case NL80211_ATTR_USER_PRIO:
//    case NL80211_ATTR_ADMITTED_TIME:
//
//    case NL80211_ATTR_SMPS_MODE:
//
//    case NL80211_ATTR_OPER_CLASS:
//
//    case NL80211_ATTR_MAC_MASK:
//
//    case NL80211_ATTR_WIPHY_SELF_MANAGED_REG:
//
//    case NL80211_ATTR_EXT_FEATURES:
//
//    case NL80211_ATTR_SURVEY_RADIO_STATS:
//
//    case NL80211_ATTR_NETNS_FD:
//
//    case NL80211_ATTR_SCHED_SCAN_DELAY:
//
  case NL80211_ATTR_REG_INDOOR:
    str = "NL80211_ATTR_REG_INDOOR";
    break;
  case NL80211_ATTR_MAX_NUM_SCHED_SCAN_PLANS:
    str = "NL80211_ATTR_MAX_NUM_SCHED_SCAN_PLANS";
    break;
  case NL80211_ATTR_MAX_SCAN_PLAN_INTERVAL:
    str = "NL80211_ATTR_MAX_SCAN_PLAN_INTERVAL";
    break;
  case NL80211_ATTR_MAX_SCAN_PLAN_ITERATIONS:
    str = "NL80211_ATTR_MAX_SCAN_PLAN_ITERATIONS";
    break;
  case NL80211_ATTR_SCHED_SCAN_PLANS:
    str = "NL80211_ATTR_SCHED_SCAN_PLANS";
    break;
  default:
    str = zToStr(id_);
    break;
  }
  return (str);
}

void
__dump_hex(const char* prefix_, const uint8_t* addr_, size_t len_, bool verbose_)
{
  const char* nl = "";
  unsigned long pad = ((unsigned long)addr_ & 0x07);

  if (!len_)
    return;

  if (!verbose_)
    len_ = std::min(int(len_), 16);

  for (int cnt = 0, i = -pad; i < len_; cnt++, i++)
  {
    if (!(cnt % 8))
      printf("%s%s%p: ", nl, prefix_, &addr_[i]);
    if (i < 0)
      printf("-- ");
    else
      printf("%02x ", addr_[i]);
    nl = "\n";
  }
  printf("\n");
}

//*****************************************************************************
// Class: Attribute
//*****************************************************************************

AttributeValue::AttributeValue(const uint32_t id_) :
    Attribute(id_)
{
}

AttributeValue::AttributeValue(const Attribute* other_) :
    Attribute(0)
{
  this->operator =(other_);
}

AttributeValue::~AttributeValue()
{
}

AttributeValue&
AttributeValue::operator =(const Attribute* other_)
{
  if (other_)
  {
    this->SetId(other_->GetId());
    if (other_->IsNested())
    {
      AttributeNested* val = (AttributeNested*) other_;
      // TODO:
      std::cerr << "AttributeValue::operator=(): Not implemented" << std::endl;
    }
    else
    {
      AttributeValue* val = (AttributeValue*) other_;
      this->Set(val->GetData(), val->GetLength());
    }
  }
  return (*this);
}

bool
AttributeValue::Assemble(struct nl_msg* msg_)
{
  bool status = true;
  if (msg_)
  {
    status = (nla_put(msg_, this->GetId(), this->_data.size(), this->_data.data()) == 0);
  }
  return (status);
}

bool
AttributeValue::Disassemble(struct nlattr* attr_, size_t len)
{
  bool status = false;
  if (attr_ && this->SetId(nla_type(attr_)))
  {
    status = this->Set((const uint8_t*)nla_data(attr_), nla_len(attr_));
  }
  return (status);
}

const uint8_t*
AttributeValue::GetData() const
{
  return (this->_data.data());
}
size_t
AttributeValue::GetLength() const
{
  return (this->_data.size());
}

bool
AttributeValue::Get(std::string& str_) const
{
  bool status = false;
  if (this->IsValid() && this->GetData() && this->GetLength())
  {
    str_ = std::string((char*)this->GetData());
  }
  return (status);
}

bool
AttributeValue::Set(const std::string& str_)
{
  bool status = false;
  size_t len = strlen(str_.c_str());
  this->_data.resize(len + 1); // add null terminator
  if (strncpy((char*)this->GetData(), str_.c_str(), this->GetLength()) == (char*)this->GetData())
  {
    this->SetValid();
    status = true;
  }
  return (status);
}

bool
AttributeValue::Get(uint8_t* p_, size_t& len_) const
{
  bool status = false;
  if (this->IsValid())
  {
    len_ = std::min(this->GetLength(), len_);
    status = (memcpy(p_, this->_data.data(), len_) == p_);
  }
  return (status);
}

bool
AttributeValue::Set(const uint8_t* p_, const size_t len_)
{
  bool status = false;
  this->_data.resize(len_);
  if (memcpy(this->_data.data(), p_, len_) == this->_data.data())
  {
    this->SetValid();
    status = true;
  }
  return (status);
}

void
AttributeValue::Display(const std::string& prefix_) const
{
  std::cout << prefix_ << "Value[" << __nlattrstr(this->GetId()) << "]: " << this->GetLength() << std::endl;
  __dump_hex((prefix_ + "\t").c_str(), this->GetData(), this->GetLength(), false);
}

//*****************************************************************************
// Class: AttributeTable
//*****************************************************************************

AttributeTable::AttributeTable()
{
}

AttributeTable::~AttributeTable()
{
}

bool
AttributeTable::Assemble(struct nl_msg* msg_)
{
  bool status = true;
  FOREACH(auto& attr, this->_attrs)
  {
    status &= attr.second->Assemble(msg_);
  }
  return (status);
}

bool
AttributeTable::Disassemble(struct nlattr* attr_, size_t len_)
{

  bool status = true;
  struct nlattr* pos = NULL;
  int rem = 0;

  // Start fresh
  this->_attrs.clear();

  for (pos = attr_, rem = len_; nla_ok(pos, rem); pos = nla_next(pos, &rem))
  {
    SHARED_PTR(Attribute) a;
    if (nla_is_nested(pos))
    {
      a = STATIC_CAST(Attribute)(SHARED_PTR(AttributeNested)(new AttributeNested(nla_type(pos))));
    }
    else
    {
      a = STATIC_CAST(Attribute)(SHARED_PTR(AttributeValue)(new AttributeValue(nla_type(pos))));
    }
    status &= a->Disassemble(pos);
    this->_attrs[nla_type(pos)] = a;
  }

  return (status);
}

size_t
AttributeTable::GetLength() const
{
  size_t len = 0;
  FOREACH (auto& attr, this->_attrs)
  {
    len += sizeof(struct nlattr) + attr.second->GetLength();
  }
  return (len);
}

bool
AttributeTable::Get(Attribute* attr_) const
{
  bool status = false;
  if (attr_ && this->_attrs.count(attr_->GetId()))
  {
    if (attr_->IsNested())
    {
      *(AttributeNested*)attr_ = this->_attrs.at(attr_->GetId()).get();
    }
    else
    {
      *(AttributeValue*)attr_ = this->_attrs.at(attr_->GetId()).get();
    }
    status = true;
  }
  return (status);
}

bool
AttributeTable::Put(Attribute* attr_)
{
  bool status = false;
  if (attr_ && attr_->IsValid())
  {
    SHARED_PTR(Attribute) a;
    if (attr_->IsNested())
    {
      a = STATIC_CAST(Attribute)(SHARED_PTR(AttributeNested)(new AttributeNested(attr_)));
    }
    else
    {
      a = STATIC_CAST(Attribute)(SHARED_PTR(AttributeValue)(new AttributeValue(attr_)));
    }
    this->_attrs[attr_->GetId()] = a;
  }
  return (true);
}

void
AttributeTable::Display(const std::string& prefix_) const
{
  std::cout << prefix_ << "Table: " << this->GetLength() << std::endl;
  FOREACH (auto& attr, this->_attrs)
  {
    attr.second->Display(prefix_ + std::string("\t"));
  }
  return;
}

//*****************************************************************************
// Class: AttributeNested
//*****************************************************************************

AttributeNested::AttributeNested(const uint32_t id_) :
    Attribute(id_, true)
{
}

AttributeNested::AttributeNested(const Attribute* other_) :
    Attribute(0)
{
  this->operator =(other_);
}

AttributeNested::~AttributeNested()
{
}

AttributeNested&
AttributeNested::operator =(const Attribute* other_)
{
  if (other_ && other_->IsValid())
  {
    this->SetValid();
    this->SetId(other_->GetId());
    if (other_->IsNested())
    {
      AttributeNested* val = (AttributeNested*) other_;
      this->_attrs = val->_attrs;
    }
    else
    {
      AttributeValue* val = (AttributeValue*) other_;
      this->_attrs.Disassemble((struct nlattr*)val->GetData(), val->GetLength());
    }
  }
  return (*this);
}

bool
AttributeNested::Assemble(struct nl_msg* msg_)
{
  bool status = false;
  struct nlattr* start = nla_nest_start(msg_, (this->GetId() | NLA_F_NESTED));
  if (msg_ && start)
  {
    status = this->_attrs.Assemble(msg_);
    nla_nest_end(msg_, start);
  }
  else
  {
    nla_nest_cancel(msg_, start);
  }
  return (status);
}

bool
AttributeNested::Disassemble(struct nlattr* attr_, size_t len_)
{
  bool status = this->_attrs.Disassemble((struct nlattr*)nla_data(attr_), nla_len(attr_));
  if (status)
  {
    this->SetValid();
  }
  return (status);
}

size_t
AttributeNested::GetLength() const
{
  return (this->_attrs.GetLength());
}

bool
AttributeNested::Get(Attribute* attr_) const
{
  return (this->_attrs.Get(attr_));
}

bool
AttributeNested::Put(Attribute* attr_)
{
  return (this->_attrs.Put(attr_));
}

void
AttributeNested::Display(const std::string& prefix_) const
{
  std::cout << prefix_ << "Nested[" << this->GetId() << "]: " << this->GetLength() << std::endl;
  this->_attrs.Display(prefix_ + std::string("\t"));
}

}
