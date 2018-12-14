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

#ifndef __EVENT_H__
#define __EVENT_H__

#include <list>

#include <zutils/zSem.h>

namespace zUtils
{
namespace zEvent
{

class Notification;
class Handler;

// Return status for event handlers
enum STATUS
{
  STATUS_NONE = 0x00, // Uninitialized
  STATUS_ERR = 0x01, // Error observing event notification
  STATUS_OK = 0x02, // Event observed successfully
  STATUS_CONT = 0x04, // Allow next observer to be called
  STATUS_OK_CONT = (STATUS_OK | STATUS_CONT),
  STATUS_ERR_CONT = (STATUS_ERR | STATUS_CONT),
  STATUS_STOP = 0x08, // Stop all event observation
  STATUS_OK_STOP = (STATUS_OK | STATUS_STOP),
  STATUS_ERR_STOP = (STATUS_ERR | STATUS_STOP),
  STATUS_LAST
};

enum TYPE
{
  TYPE_ERR = -1,
  TYPE_NONE = 0,
  TYPE_TEST = 1,
  TYPE_TIMER = 2,
  TYPE_SIGNAL = 3,
  TYPE_CONFIG = 4,
  TYPE_GPIO = 5,
  TYPE_SERIAL = 6,
  TYPE_INTERFACE = 7,
  TYPE_SOCKET = 8,
  TYPE_MSG = 9,
  TYPE_TEMP = 10,
  TYPE_COMMAND = 11,
  TYPE_STATE = 12,
  TYPE_RSVD_0 = 32,
  TYPE_RSVD_1 = 33,
  TYPE_RSVD_2 = 34,
  TYPE_RSVD_3 = 35,
  TYPE_RSVD_4 = 36,
  TYPE_RSVD_5 = 37,
  TYPE_RSVD_6 = 38,
  TYPE_RSVD_7 = 39,
  TYPE_LAST
};

//**********************************************************************
// Class: Event
//**********************************************************************

class Event
{

  friend class Handler;

public:

  Event(zEvent::TYPE type_);

  virtual
  ~Event();

  zEvent::TYPE
  GetType() const;

protected:

  virtual bool
  registerHandler(Handler* handler_);

  virtual bool
  unregisterHandler(Handler* handler_);

  zEvent::STATUS
  notifyHandlers(SHPTR(zEvent::Notification) n_);

  std::list<Handler*>
  getHandlers();

private:

  zSem::Mutex _hlock; // lock for handler list
  std::list<Handler*> _handlers;

  zSem::Mutex _nlock; // lock for notifying handlers

  zEvent::TYPE _type;

  Event(Event &other_);

  Event(const Event &other_);

};

//**********************************************************************
// Class: Notification
//**********************************************************************

class Notification
{

public:

  Notification(Event& event_);

  virtual
  ~Notification();

  zEvent::TYPE
  GetType() const;

  zEvent::Event&
  GetEvent() const;

protected:

private:

  zEvent::Event& _event;

};

//**********************************************************************
// Class: Observer
//**********************************************************************
class Observer
{
public:

  virtual zEvent::STATUS
  ObserveEvent(SHPTR(zEvent::Notification) n_) = 0;
};

//**********************************************************************
// Class: Handler
//**********************************************************************

class Handler
{

  friend class Event;

public:

  Handler();

  virtual
  ~Handler();

  virtual bool
  RegisterEvent(Event* event_);

  virtual bool
  UnregisterEvent(Event* event_);

  bool
  RegisterObserver(Observer* obs_);

  bool
  UnregisterObserver(Observer* obs_);

protected:

  zEvent::STATUS
  notifyObservers(SHPTR(zEvent::Notification) n_);

  std::list<Event*>
  getEvents();

  std::list<Observer*>
  getObservers();

private:

  std::list<Event *> _event_list;
  zSem::Mutex _elock; // lock for event list
  std::list<Observer*> _obs_list;
  zSem::Mutex _olock; // lock for observer list

  zSem::Mutex _nlock; // lock for notifying observers

  Handler(Handler const &);

  void
  operator=(Handler const &);

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

#endif /* __EVENT_H__ */
