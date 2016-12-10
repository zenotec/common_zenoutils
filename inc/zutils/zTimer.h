/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
