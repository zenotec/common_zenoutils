/*
 * zConfTest.h
 *
 *  Created on: Jan 11, 2015
 *      Author: kmahoney
 */

#ifndef _ZCONFTEST_H_
#define _ZCONFTEST_H_

#include "UnitTest.h"

#include <zutils/zConf.h>

int
zConfTest_DataDefaults(void* arg_);
int
zConfTest_ConnectorDefaults(void* arg_);

using namespace Test;
using namespace zUtils;

class TestObserver : public zConf::Observer
{
public:
  TestObserver();
  virtual
  ~TestObserver();

  virtual void
  Prepare(zConf::Data *item_)
  {

  }

  virtual void
  Commit(zConf::Data *item_)
  {

  }

private:
};

#endif /* _ZCONFTEST_H_ */
