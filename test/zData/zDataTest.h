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

#ifndef _ZDATATEST_H_
#define _ZDATATEST_H_

#include "UnitTest.h"
#include "zutils/zData.h"

int
zDataTest_PathDefaults(void* arg);
int
zDataTest_Defaults(void* arg);

int
zDataTest_PathAppend(void* arg);
int
zDataTest_PathPrepend(void* arg);
int
zDataTest_PathCompare(void* arg);

int
zDataTest_PutValueMultiple(void* arg_);
int
zDataTest_PutValueSingle(void* arg_);

int
zDataTest_GetChildSingle(void* arg_);
int
zDataTest_GetChildMultiple(void* arg_);
int
zDataTest_PutChildSingle(void* arg_);
int
zDataTest_PutChildMultiple(void* arg_);

int
zDataTest_JsonSimple(void* arg_);
int
zDataTest_XmlSimple(void* arg_);

int
zDataTest_DataCopy(void* arg);

int
zDataTest_Array(void* arg);

#endif /* _ZDATATEST_H_ */

