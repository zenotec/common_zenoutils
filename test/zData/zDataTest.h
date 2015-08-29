#ifndef _ZDATATEST_H_
#define _ZDATATEST_H_

#include "UnitTest.h"
#include "zutils/zData.h"

int
zDataTest_Defaults(void* arg);

int
zDataTest_SetValueMultiple(void* arg_);
int
zDataTest_SetValueSingle(void* arg_);

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
zDataTest_DataCopy( void* arg );

int
zDataTest_Array( void* arg );

#endif /* _ZDATATEST_H_ */

