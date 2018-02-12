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

#include <iostream>
#include <thread>

#include <zutils/zSem.h>

namespace zUtils
{
namespace zSem
{

//*****************************************************************************
// zMutex Class
//*****************************************************************************

Mutex::Mutex(Mutex::STATE state_) :
    _mutex_owner(std::thread::id(0)), _refcnt(0)
{
  if (state_ == Mutex::LOCKED)
  {
    this->_mutex_lock.lock();
    this->_set_owner(std::this_thread::get_id());
    this->_refcnt++;
  }
}

Mutex::~Mutex()
{
}

bool
Mutex::Lock()
{
  std::thread::id tid = std::this_thread::get_id();
  if (this->_mutex_owner != tid)
  {
    this->_mutex_lock.lock();
    this->_set_owner(tid);
  }
  else
  {
    // TODO: Debug code to be removed later
    fprintf(stderr, "(%p) BUG: Deadlock avoided: %p\n", this, tid);
  }
  this->_refcnt++;
  return (true);
}

bool
Mutex::TryLock()
{
  bool status = this->_mutex_lock.try_lock();
  if (status)
  {
    this->_set_owner(std::this_thread::get_id());
    this->_refcnt++;
  }
  return (status);
}

bool
Mutex::TimedLock(uint32_t msec_)
{
  std::thread::id tid = std::this_thread::get_id();
  bool status = TIMED_LOCK(this->_mutex_lock, msec_);
  if (status)
  {
    this->_set_owner(tid);
    this->_refcnt++;
  }
  return (status);}

bool
Mutex::Unlock()
{
  bool status = false;
  std::thread::id tid = std::this_thread::get_id();
  if (this->_mutex_owner == tid)
  {
    if (this->_refcnt-- == 1)
    {
      this->_set_owner(std::thread::id(0));
      this->_mutex_lock.unlock();
    }
    status = true;
  }
  else if (this->_mutex_owner != std::thread::id(0))
  {
    // TODO: Debug code to be removed later
    fprintf(stderr, "(%p) BUG: Unlock not by owner: %p\n", this, tid);
  }
  return (status);
}

Mutex::STATE
Mutex::State() const
{
  return ((this->_refcnt > 0) ? Mutex::LOCKED : Mutex::UNLOCKED);
}

void
Mutex::_set_owner(const std::thread::id tid_)
{
  this->_mutex_owner = tid_;
}

}
}
