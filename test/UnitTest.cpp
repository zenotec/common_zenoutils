/*
 * UnitTest.cpp
 *
 *  Created on: May 9, 2014
 *      Author: Kevin Mahoney
 */

#include "UnitTest.h"

int
zData_utest(void);
int
zNode_utest(void);

int
main(int argc, const char **argv)
{
  int ret = 0;
  ret |= zData_utest();
  ret |= zNode_utest();
  return (ret);
}
