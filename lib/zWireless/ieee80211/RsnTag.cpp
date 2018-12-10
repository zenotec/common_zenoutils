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
#include <zutils/ieee80211/RsnTag.h>

ZLOG_MODULE_INIT(zLog::Log::MODULE_WIRELESS);

namespace zUtils
{

namespace zWireless
{

namespace ieee80211
{

#define GROUP_DATA_DEFAULT 4
#define PAIRWISE_CIPHER_DEFAULT 4
#define AKM_CIPHER_DEFAULT 2

static void
pushOui(std::vector<uint8_t>& payload, struct rsn_oui_format rsnOui)
{
    payload.push_back(rsnOui.oui_b1);
    payload.push_back(rsnOui.oui_b2);
    payload.push_back(rsnOui.oui_b3);
}

static bool
verifyOui(uint8_t b1_, uint8_t b2_, uint8_t b3_, struct rsn_oui_format orig)
{
    if(orig.oui_b1 == b3_ && orig.oui_b2 == b2_ && orig.oui_b3 == b1_)
        return true;
    return false;
}

template<typename T>
static std::vector<T>
unpackSuiteList(T t, std::vector<uint8_t>& val, int& cntr, struct rsn_oui_format orig)
{
    std::vector<T> retList;
    uint16_t numList = 0;
    uint16_t totalPair = ((uint16_t)val[cntr++]) | ((uint16_t)val[cntr++] << 8 );
    while(numList < totalPair)
    {
        if(verifyOui(val[cntr++], val[cntr++], val[cntr++], orig))
        {
            retList.push_back((T)val[cntr++]);
        }
        else
        {
            ++cntr;
        }
        ++numList;
    }
    return(retList);
}

static RsnTag::IEEE_80211_CIPHER_SUITES
findDefaultCipher(std::vector<RsnTag::IEEE_80211_CIPHER_SUITES> suites)
{
    FOREACH(auto& ciph, suites)
    {
        if(ciph == RsnTag::CCMP_128)
            return(ciph);
    }
    return(RsnTag::CIPHER_UNKNOWN);
}

// Will have to make changes later, since we will be doing enterprise.
static RsnTag::AKM_CIPHER_SUITES
findDefaultAkmCipher(std::vector<RsnTag::AKM_CIPHER_SUITES> suites)
{
    FOREACH(auto& ciph, suites)
    {
        if(ciph == RsnTag::PSK)
            return(ciph);
    }
    return(RsnTag::AKM_CIPHER_UNKNOWN);
}

struct RsnTag::rsn_default
RsnTag::BuildDefault() const
{
    struct rsn_default rsn_def;
    rsn_def.protocol = _rsnProtocol;
    rsn_def.group_data_cipher.cipher_oui = _rsnOui;
    rsn_def.group_data_cipher.cipher_suite_type = (uint8_t)GetGroupDataCipher();
    rsn_def.pairwise_count.suite_count = 1;
    rsn_def.pairwise_cipher_suite.cipher_oui = _rsnOui;
    rsn_def.pairwise_cipher_suite.cipher_suite_type = (uint8_t)findDefaultCipher(GetPairwiseCiphers());
    rsn_def.akm_count.suite_count = 1;
    rsn_def.akm_suite_list.cipher_oui = _rsnOui;
    rsn_def.akm_suite_list.cipher_suite_type = (uint8_t)findDefaultAkmCipher(GetAkmSuites());
    return(rsn_def);
}

// This method will parse the tag information if it hasn't already.
void
RsnTag::ParseTag(std::vector<uint8_t> values)
{
    int cnt = 0;

    bool group = false;
    bool pair = false;
    bool akm = false;
    bool unsup = false;
    //First two bytes are always the version
    uint16_t ver = ((uint16_t)values[cnt++]) | ((uint16_t)values[cnt++] << 8 );

    //Currently there is only 1 version number defined by IEEE 802.11-2016
    SetVersion((RsnTag::RSN_PROTOCOL)ver);

    // Iterate through values, everything is varied length.
    // We only care up to the AKM.
    while(cnt < values.size())
    {
        if(!group)
        {
            if(verifyOui(values[cnt++], values[cnt++], values[cnt++], _rsnOui))
            {
                group = true;
                SetGroupDataCipher((RsnTag::IEEE_80211_CIPHER_SUITES)values[cnt++]);
            }
            else
            {
                unsup = true;
                break;
            }
        }
        else if(!pair)
        {

            std::vector<IEEE_80211_CIPHER_SUITES> pair_list =
                    unpackSuiteList(RsnTag::CIPHER_SUITE_LAST, values, cnt, _rsnOui);
            pair = true;
            if(pair_list.size())
            {
                SetPairwiseCiphers(pair_list);
            }
            else
            {
                // If we iterate through list and none found, then we know we don't support
                SetPairwiseCiphers({RsnTag::CIPHER_UNKNOWN});
            }
        }
        else if(!akm)
        {
            std::vector<RsnTag::AKM_CIPHER_SUITES> akm_list =
                        unpackSuiteList(RsnTag::AKM_CIPHER_LAST, values, cnt, _rsnOui);
            akm = true;
            if(akm_list.size())
            {
                SetAkmSuites(akm_list);
            }
            else
            {
                // If we iterate through list and none found, then we know we don't support
                SetAkmSuites({RsnTag::AKM_CIPHER_UNKNOWN});
            }
        }
        else
        {
            // Currently we don't care about the rest of the values
            break;
        }
    }
    // Not a supported set
    if(unsup)
    {
        SetGroupDataCipher((RsnTag::IEEE_80211_CIPHER_SUITES)RsnTag::CIPHER_UNKNOWN);
        SetPairwiseCiphers({(RsnTag::IEEE_80211_CIPHER_SUITES)RsnTag::CIPHER_UNKNOWN});
        SetAkmSuites({(RsnTag::AKM_CIPHER_SUITES)RsnTag::AKM_CIPHER_UNKNOWN});
    }

}

struct RsnTag::rsn_default
RsnTag::operator()()
{
    std::vector<uint8_t> rsn;
    rsn.resize(this->Length());
    this->GetValue(rsn.data(), rsn.size());
    if(!_parsed && !rsn.empty())
    {
        ParseTag(rsn);
    }
    return(BuildDefault());
}

// Assumption is that Ciphers passed in here are all of the IEEE specs
// This assumes all will have the same OUI
bool
RsnTag::operator()(const RsnTag::IEEE_80211_CIPHER_SUITES group_cipher_,
                        const std::vector<RsnTag::IEEE_80211_CIPHER_SUITES> pairwise_ciphers_,
                        const std::vector<RsnTag::AKM_CIPHER_SUITES> akm_ciphers_)
{
    std::vector<uint8_t> payload;

    //Push items into tag, as defined by IEEE802.11-2016
    SetVersion();
    pushOui(payload, _rsnOui);
    payload.push_back((uint8_t) group_cipher_);
    SetGroupDataCipher(group_cipher_);

    // Pairwise cipher size per specs is 2 bytes
    uint16_t var = pairwise_ciphers_.size();
    //payload.push_back((uint8_t)(var & 0x0F));
    //payload.push_back((uint8_t)((var & 0xF0) >> 8));

    payload.push_back((uint8_t)(var));
    payload.push_back((uint8_t)(var >> 8));


    FOREACH(auto& pairwise, pairwise_ciphers_)
    {
        pushOui(payload, _rsnOui);
        payload.push_back((uint8_t) pairwise);
    }
    SetPairwiseCiphers(pairwise_ciphers_);

    var = akm_ciphers_.size();
    payload.push_back((uint8_t)(var));
    payload.push_back((uint8_t)(var >> 8));
    // AKM Size is 2 bytes as per 802.11-2016 specs

    FOREACH(auto& akm, akm_ciphers_)
    {
        pushOui(payload, _rsnOui);
        payload.push_back((uint8_t) akm);
    }
    SetAkmSuites(akm_ciphers_);

    if(PushVersion())
    {
        _parsed = true;
        return(this->AddValue(payload.data(), payload.size()));
    }
    return false;
}





} // End ieee80211 ns

} // End zWireless ns

} // End zUtils ns
