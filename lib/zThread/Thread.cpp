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
#include <poll.h>

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
    _lock(zSem::Mutex::LOCKED)
{
  int fd = 0;

  // Automatically add the exit semaphore to the poll file descriptors
  fd = this->_exit.GetFd();
  this->_fds[fd].fd = fd;
  this->_fds[fd].events = (POLLIN || POLLERR);
  this->_fds[fd].revents = 0;

  // Automatically add the reload semaphore to the poll file descriptors
  fd = this->_reload.GetFd();
  this->_fds[fd].fd = fd;
  this->_fds[fd].events = (POLLIN || POLLERR);
  this->_fds[fd].revents = 0;

  this->_lock.Unlock();
}

ThreadFunction::~ThreadFunction()
{
  this->_lock.Lock();
  this->_fds.clear();
}

bool
ThreadFunction::RegisterFd(const int fd_, const short int events_)
{
  this->_fds[fd_].fd = fd_;
  this->_fds[fd_].events = events_;
  this->_fds[fd_].revents = 0;
  return (this->_reload.Post());
}

bool
ThreadFunction::UnregisterFd(const int fd_)
{
  this->_fds.erase(fd_);
  return (this->_reload.Post());
}

bool
ThreadFunction::IsReload(const int fd_)
{
  return (fd_ == this->_reload.GetFd());
}

bool
ThreadFunction::Exit()
{
  bool flag = false;
  if (this->_lock.Lock())
  {
    flag = this->_exit.TryWait();
    this->_lock.Unlock();
  }
  return (flag);
}

bool
ThreadFunction::Exit(const bool flag_)
{
  bool flag = false;
  if (flag_ && this->_lock.Lock())
  {
    flag = this->_exit.Post();
    this->_lock.Unlock();
  }
  return (flag);
}

bool
ThreadFunction::IsExit(const int fd_)
{
  return (fd_ == this->_exit.GetFd());
}

int
ThreadFunction::Poll(std::vector<struct pollfd>& fds_, const int timeout_)
{

  // Construct poll file descriptor array
  std::vector<struct pollfd> fds;
  FOREACH (auto& fd, this->_fds)
  {
    fds.emplace_back(fd.second);
  }

  // Poll on file descriptors (note: includes the reload and exit semaphore file descriptors)
  int ret = poll(fds.data(), fds.size(), -1);
  if (ret > 0)
  {
    FOREACH (auto& fd, fds)
    {
      if (this->IsReload(fd.fd))
      {
        this->_reload.GetCount(); // need to read the counter to clear
      }
      if (this->IsExit(fd.fd))
      {
        this->_exit.GetCount(); // need to read the counter to clear
      }
    }
    fds_ = fds;
  }

  return (ret);
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
  if (!this->_thread && this->_func)
  {
    zSignal::Manager::Instance().RegisterObserver(zSignal::Signal::ID_SIGTERM, this);
    zSignal::Manager::Instance().RegisterObserver(zSignal::Signal::ID_SIGINT, this);
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
Thread::ObserveEvent(SHPTR(zEvent::Notification) n_)
{
  zEvent::STATUS status = zEvent::STATUS_ERR;

  if (n_ && (n_->GetType() == zEvent::TYPE_SIGNAL))
  {
    SHPTR(zSignal::Notification) n(STATIC_CAST(zSignal::Notification)(n_));
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
