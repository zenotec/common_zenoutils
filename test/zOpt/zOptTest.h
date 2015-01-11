/*
 * zOptTest.h
 *
 *  Created on: Jan 10, 2015
 *      Author: kmahoney
 */

#ifndef _ZOPTTEST_H_
#define _ZOPTTEST_H_

#include "UnitTest.h"

#include <zutils/zOpt.h>

int
zOptTest_Defaults(void* arg_);
int
zOptTest_ParseSimpleShort(void* arg_);
int
zOptTest_ParseSimpleLong(void* arg_);

using namespace Test;
using namespace zUtils;

#endif /* _ZOPTTEST_H_ */
