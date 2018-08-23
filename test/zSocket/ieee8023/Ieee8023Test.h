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

#ifndef _IEEE8023_IEEE8023TEST_H_
#define _IEEE8023_IEEE8023TEST_H_

#include "UnitTest.h"

extern const uint8_t arp_pkt[];
extern const size_t arp_pkt_len;

int
Ieee8023Test_Defaults(void* arg_);
int
Ieee8023Test_GetSet(void* arg_);
int
Ieee8023Test_Assemble(void* arg_);
int
Ieee8023Test_Disassemble(void* arg_);


#endif /* _IEEE8023_IEEE8023TEST_H_ */
