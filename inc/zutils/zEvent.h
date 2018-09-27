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

#ifndef __EVENT_H__
#define __EVENT_H__

#include <list>

#include <zutils/zSem.h>
#include <zutils/zQueue.h>

namespace zUtils
{
namespace zEvent
{

class Notification;
class Adapter;
class Handler;

//**********************************************************************
// Class: Event
//**********************************************************************

class Event
{

  friend class Handler;

public:

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
    TYPE_LAST
  };

  Event(Event::TYPE type_);

  virtual
  ~Event();

  Event::TYPE
  GetType() const;

protected:

  std::list<Handler*> _handler_list;

  bool
  registerHandler(Handler* handler_);

  bool
  unregisterHandler(Handler* handler_);

  bool
  notifyHandlers(SHARED_PTR(zEvent::Notification) noti_);

private:

  mutable zSem::Mutex _event_lock;
  Event::TYPE _type;

  Event(Event &other_);

  Event(const Event &other_);

};

//**********************************************************************
// Class: Notification
//**********************************************************************

class Notification
{

  friend Event;

public:

  virtual
  ~Notification();

  zEvent::Event::TYPE
  GetType() const;

  zEvent::Event&
  GetEvent() const;

protected:

  Notification(Event& event_);

private:

  zEvent::Event& _event;

};

//**********************************************************************
// Class: Observer
//**********************************************************************
class Observer
{
public:
  virtual bool
  ObserveEvent(SHARED_PTR(zEvent::Notification) n_) = 0;
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

  bool
  RegisterEvent(Event* event_);

  bool
  UnregisterEvent(Event* event_);

  bool
  RegisterObserver(Observer* obs_);

  bool
  UnregisterObserver(Observer* obs_);

protected:

  bool
  notifyObservers(SHARED_PTR(zEvent::Notification) noti_);

private:

  zSem::Mutex _event_lock;
  std::list<Event *> _event_list;
  std::list<Observer*> _obs_list;
  zQueue<SHARED_PTR(Notification)> _noti_queue;

  Handler(Handler const &);

  void
  operator=(Handler const &);

};

//**********************************************************************
// Class: Adapter
//**********************************************************************

class Adapter :
    public Event
{

public:

  Adapter(Event& event_);

  virtual
  ~Adapter();

protected:

  virtual SHARED_PTR(zEvent::Notification)
  AdaptEvent(SHARED_PTR(zEvent::Notification) noti_) = 0;

private:

  Event& _event;

  virtual bool
  ObserveEvent(SHARED_PTR(zEvent::Notification) noti_);

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
