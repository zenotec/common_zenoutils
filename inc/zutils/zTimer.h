//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zTimer.h
//    Description:
//
//*****************************************************************************

#ifndef __TIMER_H__
#define __TIMER_H__

#include <time.h>

#include <list>

#include "zutils/zSemaphore.h"
#include "zutils/zEvent.h"

namespace zUtils
{

//**********************************************************************
// zTimerHandler Class
//**********************************************************************
class zTimerHandler
{
public:
  virtual void
  TimerTick() = 0;

protected:

private:

};
//**********************************************************************
// zTimer Class
//**********************************************************************
class zTimer : public zEvent
{
public:
  zTimer();
  ~zTimer();

  void
  Start(uint32_t usec_);

  void
  Register(zTimerHandler *obs_);

  void
  Unregister(zTimerHandler *obs_);

protected:

private:
  timer_t _timerid;
  static void
  _handler(union sigval sv_);

  void
  _start();
  uint32_t _interval;

  zMutex _lock;
  std::list<zTimerHandler *> _observers;
};

}

#endif /* __TIMER_H__ */
