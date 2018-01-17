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

#include <zutils/zSem.h>

namespace zUtils
{
namespace zSem
{

//*****************************************************************************
// zMutex Class
//*****************************************************************************

Mutex::Mutex(Mutex::STATE state_)
{
  if (state_ == Mutex::LOCKED)
  {
    this->_lock.lock();
  }
}

Mutex::~Mutex()
{
}

bool
Mutex::Lock()
{
  this->_lock.lock();
  return (true);
}

bool
Mutex::TryLock()
{
  return (this->_lock.try_lock());
}

bool
Mutex::TimedLock(uint32_t msec_)
{
  return (TIMED_LOCK(this->_lock, msec_));
}

bool
Mutex::Unlock()
{
  this->_lock.unlock();
  return (true);
}

}
}
