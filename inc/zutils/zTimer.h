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
class Timer : public zEvent::EventHandler
{

public:

  Timer();

  virtual
  ~Timer();

  void
  Start(uint32_t usec_);

  void
  Stop(void);

protected:

  static void
  timer_handler(union sigval sv_);

private:

  zSem::Mutex _lock;
  uint32_t _interval;
  struct sigevent *_sigev;
  timer_t _timerid;
  uint64_t _tick;

  zEvent::Event _event;

  void
  _start(void);

  void
  _stop(void);

};

//**********************************************************************
// zTimer::TimerNotification Class
//**********************************************************************

class TimerNotification : public zEvent::EventNotification
{
public:

  TimerNotification(const uint64_t tick_);

  virtual
  ~TimerNotification();

  uint64_t
  Tick();

protected:

private:

  uint64_t _tick;

};

}
}

#endif /* __TIMER_H__ */
