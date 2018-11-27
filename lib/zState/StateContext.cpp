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

// libc includes


// libc++ includes

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;
#include <zutils/zEvent.h>
#include <zutils/zState.h>

// local includes

ZLOG_MODULE_INIT(zLog::Log::MODULE_STATE);

namespace zUtils
{
namespace zState
{

//**********************************************************************
// Class: Context
//**********************************************************************

Context::Context() :
    zEvent::Event(zEvent::TYPE_STATE)
{
  this->_last = NULL;
  this->_state = NULL;
  this->_next = NULL;
  this->_lock.Unlock();
}

Context::~Context()
{
  this->_lock.Lock();
  // Explicitly delete all states before the context gets destructed
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

zEvent::STATUS
Context::SetNextStateAndNotify(SHARED_PTR(zState::State) state_)
{
  zEvent::STATUS status = zEvent::STATUS_ERR;
  if (this->SetNextState(state_))
  {
    status = this->Notify();
  }
  return (status);
}

zEvent::STATUS
Context::SetNextStateAndNotify(SHARED_PTR(zState::State) state_, SHARED_PTR(zEvent::Notification) n_)
{
  zEvent::STATUS status = zEvent::STATUS_ERR;
  if (this->SetNextState(state_))
  {
    status = this->Notify(n_);
  }
  return (status);
}

zEvent::STATUS
Context::Notify()
{
  SHARED_PTR(zEvent::Notification) n(new zState::Notification(*this));
  return (this->Notify(n));
}

zEvent::STATUS
Context::Notify(SHARED_PTR(zEvent::Notification) n_)
{
  zEvent::STATUS status = zEvent::STATUS_ERR;

  // Protect current state from destroying itself by changing state
  SHARED_PTR(zState::State) s(this->GetNextState());

  if (s.get())
  {
    this->SetLastState(this->GetState());
    this->SetState(this->GetNextState());
    ZLOG_DEBUG("State change: " + ZLOG_UINT(this->GetLastStateId()) + " -> " + ZLOG_UINT(this->GetStateId()));
    fprintf(stderr, "\n[%d] State: %d -> %d\n", __LINE__, this->GetLastStateId(), this->GetStateId());
    status = this->GetState()->ObserveEvent(n_);
    ZLOG_DEBUG("State status: " + ZLOG_UINT(status));
    fprintf(stderr, "[%d] Status: 0x%02x\n", __LINE__, status);
  }

  return (status);
}

zEvent::STATUS
Context::ObserveEvent(SHARED_PTR(zEvent::Notification) n_)
{
  return (this->Notify(n_));
}

}
}
