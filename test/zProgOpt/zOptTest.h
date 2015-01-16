/*
 * zOptTest.h
 *
 *  Created on: Jan 10, 2015
 *      Author: kmahoney
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
