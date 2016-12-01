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

class TestFunction : public zThread::ThreadFunction
{
public:
  TestFunction(int cnt_ = 1) :
      _cnt(cnt_)
  {
  }

  virtual
  ~TestFunction()
  {
  }

  int
  GetCount()
  {
    return (this->_cnt);
  }

  bool
  SetCount(int cnt_)
  {
    this->_cnt = cnt_;
    return (true);
  }

  virtual void
  Run(zThread::ThreadArg *arg_)
  {
    while (!this->Exit() && this->_cnt--)
    {
      usleep(100000);
    }
  }

private:

  int _cnt;

};

class TestThread : public zThread::Thread, public zThread::ThreadArg
{

public:

  TestThread(int loops_) :
    zThread::Thread(&this->TestFunc, this), TestFunc(loops_)
  {
  }

  virtual
  ~TestThread()
  {
  }

  TestFunction TestFunc;

protected:

private:


};

#endif /* _ZTHREADTEST_H_ */
