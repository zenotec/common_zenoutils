/*
 * Copyright (c) 2016 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
 *
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

#ifndef __ZWIRELESSTEST_H__
#define __ZWIRELESSTEST_H__

int
zWirelessTest_WirelessConfigurationDefaults(void* arg);
int
zWirelessTest_WirelessDefaults(void* arg);

int
zWirelessTest_WirelessConfiguration(void* arg);

int
zWirelessTest_WirelessInterface(void* arg);
int
zWirelessTest_MonitorInterface(void* arg);
int
zWirelessTest_AccessPointInterface(void* arg);
int
zWirelessTest_setFrequency(void* arg);
int
zWirelessTest_BasicServiceSet(void* arg);
int
zWirelessTest_GetSsid(void* arg);
int
zWirelessTest_SetSsid(void* arg);
int
zWirelessTest_GetBssid(void* arg);
int
zWirelessTest_SetBssid(void* arg);
int
zWirelessTest_TestInterface(void* arg); //
int
zWirelessTest_getCenterFrequency1(void* arg);
#endif /* __ZWIRELESSTEST_H__ */
