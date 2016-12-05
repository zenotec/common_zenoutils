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
int
zThreadTest_Synchronize(void* arg_);

using namespace Test;
using namespace zUtils;

class TestThreadArg : public zThread::ThreadArg, public zSem::Semaphore
{
public:

  TestThreadArg(void* id_, const std::string& name_, long cnt_) :
      Id(id_), Name(name_), LoopCnt(cnt_), ErrorCnt(0)
  {
    ZLOG_DEBUG(Name + "(" + ZLOG_P(this) + "): " + ZLOG_P(Id));
  }

  virtual
  ~TestThreadArg()
  {
    ZLOG_DEBUG(Name + "(" + ZLOG_P(this) + "): " + ZLOG_P(Id));
  }

  void* Id;
  std::string Name;
  long LoopCnt;
  long ErrorCnt;

protected:

private:

};

class TestFunction : public zThread::ThreadFunction
{
public:
  TestFunction() :
      _other(NULL)
  {
  }

  virtual
  ~TestFunction()
  {
  }

  TestThreadArg*
  GetSyncArg()
  {
    return (this->_other);
  }

  bool
  SetSyncArg(TestThreadArg* arg_)
  {
    this->_other = arg_;
    return (true);
  }

  virtual void
  Run(zThread::ThreadArg *arg_)
  {
    TestThreadArg* arg = (TestThreadArg*) arg_;

    if (arg == NULL)
    {
      ZLOG_ERR("(" + ZLOG_P(arg->Id) + "): NULL argument");
      return;
    }

    ZLOG_DEBUG(arg->Name + "(" + ZLOG_P(arg->Id) + "): Running");

    for (; (!this->Exit() && arg->LoopCnt); arg->LoopCnt--)
    {
      ZLOG_DEBUG(arg->Name + "(" + ZLOG_P(arg->Id) + "): Count: " +
          ZLOG_LONG(arg->LoopCnt) + "; Error: " + ZLOG_LONG(arg->ErrorCnt));
      // Wake up other thread
      ZLOG_DEBUG("Posting...");
      if (!arg->Post())
      {
        arg->ErrorCnt++;
        continue;
      }
      if (this->_other && !this->_other->TimedWait(100))
      {
        arg->ErrorCnt++;
      }
    }

    ZLOG_DEBUG(arg->Name + "(" + ZLOG_P(arg->Id) + "): Finishing");
    ZLOG_DEBUG(arg->Name + "(" + ZLOG_P(arg->Id) + "): Count: " +
        ZLOG_LONG(arg->LoopCnt) + "; Error: " + ZLOG_LONG(arg->ErrorCnt));
  }

private:

  TestThreadArg *_other;

};

class TestThread : public zThread::Thread
{

  friend TestFunction;

public:

  TestThread(const std::string& name_, int loops_) :
      zThread::Thread(&this->TestFunc, &this->TestArg), TestArg(this, name_, loops_)
  {
    this->Name(name_);
  }

  virtual
  ~TestThread()
  {
  }

  TestFunction TestFunc;
  TestThreadArg TestArg;

protected:

private:

};

#endif /* _ZTHREADTEST_H_ */
