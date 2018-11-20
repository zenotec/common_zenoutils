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

class Handler;

//**********************************************************************
// Class: Notification
//**********************************************************************

class Notification :
    public zEvent::Notification
{

public:

  Notification(Handler& handler_);

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

  State(Handler& handler_, const uint32_t id_);

  virtual
  ~State();

  Handler&
  GetHandler();

  uint32_t
  GetId() const;

  virtual bool
  ObserveEvent(SHARED_PTR(zEvent::Notification) n_) = 0;

protected:

private:

  Handler& _handler;
  uint32_t _id;

};

//**********************************************************************
// Class: Handler
//**********************************************************************

class Handler :
    public zEvent::Event
{

public:

  Handler();

  virtual
  ~Handler();

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

  virtual bool
  Notify();

  virtual bool
  Notify(SHARED_PTR(zEvent::Notification) n_);

protected:

private:

  mutable zSem::Mutex _lock;
  SHARED_PTR(zState::State) _last;
  SHARED_PTR(zState::State) _state;
  SHARED_PTR(zState::State) _next;

};

//**********************************************************************
// Class: Manager
//**********************************************************************

class Manager : public Handler
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
