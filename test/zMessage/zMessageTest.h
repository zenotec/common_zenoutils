/*
 * zMessageTest.h
 *
 *  Created on: Jan 9, 2015
 *      Author: kmahoney
 */

#ifndef _ZMESSAGETEST_H_
#define _ZMESSAGETEST_H_

#include "UnitTest.h"

#include <zutils/zMessage.h>

int
zMessageTest_Defaults(void* arg_);
int
zMessageTest_HelloDefaults(void* arg_);
int
zMessageTest_AckDefaults(void* arg_);
int
zMessageTest_ByeDefaults(void* arg_);

#endif /* _ZMESSAGETEST_H_ */
