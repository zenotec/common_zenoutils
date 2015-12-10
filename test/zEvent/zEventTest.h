/*
 * zEventTest.h
 *
 *  Created on: Jan 9, 2015
 *      Author: kmahoney
 */

#ifndef _ZEVENTTEST_H_
#define _ZEVENTTEST_H_

#include <zutils/zEvent.h>

#include "UnitTest.h"

int
zEventTest_Defaults(void * arg_);
int
zEventTest_EventHandler(void* arg_);

using namespace zUtils;
using namespace Test;

#endif /* _ZEVENTTEST_H_ */
