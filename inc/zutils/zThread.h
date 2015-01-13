//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zThread.h
//    Description:
//
//*****************************************************************************

#ifndef __ZTHREAD_H__
#define __ZTHREAD_H__

#include <stdint.h>
#include <pthread.h>

#include "zutils/zEvent.h"

namespace zUtils
{
namespace zThread
{

//**********************************************************************
// Function Class
//**********************************************************************

class Function
{
public:
  virtual void *
  ThreadFunction(void *arg_) = 0;
protected:
private:
};

//**********************************************************************
// Thread Class
//**********************************************************************

class Thread
{
public:
  Thread(Function *func_, void *arg_);
  ~Thread();

  unsigned long
  GetId();

  bool
  Run();

  bool
  Join(uint32_t msecs_);

protected:

private:
  static void *
  _threadHandler(void *arg_);

  zSem::Mutex _mutex;
  pthread_t _id;
  Function *_func;
  void *_arg;
  bool _exit;

};

}
}

#endif /* __ZTHREAD_H__ */
