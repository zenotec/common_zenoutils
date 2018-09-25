/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
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

#ifndef _IEEE80211_IEEE80211TEST_H_
#define _IEEE80211_IEEE80211TEST_H_

#include "UnitTest.h"

extern const uint8_t radiotap[];
extern const size_t radiotap_len;

extern const uint8_t beacon[];
extern const size_t beacon_len;

extern const uint8_t probe_req[];
extern const size_t probe_req_len;

extern const uint8_t probe_resp[];
extern const size_t probe_resp_len;

extern const uint8_t ass_req[];
extern const size_t ass_req_len;

extern const uint8_t ass_resp[];
extern const size_t ass_resp_len;

extern const uint8_t rts[];
extern const size_t rts_len;

extern const uint8_t cts[];
extern const size_t cts_len;

extern const uint8_t ack[];
extern const size_t ack_len;

extern const uint8_t auth[];
extern const size_t auth_len;

int
Ieee80211Test_RadiotapDefaults(void* arg_);
int
Ieee80211Test_RadiotapGetSet(void* arg_);
int
Ieee80211Test_RadiotapAssemble(void* arg_);
int
Ieee80211Test_RadiotapDisassemble(void* arg_);

int
Ieee80211Test_FrameDefaults(void* arg_);
int
Ieee80211Test_FrameGetSet(void* arg_);
int
Ieee80211Test_FrameAssemble(void* arg_);
int
Ieee80211Test_FrameDisassemble(void* arg_);

int
Ieee80211Test_ControlFrameDefaults(void* arg_);
int
Ieee80211Test_ControlFrameGetSet(void* arg_);
int
Ieee80211Test_ControlFrameAssemble(void* arg_);
int
Ieee80211Test_ControlFrameDisassemble(void* arg_);

int
Ieee80211Test_ManagementFrameDefaults(void* arg_);
int
Ieee80211Test_ManagementFrameGetSet(void* arg_);
int
Ieee80211Test_ManagementFrameAssemble(void* arg_);
int
Ieee80211Test_ManagementFrameDisassemble(void* arg_);

int
Ieee80211Test_BeaconDefaults(void* arg_);
int
Ieee80211Test_BeaconGetSet(void* arg_);
int
Ieee80211Test_BeaconAssemble(void* arg_);
int
Ieee80211Test_BeaconDisassemble(void* arg_);

int
Ieee80211Test_ProbeRequestDefaults(void* arg_);
int
Ieee80211Test_ProbeRequestGetSet(void* arg_);
int
Ieee80211Test_ProbeRequestAssemble(void* arg_);
int
Ieee80211Test_ProbeRequestDisassemble(void* arg_);

int
Ieee80211Test_ProbeResponseDefaults(void* arg_);
int
Ieee80211Test_ProbeResponseGetSet(void* arg_);
int
Ieee80211Test_ProbeResponseAssemble(void* arg_);
int
Ieee80211Test_ProbeResponseDisassemble(void* arg_);

int
Ieee80211Test_AssociationRequestDefaults(void* arg_);
int
Ieee80211Test_AssociationRequestGetSet(void* arg_);
int
Ieee80211Test_AssociationRequestAssemble(void* arg_);
int
Ieee80211Test_AssociationRequestDisassemble(void* arg_);

int
Ieee80211Test_AssociationResponseDefaults(void* arg_);
int
Ieee80211Test_AssociationResponseGetSet(void* arg_);
int
Ieee80211Test_AssociationResponseAssemble(void* arg_);
int
Ieee80211Test_AssociationResponseDisassemble(void* arg_);

int
Ieee80211Test_ReassociationRequestDefaults(void* arg_);
int
Ieee80211Test_ReassociationRequestGetSet(void* arg_);
int
Ieee80211Test_ReassociationRequestAssemble(void* arg_);
int
Ieee80211Test_ReassociationRequestDisassemble(void* arg_);

int
Ieee80211Test_ReassociationResponseDefaults(void* arg_);
int
Ieee80211Test_ReassociationResponseGetSet(void* arg_);
int
Ieee80211Test_ReassociationResponseAssemble(void* arg_);
int
Ieee80211Test_ReassociationResponseDisassemble(void* arg_);

int
Ieee80211Test_DisassociationDefaults(void* arg_);
int
Ieee80211Test_DisassociationGetSet(void* arg_);
int
Ieee80211Test_DisassociationAssemble(void* arg_);
int
Ieee80211Test_DisassociationDisassemble(void* arg_);

int
Ieee80211Test_AuthenticationDefaults(void* arg_);
int
Ieee80211Test_AuthenticationGetSet(void* arg_);
int
Ieee80211Test_AuthenticationAssemble(void* arg_);
int
Ieee80211Test_AuthenticationDisassemble(void* arg_);

int
Ieee80211Test_DeauthenticationDefaults(void* arg_);
int
Ieee80211Test_DeauthenticationGetSet(void* arg_);
int
Ieee80211Test_DeauthenticationAssemble(void* arg_);
int
Ieee80211Test_DeauthenticationDisassemble(void* arg_);

int
Ieee80211Test_DataFrameDefaults(void* arg_);
int
Ieee80211Test_DataFrameGetSet(void* arg_);
int
Ieee80211Test_DataFrameAssemble(void* arg_);
int
Ieee80211Test_DataFrameDisassemble(void* arg_);

#endif /* _IEEE80211_IEEE80211TEST_H_ */
