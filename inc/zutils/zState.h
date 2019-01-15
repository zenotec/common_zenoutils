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

  Notification(Context& ctx_);

  virtual
  ~Notification();

  uint32_t
  GetCommand() const;

  bool
  SetCommand(const uint32_t cmd_);

protected:

private:

  uint32_t _cmd;

};

//**********************************************************************
// Class: State
//**********************************************************************

class State :
    public zEvent::Observer
{

public:

  State(SHPTR(Context) context_, const uint32_t id_);

  virtual
  ~State();

  SHPTR(Context)
  GetContext();

  uint32_t
  GetId() const;

  virtual zEvent::STATUS
  ObserveEvent(SHPTR(zEvent::Notification) n_) = 0;

protected:

private:

  SHPTR(Context) _ctx;
  uint32_t _id;

};

//**********************************************************************
// Class: Context
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

  SHPTR(zState::State)
  GetLastState() const;

  bool
  SetLastState(SHPTR(zState::State) state_);

  uint32_t
  GetStateId() const;

  SHPTR(zState::State)
  GetState() const;

  bool
  SetState(SHPTR(zState::State) state_);

  uint32_t
  GetNextStateId() const;

  SHPTR(zState::State)
  GetNextState() const;

  bool
  SetNextState(SHPTR(zState::State) state_);

  zEvent::STATUS
  SetNextStateAndNotify(SHPTR(zState::State) state_, const uint32_t cmd_ = 0);

  zEvent::STATUS
  SetNextStateAndNotify(SHPTR(zState::State) state_, SHPTR(zEvent::Notification) n_);

  virtual zEvent::STATUS
  Notify(const uint32_t cmd_ = 0);

  virtual zEvent::STATUS
  Notify(SHPTR(zEvent::Notification) n_);

protected:

  virtual zEvent::STATUS
  ObserveEvent(SHPTR(zEvent::Notification) n_);

private:

  mutable zSem::Mutex _lock;
  SHPTR(zState::State) _last;
  SHPTR(zState::State) _state;
  SHPTR(zState::State) _next;

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
