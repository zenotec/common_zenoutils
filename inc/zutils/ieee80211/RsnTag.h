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

    // For this problem space, we are supporting the default values for a NON-DMG wireless access
    // This is per IEEE 802.11-2016
    struct rsn_default
    {
        struct rsn_element protocol;
        struct rsn_suite group_data_cipher;
        struct rsn_suite_count pairwise_count;
        struct rsn_suite pairwise_cipher_suite;
        struct rsn_suite_count akm_count;
        struct rsn_suite akm_suite_list;

        bool
        operator!=(const rsn_default& rhs) const
        {
            if( this->protocol.version != rhs.protocol.version &&
                this->group_data_cipher.cipher_oui.oui_b1 != rhs.group_data_cipher.cipher_oui.oui_b1 &&
                this->group_data_cipher.cipher_oui.oui_b2 != rhs.group_data_cipher.cipher_oui.oui_b2 &&
                this->group_data_cipher.cipher_oui.oui_b3 != rhs.group_data_cipher.cipher_oui.oui_b3 &&
                this->pairwise_count.suite_count != rhs.pairwise_count.suite_count &&
                this->pairwise_cipher_suite.cipher_oui.oui_b1 != rhs.pairwise_cipher_suite.cipher_oui.oui_b1 &&
                this->pairwise_cipher_suite.cipher_oui.oui_b2 != rhs.pairwise_cipher_suite.cipher_oui.oui_b2 &&
                this->pairwise_cipher_suite.cipher_oui.oui_b3 != rhs.pairwise_cipher_suite.cipher_oui.oui_b3 &&
                this->akm_count.suite_count != rhs.akm_count.suite_count &&
                this->akm_suite_list.cipher_oui.oui_b1 != rhs.akm_suite_list.cipher_oui.oui_b1 &&
                this->akm_suite_list.cipher_oui.oui_b2 != rhs.akm_suite_list.cipher_oui.oui_b2 &&
                this->akm_suite_list.cipher_oui.oui_b3 != rhs.akm_suite_list.cipher_oui.oui_b3 )
                return true;
            return false;
        }
/*
        bool
        operator==(const rsn_default& lhs, const rsn_default& rhs) const
        {
            if( lhs.protocol == rhs.protocol &&
                lhs.group_data_cipher == rhs.group_data_cipher &&
                lhs.pairwise_count == rhs.pairwise_count &&
                lhs.pairwise_cipher_suite == rhs.pairwise_cipher_suite &&
                lhs.akm_count == rhs.akm_count &&
                lhs.akm_suite_list == rhs.pairwise_cipher_suite )
                return true;
            return false;
        }
        */
    }__attribute__((packed));

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

    RsnTag() :
        Tag(Tag::ID_RSN)
    {
        _rsnProtocol.version = 0;
        _parsed = false;
        _group_cipher = RsnTag::CIPHER_UNKNOWN;
        // Default OUI for all RSN elements is 00:0F:AC
        _rsnOui.oui_b1 = 0x00;
        _rsnOui.oui_b2 = 0x0F;
        _rsnOui.oui_b3 = 0xAC;
    }

    virtual
    ~RsnTag()
    {
    }


    RsnTag::rsn_default
    operator()();

    bool
    operator()(const RsnTag::IEEE_80211_CIPHER_SUITES group_cipher_,
                const std::vector<RsnTag::IEEE_80211_CIPHER_SUITES> pairwise_ciphers_,
                const std::vector<RsnTag::AKM_CIPHER_SUITES> akm_ciphers_);

    bool
    PushVersion()
    {
        return(this->PutValue(_rsnProtocol));
    }

    // Only one version currently defined. That is a value of 1.
    void
    SetVersion(const RsnTag::RSN_PROTOCOL ver_ = VER_80211_2016)
    {
        _rsnProtocol.version = (uint16_t)ver_;
    }

    // Return the version. Hopefully has a value of 1.
    RSN_PROTOCOL
    GetVersion() const
    {
        return((RsnTag::RSN_PROTOCOL)_rsnProtocol.version);
    }

    void
    SetGroupDataCipher(IEEE_80211_CIPHER_SUITES group_ciphers_)
    {
        _group_cipher = group_ciphers_;
    }

    RsnTag::IEEE_80211_CIPHER_SUITES
    GetGroupDataCipher() const
    {
        return(_group_cipher);
    }

    void
    SetPairwiseCiphers(std::vector<RsnTag::IEEE_80211_CIPHER_SUITES> pairwiseCiphers_)
    {
        _pairwiseCiphers = pairwiseCiphers_;
    }

    std::vector<RsnTag::IEEE_80211_CIPHER_SUITES>
    GetPairwiseCiphers() const
    {
        return(_pairwiseCiphers);
    }

    void
    SetAkmSuites(std::vector<RsnTag::AKM_CIPHER_SUITES> akmSuites_)
    {
        _akmCiphers = akmSuites_;
    }

    std::vector<RsnTag::AKM_CIPHER_SUITES>
    GetAkmSuites() const
    {
        return(_akmCiphers);
    }

    RsnTag::rsn_default
    BuildDefault() const;


    virtual void
    Display() const
    {
        struct rsn_default prnt_rsn = this->BuildDefault();

        std::cout << "Version: " << prnt_rsn.protocol.version << std::endl;
        std::cout << "Group Cipher: " << prnt_rsn.group_data_cipher.cipher_oui.oui_b1 <<
                prnt_rsn.group_data_cipher.cipher_oui.oui_b2 << prnt_rsn.group_data_cipher.cipher_oui.oui_b3
                << " Type: " << prnt_rsn.group_data_cipher.cipher_suite_type << std::endl;
        std::cout << "Number Pairwise Suites: " << prnt_rsn.pairwise_count.suite_count << std::endl;
        std::cout << "Pairwise Cipher: " << prnt_rsn.pairwise_cipher_suite.cipher_oui.oui_b1 <<
                prnt_rsn.pairwise_cipher_suite.cipher_oui.oui_b2 << prnt_rsn.pairwise_cipher_suite.cipher_oui.oui_b3
                << " Type: " << prnt_rsn.pairwise_cipher_suite.cipher_suite_type << std::endl;
        std::cout << "Number AKM Suites: " << prnt_rsn.akm_count.suite_count << std::endl;
        std::cout << "Pairwise Cipher: " << prnt_rsn.akm_suite_list.cipher_oui.oui_b1 <<
                prnt_rsn.akm_suite_list.cipher_oui.oui_b2 << prnt_rsn.akm_suite_list.cipher_oui.oui_b3
                << " Type: " << prnt_rsn.akm_suite_list.cipher_suite_type << std::endl;
    }

private:
    struct rsn_element _rsnProtocol;
    struct rsn_oui_format _rsnOui;
    IEEE_80211_CIPHER_SUITES _group_cipher;
    std::vector<IEEE_80211_CIPHER_SUITES> _pairwiseCiphers;
    std::vector<AKM_CIPHER_SUITES> _akmCiphers;
    bool _parsed;

    void
    ParseTag(std::vector<uint8_t> values);
};

}

}

}

#endif /* __IEEE80211_tAG_H__ */
