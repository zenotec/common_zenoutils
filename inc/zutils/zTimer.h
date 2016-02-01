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
// zTimer::TimerEvent Class
//**********************************************************************

class TimerEvent : public zEvent::Event
{
public:

  enum EVENTID
  {
    EVENTID_ERR = -1,
    EVENTID_NONE = 0,
    EVENTID_TICK = 1,
    EVENTID_LAST
  };

  TimerEvent(const TimerEvent::EVENTID id_);

  virtual
  ~TimerEvent();

protected:

private:

};

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

  zTimer::TimerEvent _tick_event;

  void
  _start(void);

  void
  _stop(void);

};

}
}

#endif /* __TIMER_H__ */
