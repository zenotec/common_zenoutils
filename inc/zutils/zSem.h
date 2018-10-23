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

#ifndef __ZSEMAPHORE_H__
#define __ZSEMAPHORE_H__

#include <thread>

#include <zutils/zCompatibility.h>

namespace zUtils
{
namespace zSem
{

//**********************************************************************
// Mutex Class
//**********************************************************************
class Mutex
{
public:

  enum STATE
  {
    LOCKED, UNLOCKED
  };

  Mutex(Mutex::STATE state_ = Mutex::LOCKED);

  virtual
  ~Mutex();

  bool
  Lock();

  bool
  TryLock();

  bool
  TimedLock(uint32_t ms_);

  bool
  Unlock();

  Mutex::STATE
  State() const;

protected:

private:

  TIMED_MUTEX _mutex_lock;
  uint32_t _refcnt;
  std::thread::id _mutex_owner;

  Mutex(Mutex &other_);

  Mutex(const Mutex &other_);

  Mutex &
  operator=(Mutex &other_);

  Mutex &
  operator=(const Mutex &other_);

  void
  _set_owner(const std::thread::id tid_);

};

//**********************************************************************
// Semaphore Class
//**********************************************************************
class Semaphore
{
public:

  Semaphore(const uint64_t value_ = 0);

  virtual
  ~Semaphore();

  int
  GetFd() const;

  bool
  Post(const uint64_t value_ = 1) const;

  bool
  Wait() const;

  bool
  TryWait() const;

  bool
  TimedWait(int msec_) const;

  bool
  Reset();

protected:

private:

  Semaphore(Semaphore &other_);

  Semaphore(const Semaphore &other_);

  Semaphore &
  operator=(Semaphore &other_);

  Semaphore &
  operator=(const Semaphore &other_);

  int _fd;

};

}
}

#endif /* __ZSEMAPHORE_H__ */
