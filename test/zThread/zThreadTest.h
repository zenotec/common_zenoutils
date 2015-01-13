/*
 * zThreadTest.h
 *
 *  Created on: Jan 9, 2015
 *      Author: kmahoney
 */

#ifndef _ZTHREADTEST_H_
#define _ZTHREADTEST_H_

#include <unistd.h>

#include <zutils/zThread.h>

#include "UnitTest.h"

int
zThreadTest_Defaults(void* arg_);
int
zThreadTest_RunOnce(void* arg_);
int
zThreadTest_RunMultiple(void* arg_);

using namespace Test;
using namespace zUtils;

class TestFunction : public zThread::Function, public zThread::Thread
{
public:
  TestFunction(unsigned int cnt_ = 1) :
      _cnt(cnt_), zThread::Thread(this, this)
  {
  }

  virtual
  ~TestFunction()
  {
  }

  unsigned int
  GetCount()
  {
    return (this->_cnt);
  }

  virtual void *
  ThreadFunction(void *arg_)
  {
    usleep(1000);
    if (this->_cnt > 0)
      this->_cnt--;
    if (!this->_cnt)
      return ((void*) 1);
    else
      return (NULL);
  }

private:
  unsigned int _cnt;

};

#endif /* _ZTHREADTEST_H_ */
