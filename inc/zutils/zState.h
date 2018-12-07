/*
 * Copyright (c) 2014-2018 ZenoTec LLC (http://www.zenotec.net)
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

#ifndef __ZSTATE_H__
#define __ZSTATE_H__

#include <zutils/zCompatibility.h>
#include <zutils/zSem.h>
#include <zutils/zEvent.h>

namespace zUtils
{
namespace zState
{

class Context;

//**********************************************************************
// Class: Notification
//**********************************************************************

class Notification :
    public zEvent::Notification
{

public:

  Notification(Context& handler_);

  virtual
  ~Notification();

protected:

private:


};

//**********************************************************************
// Class: State
//**********************************************************************

class State :
    public zEvent::Observer
{

public:

  State(SHARED_PTR(Context) context_, const uint32_t id_);

  virtual
  ~State();

  SHARED_PTR(Context)
  GetContext();

  uint32_t
  GetId() const;

  virtual zEvent::STATUS
  ObserveEvent(SHARED_PTR(zEvent::Notification) n_) = 0;

protected:

private:

  SHARED_PTR(Context) _ctx;
  uint32_t _id;

};

//**********************************************************************
// Class: Handler
//**********************************************************************

class Context :
    public zEvent::Event,
    public zEvent::Observer
{

public:

  Context();

  virtual
  ~Context();

  uint32_t
  GetLastStateId() const;

  SHARED_PTR(zState::State)
  GetLastState() const;

  bool
  SetLastState(SHARED_PTR(zState::State) state_);

  uint32_t
  GetStateId() const;

  SHARED_PTR(zState::State)
  GetState() const;

  bool
  SetState(SHARED_PTR(zState::State) state_);

  uint32_t
  GetNextStateId() const;

  SHARED_PTR(zState::State)
  GetNextState() const;

  bool
  SetNextState(SHARED_PTR(zState::State) state_);

  zEvent::STATUS
  SetNextStateAndNotify(SHARED_PTR(zState::State) state_);

  zEvent::STATUS
  SetNextStateAndNotify(SHARED_PTR(zState::State) state_, SHARED_PTR(zEvent::Notification) n_);

  virtual zEvent::STATUS
  Notify();

  virtual zEvent::STATUS
  Notify(SHARED_PTR(zEvent::Notification) n_);

protected:

  virtual zEvent::STATUS
  ObserveEvent(SHARED_PTR(zEvent::Notification) n_);

private:

  mutable zSem::Mutex _lock;
  SHARED_PTR(zState::State) _last;
  SHARED_PTR(zState::State) _state;
  SHARED_PTR(zState::State) _next;

};

//**********************************************************************
// Class: Manager
//**********************************************************************

class Manager : public Context
{
public:

  static Manager&
  Instance()
  {
    static Manager instance;
    return instance;
  }

protected:

private:

  Manager()
  {
  }

  Manager(Manager const&);

  void
  operator=(Manager const&);


};

}
}

#endif /* __ZSTATE_H__ */
