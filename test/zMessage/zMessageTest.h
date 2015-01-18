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
zMessageTest_AuthDefaults(void* arg_);
int
zMessageTest_HelloDefaults(void* arg_);
int
zMessageTest_AckDefaults(void* arg_);
int
zMessageTest_ByeDefaults(void* arg_);
int
zMessageTest_CfgDefaults(void* arg_);
int
zMessageTest_CmdDefaults(void* arg_);
int
zMessageTest_DataDefaults(void* arg_);

int
zMessageTest_GetSet(void* arg_);

using namespace zUtils;
using namespace Test;

#endif /* _ZMESSAGETEST_H_ */
