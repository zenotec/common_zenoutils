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

#ifndef ZSEMTEST_H_
#define ZSEMTEST_H_

#include <zutils/zSem.h>
#include "UnitTest.h"

int
zSemTest_MutexDefaults(void* arg_);
int
zSemTest_SemaphoreDefaults(void* arg_);
int
zSemTest_Mutex(void* arg_);
int
zSemTest_Semaphore(void* arg_);

using namespace Test;
using namespace zUtils;

#endif /* ZSEMTEST_H_ */
