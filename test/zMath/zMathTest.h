/*
 * Copyright (c) 2014-2017 ZenoTec LLC (http://www.zenotec.net)
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

#ifndef _ZMATHTEST_H_
#define _ZMATHTEST_H_

#include "UnitTest.h"
#include <zutils/zMath.h>

using namespace zUtils;

int
zMathTest_Min(void* arg_);

int
zMathTest_Max(void* arg_);

int
zMathTest_Sum(void* arg_);

int
zMathTest_SumSquares(void* arg_);

int
zMathTest_Mean(void* arg_);

int
zMathTest_Variance(void* arg_);

int
zMathTest_StandardDeviation(void* arg_);

#endif /* _ZMATHTEST_H_ */
