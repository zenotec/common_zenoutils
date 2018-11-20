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
// Class: Handler
//**********************************************************************

Handler::Handler() :
    zEvent::Event(zEvent::Event::TYPE_STATE)
{
  this->_lock.Unlock();
}

Handler::~Handler()
{
  this->_lock.Lock();
}

uint32_t
Handler::GetLastStateId() const
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
Handler::GetLastState() const
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
Handler::SetLastState(SHARED_PTR(zState::State) state_)
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
Handler::GetStateId() const
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
Handler::GetState() const
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
Handler::SetState(SHARED_PTR(zState::State) state_)
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
Handler::GetNextStateId() const
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
Handler::GetNextState() const
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
Handler::SetNextState(SHARED_PTR(zState::State) state_)
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
Handler::Notify()
{
  SHARED_PTR(zState::Notification) n(new zState::Notification(*this));
  return (this->Notify(n));
}

bool
Handler::Notify(SHARED_PTR(zEvent::Notification) n_)
{
  bool status = false;

  // Guarantees current state does not get destructed
  SHARED_PTR(zState::State) s(this->GetNextState());

  if (s.get())
  {
    this->SetLastState(this->GetState());
    this->SetState(this->GetNextState());
    status = this->GetState()->ObserveEvent(n_);
  }

  return (status);
}

}
}
