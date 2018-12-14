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

#include <stdint.h>

#include <map>

#include <zutils/zSem.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>
#include <zutils/zThread.h>

namespace zUtils
{
namespace zTimer
{

class Handler;

//**********************************************************************
// Class: zTimer::Timer
//**********************************************************************

class Timer : public zEvent::Event
{

  friend Handler;

public:

  Timer(const std::string& name_ = "", const uint32_t id_ = 0);

  virtual
  ~Timer();

  bool
  Start(uint32_t usec_);

  bool
  Stop(void);

  int
  GetFd() const;

  uint64_t
  GetTicks() const;

  std::string
  Name() const;

  bool
  Name(const std::string name_);

  uint32_t
  GetId();

  bool
  SetId(const uint32_t id_);

protected:

private:

  mutable zSem::Mutex _lock;
  uint32_t _id;
  int _fd;
  uint32_t _interval; // micro-seconds
  std::string _name;

  bool
  _start(void);

  bool
  _stop(void);

};

//**********************************************************************
// Class: zTimer::Notification
//**********************************************************************

class Notification : public zEvent::Notification
{
  friend Timer;

public:

  Notification(Timer& timer_);

  virtual
  ~Notification();

  Timer&
  GetTimer();

  uint32_t
  GetId() const;

  uint64_t
  GetTicks() const;

protected:

private:

  uint32_t _id;
  uint64_t _tick;

};

//**********************************************************************
// Class: zTimer::NotificationThread
//**********************************************************************

class NotificationThread :
    public zThread::ThreadFunction
{

public:

protected:

  virtual void
  Run(zThread::ThreadArg *arg_);

private:

};

//**********************************************************************
// Class: zTimer::Handler
//**********************************************************************

class Handler :
    public zEvent::Handler,
    public zThread::ThreadFunction,
    public zThread::ThreadArg
{

  friend NotificationThread;

public:

  Handler();

  virtual
  ~Handler();

  virtual bool
  RegisterEvent(SHPTR(Timer) event_);

  virtual bool
  UnregisterEvent(SHPTR(Timer) event_);

protected:

  zQueue::Queue<SHPTR(zEvent::Notification)> nq; // notification queue

  virtual void
  Run(zThread::ThreadArg *arg_);

private:

  zSem::Mutex _lock;
  std::map<int, SHPTR(Timer)> _timers; // key is timer id

  zSem::Semaphore _reload;

  zThread::Thread _timer_thread;

  zThread::Thread _notification_thread;
  NotificationThread _notification_func;

};

//**********************************************************************
// Class: Manager
//**********************************************************************

class Manager : public Handler
{
public:

  static Manager&
  Instance()
  {
    static Manager instance;
    return instance;
  }

protected:

private:

  Manager()
  {
  }

  Manager(Manager const&);

  void
  operator=(Manager const&);

};


}
}

#endif /* __TIMER_H__ */
