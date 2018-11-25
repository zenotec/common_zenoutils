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
#include <signal.h>

#include <mutex>
#include <list>
#include <map>

#include <zutils/zEvent.h>
#include <zutils/zState.h>

namespace zUtils
{
namespace zState
{

//**********************************************************************
// Class: Context
//**********************************************************************

Context::Context() :
    zEvent::Event(zEvent::Event::TYPE_STATE)
{
  this->_last = NULL;
  this->_state = NULL;
  this->_next = NULL;
  this->_lock.Unlock();
}

Context::~Context()
{
  this->_lock.Lock();
  this->_next = NULL;
  this->_state = NULL;
  this->_last = NULL;
}

uint32_t
Context::GetLastStateId() const
{
  uint32_t id = 0;
  SHARED_PTR(zState::State)s(this->GetLastState());
  if (s.get())
  {
    id = s->GetId();
  }
  return (id);
}

SHARED_PTR(zState::State)
Context::GetLastState() const
{
  SHARED_PTR(zState::State) s;
  if (this->_lock.Lock())
  {
    s = this->_last;
    this->_lock.Unlock();
  }
  return (s);
}

bool
Context::SetLastState(SHARED_PTR(zState::State) state_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    this->_last = state_;
    status = this->_lock.Unlock();
  }
  return (status);
}

uint32_t
Context::GetStateId() const
{
  uint32_t id = 0;
  SHARED_PTR(zState::State)s(this->GetState());
  if (s.get())
  {
    id = s->GetId();
  }
  return (id);
}

SHARED_PTR(zState::State)
Context::GetState() const
{
  SHARED_PTR(zState::State) s;
  if (this->_lock.Lock())
  {
    s = this->_state;
    this->_lock.Unlock();
  }
  return (s);
}

bool
Context::SetState(SHARED_PTR(zState::State) state_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    this->_state = state_;
    status = this->_lock.Unlock();
  }
  return (status);
}

uint32_t
Context::GetNextStateId() const
{
  uint32_t id = 0;
  SHARED_PTR(zState::State)s(this->GetNextState());
  if (s.get())
  {
    id = s->GetId();
  }
  return (id);
}

SHARED_PTR(zState::State)
Context::GetNextState() const
{
  SHARED_PTR(zState::State) s;
  if (this->_lock.Lock())
  {
    s = this->_next;
    this->_lock.Unlock();
  }
  return (s);
}

bool
Context::SetNextState(SHARED_PTR(zState::State) state_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    this->_next = state_;
    status = this->_lock.Unlock();
  }
  return (status);
}

bool
Context::SetNextStateAndNotify(SHARED_PTR(zState::State) state_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    this->_next = state_;
    status = this->_lock.Unlock();
  }
  return (status && this->Notify());
}

bool
Context::Notify()
{
  SHARED_PTR(zState::Notification) n(new zState::Notification(*this));
  return (this->Notify(n));
}

bool
Context::Notify(SHARED_PTR(zEvent::Notification) n_)
{
  bool status = false;

  // Guarantees current state does not get destructed
  SHARED_PTR(zState::State) s(this->GetNextState());

  if (s.get())
  {
    this->SetLastState(this->GetState());
    this->SetState(this->GetNextState());
    fprintf(stderr, "\n[%d] State: %d -> %d\n", __LINE__, this->GetLastStateId(), this->GetStateId());
    status = this->GetState()->ObserveEvent(n_);
    fprintf(stderr, "[%d] Status: %d\n", __LINE__, status);
  }

  return (status);
}

bool
Context::ObserveEvent(SHARED_PTR(zEvent::Notification) n_)
{
  return (this->Notify(n_));
}

}
}
