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
// TimerObserver Class
//**********************************************************************
class Observer
{
public:
  virtual void
  TimerTick() = 0;

protected:
private:
};

//**********************************************************************
// Timer Class
//**********************************************************************
class Timer : public zEvent::Event
{
public:
  Timer(void);
  ~Timer(void);

  void
  Start(uint32_t usec_);
  void
  Stop(void);

  void
  Register(Observer *obs_);

  void
  Unregister(Observer *obs_);

  void
  Notify(void);

protected:

private:
  timer_t _timerid;

  void
  _start(void);
  void
  _stop(void);
  uint32_t _interval;

  zSem::Mutex _lock;
  std::list<Observer *> _observers;
};

}
}

#endif /* __TIMER_H__ */
