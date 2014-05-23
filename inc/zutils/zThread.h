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

class zThreadFunction
{
public:
  virtual void *
  Function(void *arg_) = 0;
protected:
private:
};

//**********************************************************************
// zTimer Class
//**********************************************************************
class zThread : public zEvent
{
public:
  zThread(zThreadFunction *func_, void *arg_);
  ~zThread();

  uint32_t
  Id();

protected:

private:
  static void *
  _threadHandler(void *arg_);

  zThreadFunction *_func;
  void *_arg;

  pthread_t _id;
  bool _exit;

};

}

#endif /* __ZTHREAD_H__ */
