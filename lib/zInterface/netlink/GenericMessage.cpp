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

// libc includes
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <linux/nl80211.h>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
#include <zutils/netlink/Attribute.h>
#include <zutils/netlink/Message.h>
#include <zutils/netlink/GenericMessage.h>
using namespace zUtils;

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_INTERFACE);

namespace netlink
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

static const std::string
_attr2str(const int attr_)
{
  std::string str = "[" + zLog::IntStr(attr_) + "] ";
  switch(attr_)
  {
  case NL80211_ATTR_UNSPEC:
    str += "NL80211_ATTR_UNSPEC";
    break;
  case NL80211_ATTR_WIPHY:
    str += "NL80211_ATTR_WIPHY";
    break;
  case NL80211_ATTR_WIPHY_NAME:
    str += "NL80211_ATTR_WIPHY_NAME";
    break;
  case NL80211_ATTR_IFINDEX:
    str += "NL80211_ATTR_IFINDEX";
    break;
  case NL80211_ATTR_IFNAME:
    str += "NL80211_ATTR_IFNAME";
    break;
  case NL80211_ATTR_IFTYPE:
    str += "NL80211_ATTR_IFTYPE";
    break;
  case NL80211_ATTR_MAC:
    str += "NL80211_ATTR_MAC";
    break;

//  case :
//    str += "";
//    break;

//    NL80211_ATTR_KEY_DATA,
//    NL80211_ATTR_KEY_IDX,
//    NL80211_ATTR_KEY_CIPHER,
//    NL80211_ATTR_KEY_SEQ,
//    NL80211_ATTR_KEY_DEFAULT,
//
//    NL80211_ATTR_BEACON_INTERVAL,
//    NL80211_ATTR_DTIM_PERIOD,
//    NL80211_ATTR_BEACON_HEAD,
//    NL80211_ATTR_BEACON_TAIL,
//
//    NL80211_ATTR_STA_AID,
//    NL80211_ATTR_STA_FLAGS,
//    NL80211_ATTR_STA_LISTEN_INTERVAL,
//    NL80211_ATTR_STA_SUPPORTED_RATES,
//    NL80211_ATTR_STA_VLAN,
//    NL80211_ATTR_STA_INFO,
//
  case NL80211_ATTR_WIPHY_BANDS:
    str += "NL80211_ATTR_WIPHY_BANDS";
    break;
//
//    NL80211_ATTR_MNTR_FLAGS,
//
//    NL80211_ATTR_MESH_ID,
//    NL80211_ATTR_STA_PLINK_ACTION,
//    NL80211_ATTR_MPATH_NEXT_HOP,
//    NL80211_ATTR_MPATH_INFO,
//
//    NL80211_ATTR_BSS_CTS_PROT,
//    NL80211_ATTR_BSS_SHORT_PREAMBLE,
//    NL80211_ATTR_BSS_SHORT_SLOT_TIME,
//
//    NL80211_ATTR_HT_CAPABILITY,
//
  case NL80211_ATTR_SUPPORTED_IFTYPES:
    str += "NL80211_ATTR_SUPPORTED_IFTYPES";
    break;
//
//    NL80211_ATTR_REG_ALPHA2,
//    NL80211_ATTR_REG_RULES,
//
//    NL80211_ATTR_MESH_CONFIG,
//
//    NL80211_ATTR_BSS_BASIC_RATES,
//
//    NL80211_ATTR_WIPHY_TXQ_PARAMS,

  case NL80211_ATTR_WIPHY_FREQ:
    str += "NL80211_ATTR_WIPHY_FREQ";
    break;
  case NL80211_ATTR_WIPHY_CHANNEL_TYPE:
    str += "NL80211_ATTR_WIPHY_CHANNEL_TYPE";
    break;

//    NL80211_ATTR_KEY_DEFAULT_MGMT,
//
//    NL80211_ATTR_MGMT_SUBTYPE,
//    NL80211_ATTR_IE,
//
    case NL80211_ATTR_MAX_NUM_SCAN_SSIDS:
    str += "NL80211_ATTR_MAX_NUM_SCAN_SSIDS";
    break;
//
//    NL80211_ATTR_SCAN_FREQUENCIES,
//    NL80211_ATTR_SCAN_SSIDS,

  case NL80211_ATTR_GENERATION:
    str += "NL80211_ATTR_GENERATION";
    break;
  case NL80211_ATTR_BSS:
    str += "NL80211_ATTR_BSS";
    break;
//    NL80211_ATTR_REG_INITIATOR,
//    NL80211_ATTR_REG_TYPE,
//
//    NL80211_ATTR_SUPPORTED_COMMANDS,
//
//    NL80211_ATTR_FRAME,

  case NL80211_ATTR_SSID:
    str += "NL80211_ATTR_SSID";
    break;
//    NL80211_ATTR_AUTH_TYPE,
//    NL80211_ATTR_REASON_CODE,
//
//    NL80211_ATTR_KEY_TYPE,
//
//    NL80211_ATTR_MAX_SCAN_IE_LEN,
//    NL80211_ATTR_CIPHER_SUITES,
//
//    NL80211_ATTR_FREQ_BEFORE,
//    NL80211_ATTR_FREQ_AFTER,
//
//    NL80211_ATTR_FREQ_FIXED,
//
//
//    NL80211_ATTR_WIPHY_RETRY_SHORT,
//    NL80211_ATTR_WIPHY_RETRY_LONG,
//    NL80211_ATTR_WIPHY_FRAG_THRESHOLD,
//    NL80211_ATTR_WIPHY_RTS_THRESHOLD,
//
//    NL80211_ATTR_TIMED_OUT,
//
//    NL80211_ATTR_USE_MFP,
//
//    NL80211_ATTR_STA_FLAGS2,
//
//    NL80211_ATTR_CONTROL_PORT,
//
//    NL80211_ATTR_TESTDATA,
//
//    NL80211_ATTR_PRIVACY,
//
//    NL80211_ATTR_DISCONNECTED_BY_AP,
//    NL80211_ATTR_STATUS_CODE,
//
//    NL80211_ATTR_CIPHER_SUITES_PAIRWISE,
//    NL80211_ATTR_CIPHER_SUITE_GROUP,
//    NL80211_ATTR_WPA_VERSIONS,
//    NL80211_ATTR_AKM_SUITES,
//
//    NL80211_ATTR_REQ_IE,
//    NL80211_ATTR_RESP_IE,
//
//    NL80211_ATTR_PREV_BSSID,
//
//    NL80211_ATTR_KEY,
//    NL80211_ATTR_KEYS,
//
//    NL80211_ATTR_PID,
//
//    NL80211_ATTR_4ADDR,
//
//    NL80211_ATTR_SURVEY_INFO,
//
//    NL80211_ATTR_PMKID,
//    NL80211_ATTR_MAX_NUM_PMKIDS,
//
//    NL80211_ATTR_DURATION,
//
//    NL80211_ATTR_COOKIE,
//
//    NL80211_ATTR_WIPHY_COVERAGE_CLASS,
//
//    NL80211_ATTR_TX_RATES,
//
//    NL80211_ATTR_FRAME_MATCH,
//
//    NL80211_ATTR_ACK,
//
//    NL80211_ATTR_PS_STATE,
//
//    NL80211_ATTR_CQM,
//
//    NL80211_ATTR_LOCAL_STATE_CHANGE,
//
//    NL80211_ATTR_AP_ISOLATE,
//
//    NL80211_ATTR_WIPHY_TX_POWER_SETTING,
//    ,
  case NL80211_ATTR_WIPHY_TX_POWER_LEVEL:
    str += "NL80211_ATTR_WIPHY_TX_POWER_LEVEL";
    break;
//
//    NL80211_ATTR_TX_FRAME_TYPES,
//    NL80211_ATTR_RX_FRAME_TYPES,
//    NL80211_ATTR_FRAME_TYPE,
//
//    NL80211_ATTR_CONTROL_PORT_ETHERTYPE,
//    NL80211_ATTR_CONTROL_PORT_NO_ENCRYPT,
//
//    NL80211_ATTR_SUPPORT_IBSS_RSN,
//
//    NL80211_ATTR_WIPHY_ANTENNA_TX,
//    NL80211_ATTR_WIPHY_ANTENNA_RX,
//
//    NL80211_ATTR_MCAST_RATE,
//
//    NL80211_ATTR_OFFCHANNEL_TX_OK,
//
//    NL80211_ATTR_BSS_HT_OPMODE,
//
//    NL80211_ATTR_KEY_DEFAULT_TYPES,
//
//    NL80211_ATTR_MAX_REMAIN_ON_CHANNEL_DURATION,
//
//    NL80211_ATTR_MESH_SETUP,
//
//    NL80211_ATTR_WIPHY_ANTENNA_AVAIL_TX,
//    NL80211_ATTR_WIPHY_ANTENNA_AVAIL_RX,
//
//    NL80211_ATTR_SUPPORT_MESH_AUTH,
//    NL80211_ATTR_STA_PLINK_STATE,
//
//    NL80211_ATTR_WOWLAN_TRIGGERS,
//    NL80211_ATTR_WOWLAN_TRIGGERS_SUPPORTED,
//
//    NL80211_ATTR_SCHED_SCAN_INTERVAL,
//
//    NL80211_ATTR_INTERFACE_COMBINATIONS,
  case NL80211_ATTR_SOFTWARE_IFTYPES:
    str += "NL80211_ATTR_SOFTWARE_IFTYPES";
    break;
//
//    NL80211_ATTR_REKEY_DATA,
//
//    NL80211_ATTR_MAX_NUM_SCHED_SCAN_SSIDS,
//    NL80211_ATTR_MAX_SCHED_SCAN_IE_LEN,
//
//    NL80211_ATTR_SCAN_SUPP_RATES,
//
//    NL80211_ATTR_HIDDEN_SSID,
//
//    NL80211_ATTR_IE_PROBE_RESP,
//    NL80211_ATTR_IE_ASSOC_RESP,
//
//    NL80211_ATTR_STA_WME,
//    NL80211_ATTR_SUPPORT_AP_UAPSD,
//
//    NL80211_ATTR_ROAM_SUPPORT,
//
//    NL80211_ATTR_SCHED_SCAN_MATCH,
//    NL80211_ATTR_MAX_MATCH_SETS,
//
//    NL80211_ATTR_PMKSA_CANDIDATE,
//
//    NL80211_ATTR_TX_NO_CCK_RATE,
//
//    NL80211_ATTR_TDLS_ACTION,
//    NL80211_ATTR_TDLS_DIALOG_TOKEN,
//    NL80211_ATTR_TDLS_OPERATION,
//    NL80211_ATTR_TDLS_SUPPORT,
//    NL80211_ATTR_TDLS_EXTERNAL_SETUP,
//
//    NL80211_ATTR_DEVICE_AP_SME,
//
//    NL80211_ATTR_DONT_WAIT_FOR_ACK,
//
//    NL80211_ATTR_FEATURE_FLAGS,
//
//    NL80211_ATTR_PROBE_RESP_OFFLOAD,
//
//    NL80211_ATTR_PROBE_RESP,
//
//    NL80211_ATTR_DFS_REGION,
//
//    NL80211_ATTR_DISABLE_HT,
//    NL80211_ATTR_HT_CAPABILITY_MASK,
//
//    NL80211_ATTR_NOACK_MAP,
//
//    NL80211_ATTR_INACTIVITY_TIMEOUT,
//
//    NL80211_ATTR_RX_SIGNAL_DBM,
//
//    NL80211_ATTR_BG_SCAN_PERIOD,

  case NL80211_ATTR_WDEV:
    str += "NL80211_ATTR_WDEV";
    break;
//    NL80211_ATTR_USER_REG_HINT_TYPE,
//
//    NL80211_ATTR_CONN_FAILED_REASON,
//
//    NL80211_ATTR_SAE_DATA,
//
//    NL80211_ATTR_VHT_CAPABILITY,
//
//    NL80211_ATTR_SCAN_FLAGS,

  case NL80211_ATTR_CHANNEL_WIDTH:
    str += "NL80211_ATTR_CHANNEL_WIDTH";
    break;
  case NL80211_ATTR_CENTER_FREQ1:
    str += "NL80211_ATTR_CENTER_FREQ1";
    break;
  case NL80211_ATTR_CENTER_FREQ2:
    str += "NL80211_ATTR_CENTER_FREQ2";
    break;

//    NL80211_ATTR_P2P_CTWINDOW,
//    NL80211_ATTR_P2P_OPPPS,
//
//    NL80211_ATTR_LOCAL_MESH_POWER_MODE,
//
//    NL80211_ATTR_ACL_POLICY,
//
//    NL80211_ATTR_MAC_ADDRS,
//
//    NL80211_ATTR_MAC_ACL_MAX,
//
//    NL80211_ATTR_RADAR_EVENT,
//
//    NL80211_ATTR_EXT_CAPA,
//    NL80211_ATTR_EXT_CAPA_MASK,
//
//    NL80211_ATTR_STA_CAPABILITY,
//    NL80211_ATTR_STA_EXT_CAPABILITY,
//
//    NL80211_ATTR_PROTOCOL_FEATURES,
//    NL80211_ATTR_SPLIT_WIPHY_DUMP,
//
//    NL80211_ATTR_DISABLE_VHT,
//    NL80211_ATTR_VHT_CAPABILITY_MASK,
//
//    NL80211_ATTR_MDID,
//    NL80211_ATTR_IE_RIC,
//
//    NL80211_ATTR_CRIT_PROT_ID,
//    NL80211_ATTR_MAX_CRIT_PROT_DURATION,
//
//    NL80211_ATTR_PEER_AID,
//
//    NL80211_ATTR_COALESCE_RULE,
//
//    NL80211_ATTR_CH_SWITCH_COUNT,
//    NL80211_ATTR_CH_SWITCH_BLOCK_TX,
//    NL80211_ATTR_CSA_IES,
//    NL80211_ATTR_CSA_C_OFF_BEACON,
//    NL80211_ATTR_CSA_C_OFF_PRESP,
//
//    NL80211_ATTR_RXMGMT_FLAGS,
//
//    NL80211_ATTR_STA_SUPPORTED_CHANNELS,
//
//    NL80211_ATTR_STA_SUPPORTED_OPER_CLASSES,
//
//    NL80211_ATTR_HANDLE_DFS,
//
//    NL80211_ATTR_SUPPORT_5_MHZ,
//    NL80211_ATTR_SUPPORT_10_MHZ,
//
//    NL80211_ATTR_OPMODE_NOTIF,
//
//    NL80211_ATTR_VENDOR_ID,
//    NL80211_ATTR_VENDOR_SUBCMD,
//    NL80211_ATTR_VENDOR_DATA,
//    NL80211_ATTR_VENDOR_EVENTS,
//
//    NL80211_ATTR_QOS_MAP,
//
//    NL80211_ATTR_MAC_HINT,
//    NL80211_ATTR_WIPHY_FREQ_HINT,
//
//    NL80211_ATTR_MAX_AP_ASSOC_STA,
//
//    NL80211_ATTR_TDLS_PEER_CAPABILITY,
//
//    NL80211_ATTR_SOCKET_OWNER,
//
//    NL80211_ATTR_CSA_C_OFFSETS_TX,
//    NL80211_ATTR_MAX_CSA_COUNTERS,
//
//    NL80211_ATTR_TDLS_INITIATOR,
//
//    NL80211_ATTR_USE_RRM,
//
//    NL80211_ATTR_WIPHY_DYN_ACK,
//
//    NL80211_ATTR_TSID,
//    NL80211_ATTR_USER_PRIO,
//    NL80211_ATTR_ADMITTED_TIME,
//
//    NL80211_ATTR_SMPS_MODE,
//
//    NL80211_ATTR_OPER_CLASS,
//
//    NL80211_ATTR_MAC_MASK,
//
//    NL80211_ATTR_WIPHY_SELF_MANAGED_REG,
//
//    NL80211_ATTR_EXT_FEATURES,
//
//    NL80211_ATTR_SURVEY_RADIO_STATS,
//
//    NL80211_ATTR_NETNS_FD,
//
//    NL80211_ATTR_SCHED_SCAN_DELAY,
//
//    NL80211_ATTR_REG_INDOOR,
//
//    NL80211_ATTR_MAX_NUM_SCHED_SCAN_PLANS,
//    NL80211_ATTR_MAX_SCAN_PLAN_INTERVAL,
//    NL80211_ATTR_MAX_SCAN_PLAN_ITERATIONS,
//    NL80211_ATTR_SCHED_SCAN_PLANS,
  default:
    str += "Unknown";
    break;
  }
  return(str);
};

//*****************************************************************************
// Class: GenericMessage
//*****************************************************************************

GenericMessage::GenericMessage(const int family_, const int flags_, const uint8_t id_)
{
  if (!genlmsg_put(this->operator ()(), NL_AUTO_PORT, NL_AUTO_SEQ,
      family_, 0, flags_, id_, 0))
  {
    ZLOG_ERR("Error initializing netlink generic message header");
  }
}

GenericMessage::GenericMessage(struct nl_msg* msg_) :
    Message(msg_)
{
}

GenericMessage::~GenericMessage()
{
}

bool
GenericMessage::Parse()
{

  int ret = 0;

  struct genlmsghdr *gnlhdr = (struct genlmsghdr *) this->Data();
  struct nlattr* attr = genlmsg_attrdata(gnlhdr, 0);
  int len = genlmsg_attrlen(gnlhdr, 0);

  ret = nla_parse(this->_attrs, NL80211_ATTR_MAX, attr, len, NULL);
  if (ret < 0)
  {
    ZLOG_ERR("Error parsing generic message attributes");
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
    return (false);
  }

  return(true);

}

void
GenericMessage::DisplayAttributes() const
{
  for(int i = 0; i < NL80211_ATTR_MAX; i++)
  {
    if (this->_attrs[i])
    {
      std::cout << _attr2str(i) << std::endl;
    }
  }
  std::cout << std::endl;
}

}

