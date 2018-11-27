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

#include <string.h>

#include <mutex>
#include <list>
#include <map>

#include <zutils/zSem.h>
#include <zutils/zEvent.h>
#include <zutils/zSignal.h>
#include <zutils/zThread.h>

namespace zUtils
{
namespace zThread
{

//**********************************************************************
// Class: ThreadFunction
//**********************************************************************

ThreadFunction::ThreadFunction() :
    _thread_lock(zSem::Mutex::LOCKED), _thread(NULL), _yield(false), _exit(false)
{
  this->_thread_lock.Unlock();
}

ThreadFunction::~ThreadFunction()
{
  this->_thread_lock.Lock();
}

bool
ThreadFunction::Yield()
{
  bool flag = false;
  if (this->_thread_lock.Lock())
  {
    flag = this->_yield;
    this->_thread_lock.Unlock();
  }
  return (flag);
}

bool
ThreadFunction::Yield(bool flag_)
{
  bool status = false;
  if (this->_thread_lock.Lock())
  {
    this->_yield = flag_;
    status = true;
    this->_thread_lock.Unlock();
  }
  return (status);
}

bool
ThreadFunction::Exit()
{
  bool flag = false;
  if (this->_thread_lock.Lock())
  {
    flag = this->_exit;
    this->_thread_lock.Unlock();
  }
  return (flag);
}

bool
ThreadFunction::Exit(bool flag_)
{
  bool status = false;
  if (this->_thread_lock.Lock())
  {
    this->_exit = flag_;
    status = true;
    this->_thread_lock.Unlock();
  }
  return (status);
}

bool
ThreadFunction::setThread(Thread* thread_)
{
  bool status = false;
  if (thread_)
  {
    this->_thread = thread_;
    status = true;
  }
  return(status);
}

void
ThreadFunction::yield() const
{
  std::this_thread::yield();
}

//*****************************************************************************
// Class Thread
//*****************************************************************************
Thread::Thread(ThreadFunction *func_, ThreadArg *arg_) :
    _thread(NULL), _func(func_), _arg(arg_)
{
}

Thread::~Thread()
{
  // Terminate listener thread
  this->Stop();
}

void*
Thread::Id()
{
  return (this);
}

std::string
Thread::Name()
{
  return (this->_name);
}

void
Thread::Name(const std::string &name_)
{
  this->_name = name_;
  return;
}

bool
Thread::Start()
{
  bool status = false;
  if (!this->_thread && this->_func && this->_func->setThread(this))
  {
    zSignal::Manager::Instance().RegisterObserver(zSignal::Signal::ID_SIGTERM, this);
    zSignal::Manager::Instance().RegisterObserver(zSignal::Signal::ID_SIGINT, this);
    this->_func->Exit(false);
    this->_func->Yield(false);
    this->_thread = new std::thread(&ThreadFunction::Run, this->_func, this->_arg);
    status = !!this->_thread;
  }
  return (status);
}

bool
Thread::Join()
{
  bool status = false;
  if (this->_func && this->_thread && this->_thread->joinable())
  {
    this->_thread->join();
    delete (this->_thread);
    this->_thread = NULL;
    zSignal::Manager::Instance().UnregisterObserver(zSignal::Signal::ID_SIGTERM, this);
    zSignal::Manager::Instance().UnregisterObserver(zSignal::Signal::ID_SIGINT, this);
    status = true;
  }
  return (status);
}

bool
Thread::Stop()
{
  bool status = false;
  if (this->_func && this->_thread)
  {
    if (this->_func->Exit(true))
    {
      status = this->Join();
    }
  }
  return (status);
}

zEvent::STATUS
Thread::ObserveEvent(SHARED_PTR(zEvent::Notification) n_)
{
  zEvent::STATUS status = zEvent::STATUS_ERR;

  if (n_ && (n_->GetType() == zEvent::TYPE_SIGNAL))
  {
    SHARED_PTR(zSignal::Notification) n(STATIC_CAST(zSignal::Notification)(n_));
    switch (n->Id())
    {
    case zSignal::Signal::ID_SIGTERM:
      // No break
    case zSignal::Signal::ID_SIGINT:
      // No break
    case zSignal::Signal::ID_SIGABRT:
      this->_func->Exit(true);
      status = zEvent::STATUS_CONT;
      break;
    default:
      break;
    }
  }
  return (status);
}

}
}
