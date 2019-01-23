/*
 * Copyright (c) 2018 Cablelabs (http://www.cablelabs.com)
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



#ifndef _ZSECURITY_ZSECURITYTEST_H_
#define _ZSECURITY_ZSECURITYTEST_H_

#include "UnitTest.h"

extern const uint8_t eapol_pkt_1[];
extern const uint8_t eapol_pkt_2[];
extern const uint8_t eapol_pkt_3[];
extern const uint8_t eapol_pkt_4[];
extern const size_t eapol_pkt_1_len;
extern const size_t eapol_pkt_1_len;
extern const size_t eapol_pkt_3_len;
extern const size_t eapol_pkt_4_len;

int
zSecurityTest_Defaults(void* arg_);
int
zSecurityTest_GetSet(void* arg_);
int
zSecurityTest_Handshake_Mes1(void* arg_);
int
zSecurityTest_Handshake_Mes2(void* arg_);
int
zSecurityTest_Handshake_Mes3(void* arg_);
int
zSecurityTest_Handshake_Mes4(void* arg_);




#endif
