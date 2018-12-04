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

#ifndef __IEEE80211_RSNTAG_H__
#define __IEEE80211_RSNTAG_H__

#include <zutils/ieee80211/Tag.h>
#include <zutils/ieee80211/ieee80211.h>

namespace zUtils
{

namespace zWireless
{

namespace ieee80211
{


//*****************************************************************************
// Class: RsnTag
//*****************************************************************************

/* RSN TAG LAYOUT (After ID and Length)
 * ***** AS DEFINED IN 802.11-2016 SECTION 9.4.2.25.1 *******
 * All fields after Version are optional, if any field is 0 in length then
 * subsequent fields are not present.
 * m == Number of pairwise cipher suite counts supported
 * n == AKM cipher suite counts supported
 * s == PMKID count
 * Version - 2 Bytes
 * Group Data Cipher Suite - 0 or 4 Bytes
 * Pairwise Cipher Suite Count - 0 or 2 Bytes
 * Pairwise Cipher Suite List - 0 or (4 * m) Bytes
 * AKM Suite Count - 0 or 2 Bytes
 * AKM Suite List - 0 or (4*n) Bytes
 * RSN Capabilities - 0 or 2 Bytes
 * PMKID Count - 0 or 2 Bytes
 * PMKID List - 0 or (16 * s) Bytes
 * Group Management Cipher Suite - 0 or 4 Bytes
 */
class RsnTag : public Tag
{
public:
    enum RSN_PROTOCOL
    {
        VER_80211_2016 = 1,
        VER_LAST
    };

    // From IEEE 802.11-2016 Section 9.4.2.25.2
    enum IEEE_80211_CIPHER_SUITES
    {
        USE_GROUP_CIPHER = 0,
        WEP_40 = 1,
        TKIP = 2,
        RESERVED = 3,
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
        CIPHER_UKNOWN = 256,
        CIPHER_SUITE_LAST
    };

    // Taken from IEEE 802.11-2016 Sec 9.4.2.25.3
    enum AKM_CIPHER_SUITES
    {
        RESERVED = 0,
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
        AKM_CIPHER_UNKNOWN = 256,
        AKM_CIPHER_LAST
    };

    RsnTag(const RsnTag::RSN_PROTOCOL ver_ = VER_80211_2016) :
        Tag(Tag::ID_RSN)
    {
        _rsnProtocol.version = 0 | ver_;
    }

    virtual
    ~RsnTag()
    {
    }

    bool
    SetVersion()
    {
        return(this->PutValue(_rsnProtocol));
    }

    bool
    operator()(const struct rsn_suite_count& pw_cnt_, const struct rsn_suite (&pw_cipher_)[])
    {
        this->AddValue(pw_cnt_);
        return(this->AddValue(pw_cipher_));
    }

    template<typename T>
        bool
        operator()(T& val_)
        {
            return(this->AddValue(val_));
        }

    virtual std::vector<uint8_t>
    operator()() const
    {
        std::vector<uint8_t> rsn_info;
        rsn_info.resize(this->Length());
        this->GetValue(rsn_info.data(), rsn_info.size());
        return(rsn_info);
    }

    virtual void
    Display() const
    {
        int cnt = 0;
        Tag::Display();
        std::vector<uint8_t> rsn = this->operator()();
        uint16_t ver = ((uint16_t)rsn[0] << 8) | rsn[1];
        cnt += 2;
        std::cout << "Version: " << ver << std::endl;
        FOREACH(auto& rsn_info, rsn[cnt])
        {
            printf("0x%02x\n", rsn_info);
        }
    }

private:
    struct rsn_element _rsnProtocol;

};

}

}

}

#endif /* __IEEE80211_tAG_H__ */
