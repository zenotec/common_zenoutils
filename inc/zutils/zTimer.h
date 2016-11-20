//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zTimer.h
//    Description:
//
//*****************************************************************************

#ifndef __TIMER_H__
#define __TIMER_H__

#include <list>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zEvent.h>

namespace zUtils
{
namespace zTimer
{

//**********************************************************************
// zTimer::Timer Class
//**********************************************************************
class Timer : public zEvent::Event, public zEvent::EventObserver
{

public:

  Timer();

  virtual
  ~Timer();

  void
  Start(uint32_t usec_);

  void
  Stop(void);

  bool
  Notify();

protected:

private:

  zSem::Mutex _lock;
  uint32_t _interval;
  sigevent_t _sigev;
  timer_t _timerid;
  uint64_t _tick;

  virtual bool
  EventHandler(const zEvent::EventNotification* notification_);

  static void
  timer_handler(union sigval sv_);

  virtual void
  _start(void);

  virtual void
  _stop(void);

};

//**********************************************************************
// zTimer::TimerNotification Class
//**********************************************************************

class TimerNotification : public zEvent::EventNotification
{
  friend Timer;

public:

  TimerNotification(Timer* timer_);

  virtual
  ~TimerNotification();

  uint64_t
  Tick();

protected:

  void
  tick(uint64_t tick_);

private:

  uint64_t _tick;

};

}
}

#endif /* __TIMER_H__ */
