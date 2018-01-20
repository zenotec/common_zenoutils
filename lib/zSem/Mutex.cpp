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
    _owner(std::thread::id(0)), _cnt(0)
{
  if (state_ == Mutex::LOCKED)
  {
    this->_lock.lock();
    this->_owner = std::this_thread::get_id();
    this->_cnt++;
  }
}

Mutex::~Mutex()
{
}

bool
Mutex::Lock()
{
  std::thread::id tid = std::this_thread::get_id();
  if (this->_owner != tid)
  {
    this->_lock.lock();
    this->_owner = std::this_thread::get_id();
  }
  else
  {
    // TODO: Debug code to be removed later
    std::cerr << "BUG: Deadlock avoided" << std::endl;
  }
  this->_cnt++;
  return (true);
}

bool
Mutex::TryLock()
{
  bool status = this->_lock.try_lock();
  if (status)
  {
    this->_owner = std::this_thread::get_id();
    this->_cnt++;
  }
  return (status);
}

bool
Mutex::TimedLock(uint32_t msec_)
{
  bool status = TIMED_LOCK(this->_lock, msec_);
  if (status)
  {
    this->_owner = std::this_thread::get_id();
    this->_cnt++;
  }
  return (status);}

bool
Mutex::Unlock()
{
  bool status = false;
  std::thread::id tid = std::this_thread::get_id();
  if (this->_owner == tid)
  {
    if (this->_cnt-- == 1)
    {
      this->_owner = std::thread::id(0);
      this->_lock.unlock();
    }
    status = true;
  }
  return (status);
}

Mutex::STATE
Mutex::State() const
{
  return ((this->_cnt > 0) ? Mutex::LOCKED : Mutex::UNLOCKED);
}


}
}
