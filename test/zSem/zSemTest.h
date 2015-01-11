/*
 * zSemTest.h
 *
 *  Created on: Jan 9, 2015
 *      Author: kmahoney
 */

#ifndef ZSEMTEST_H_
#define ZSEMTEST_H_

#include <zutils/zSem.h>
#include "UnitTest.h"

int
zSemTest_Defaults(void* arg_);
int
zSemTest_Mutex(void* arg_);
int
zSemTest_Semaphore(void* arg_);

using namespace Test;
using namespace zUtils;

#endif /* ZSEMTEST_H_ */
