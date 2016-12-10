/*
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

#ifndef _ZOPTTEST_H_
#define _ZOPTTEST_H_

#include "UnitTest.h"

#include <zutils/zProgOpt.h>

int
zOptTest_OptionDefaults(void* arg_);
int
zOptTest_TableDefaults(void* arg_);

int
zOptTest_OptionString(void* arg_);
int
zOptTest_OptionInt(void* arg_);
int
zOptTest_OptionUint(void* arg_);
int
zOptTest_OptionDouble(void* arg_);

int
zOptTest_ParseSimpleShort(void* arg_);
int
zOptTest_ParseSimpleLong(void* arg_);

using namespace Test;
using namespace zUtils;

#endif /* _ZOPTTEST_H_ */
