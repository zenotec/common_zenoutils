//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zThread.h
//    Description:
//
//*****************************************************************************

#ifndef __ZTHREAD_H__
#define __ZTHREAD_H__

#include <pthread.h>

#include "zutils/zEvent.h"

namespace zUtils
{

//**********************************************************************
// zTimer Class
//**********************************************************************
class zThread : public zEvent
{
public:
  zThread();
  ~zThread();

protected:

private:
  pthread_t _threadId;
  bool _exit;

};

}

#endif /* __ZTHREAD_H__ */
