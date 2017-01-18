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

#ifndef __ZTHREAD_H__
#define __ZTHREAD_H__

#include <string>
#include <thread>

#include <zutils/zEvent.h>

namespace zUtils
{
namespace zThread
{

class Thread;

//**********************************************************************
// Class: ThreadArg
//**********************************************************************

class ThreadArg
{
public:

protected:

private:

};

//**********************************************************************
// Class: ThreadFunction
//**********************************************************************

class ThreadFunction
{

  friend Thread;

public:

  ThreadFunction();

  virtual
  ~ThreadFunction();

  virtual void
  Run(zThread::ThreadArg *arg_) = 0;

  bool
  Exit();

  bool
  Exit(bool flag_);

protected:

  bool
  setThread(Thread* thread_);

private:

  zSem::Mutex _thread_lock;
  Thread *_thread;
  bool _exit;

};

//**********************************************************************
// Class: Thread
//**********************************************************************

class Thread : public zEvent::EventObserver
{
public:

  Thread(ThreadFunction *func_, ThreadArg *arg_);

  virtual
  ~Thread();

  void*
  Id();

  std::string
  Name();

  void
  Name(const std::string &name_);

  bool
  Start();

  bool
  Join();

  bool
  Stop();

protected:

  virtual bool
  EventHandler(zEvent::EventNotification* notification_);

private:

  std::thread *_thread;

  zEvent::EventHandler _sighandler;

  std::string _name;
  ThreadFunction *_func;
  ThreadArg *_arg;

};

}
}

#endif /* __ZTHREAD_H__ */
