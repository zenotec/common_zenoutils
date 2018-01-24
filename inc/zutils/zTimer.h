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
#include <zutils/zEvent.h>
#include <zutils/zThread.h>

namespace zUtils
{
namespace zTimer
{

class Handler;

//**********************************************************************
// Class: Timer
//**********************************************************************

class Timer : public zEvent::Event
{

  friend Handler;

public:

  Timer();

  virtual
  ~Timer();

  bool
  Start(uint32_t usec_);

  bool
  Stop(void);

  uint32_t
  GetId() const;

  uint64_t
  GetTicks() const;

protected:

  int _fd;

private:

  mutable zSem::Mutex _lock;
  uint32_t _interval; // micro-seconds
  uint64_t _ticks;

  bool
  _start(void);

  bool
  _stop(void);

};

//**********************************************************************
// Class: Notification
//**********************************************************************

class Notification : public zEvent::Notification
{
  friend Timer;

public:

  Notification(Timer& timer_);

  virtual
  ~Notification();

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
// Class: Handler
//**********************************************************************

class Handler :
    public zEvent::Handler,
    public zThread::ThreadFunction
{
public:

  Handler();

  virtual
  ~Handler();

  bool
  RegisterTimer(Timer* event_);

  bool
  UnregisterTimer(Timer* event_);

protected:

  virtual void
  Run(zThread::ThreadArg *arg_);

private:

  zSem::Mutex _lock;
  std::map<int, Timer*> _timer_list;
  zThread::Thread _thread;

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
