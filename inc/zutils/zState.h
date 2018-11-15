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
#include <zutils/zEvent.h>

namespace zUtils
{
namespace zState
{

class State;

//**********************************************************************
// Class: Notification
//**********************************************************************

class Notification
{

public:

  Notification();

  virtual
  ~Notification();

  SHARED_PTR(zState::State)
  GetState() const;

  bool
  SetState(SHARED_PTR(zState::State) state_);

  SHARED_PTR(zEvent::Notification)
  GetNotification();

  bool
  SetNotification(SHARED_PTR(zEvent::Notification) n_);

protected:

private:

  SHARED_PTR(zState::State) _state;
  SHARED_PTR(zEvent::Notification) _notification;

};

//**********************************************************************
// Class: State
//**********************************************************************

class State
{

public:

  State(const uint32_t id_);

  virtual
  ~State();

  uint32_t
  GetId() const;

  virtual bool
  ObserveEvent(SHARED_PTR(zState::Notification) n_) = 0;

protected:

private:

  uint32_t _id;

};

//**********************************************************************
// Class: Handler
//**********************************************************************

class Handler
{

public:

  Handler();

  virtual
  ~Handler();

  SHARED_PTR(zState::State)
  GetState() const;

  bool
  SetState(SHARED_PTR(zState::State) state_);

  virtual bool
  Notify(SHARED_PTR(zState::Notification) n_);

  bool
  SetStateAndNotify(SHARED_PTR(zState::State) s_, SHARED_PTR(zState::Notification) n_);

protected:

private:

  SHARED_PTR(zState::State) _state;

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
