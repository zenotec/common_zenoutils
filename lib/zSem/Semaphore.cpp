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

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <zutils/zSem.h>

namespace zUtils
{
namespace zSem
{

//*****************************************************************************
// Semaphore Class
//*****************************************************************************

Semaphore::Semaphore(const uint32_t value_) :
    _sem_lock(Mutex::LOCKED), _sem_cnt(value_)
{
  if (this->_sem_cnt == 0)
  {
    this->_empty_lock.lock();
  }
  this->_sem_lock.Unlock();
}

Semaphore::~Semaphore()
{
}

bool
Semaphore::Post()
{
  bool status = false;

  if (this->_sem_lock.Lock())
  {
    // Test for overflow
    if ((this->_sem_cnt + 1) > this->_sem_cnt)
    {
      if (this->_sem_cnt == 0)
      {
        this->_empty_lock.unlock();
      }
      this->_sem_cnt++;
    }
    status = true;
    this->_sem_lock.Unlock();
  }

  return (status);
}

bool
Semaphore::Wait()
{

  bool status = this->TryWait();

  if (!status)
  {
    this->_empty_lock.lock();
    this->_empty_lock.unlock();
    status = this->TryWait();
  }

  return (status);
}

bool
Semaphore::TryWait()
{
  bool status = false;
  if (this->_sem_lock.Lock())
  {
    if (this->_sem_cnt > 0)
    {
      if (this->_sem_cnt-- == 1)
      {
        this->_empty_lock.lock();
      }
      status = true;
    }
    this->_sem_lock.Unlock();
  }
  return (status);
}

bool
Semaphore::TimedWait(uint32_t msec_)
{
  bool status = this->TryWait();
  if (!status)
  {
    if (TIMED_LOCK(this->_empty_lock, msec_))
    {
      this->_empty_lock.unlock();
      status = this->TryWait();
    }
  }
  return (status);
}

bool
Semaphore::Reset()
{
  bool status = false;

  if (this->_sem_lock.Lock())
  {
    if (this->_sem_cnt != 0)
    {
      this->_sem_cnt = 0;
      this->_empty_lock.lock();
    }
    status = this->_sem_lock.Unlock();
  }

  return (status);
}

}
}
