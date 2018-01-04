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

#ifndef _ZQUEUETEST_H_
#define _ZQUEUETEST_H_

#include "UnitTest.h"

#include <zutils/zUuid.h>

int
zUuidTest_Defaults(void* arg_);

int
zUuidTest_Copy(void* arg_);

int
zUuidTest_Compare(void* arg_);

using namespace Test;
using namespace zUtils;

#endif /* _ZQUEUETEST_H_ */
