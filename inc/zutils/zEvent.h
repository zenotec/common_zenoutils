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

#include <string>
#include <list>

#include <zutils/zSem.h>

namespace zUtils
{
namespace zEvent
{

class EventNotification;
class EventHandler;

//**********************************************************************
// Class: Event
//**********************************************************************

class Event
{

  friend class EventHandler;

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
    TYPE_SOCKET = 7,
    TYPE_MSG = 8,
    TYPE_TEMP = 9,
    TYPE_LAST
  };

  Event(Event::TYPE type_);

  virtual
  ~Event();

  Event::TYPE
  Type() const;

  void
  Notify(EventNotification* notification_);

protected:

  bool
  registerHandler(EventHandler *list_);

  bool
  unregisterHandler(EventHandler *list_);

private:

  mutable zSem::Mutex _event_lock;
  std::list<EventHandler *> _handler_list;
  Event::TYPE _type;

  Event(Event &other_);

  Event(const Event &other_);

};

//**********************************************************************
// Class: EventNotification
//**********************************************************************

class EventNotification
{

  friend Event;

public:

  EventNotification(zEvent::Event::TYPE type_ = zEvent::Event::TYPE_NONE);

  EventNotification(zEvent::Event* event_);

  virtual
  ~EventNotification();

  zEvent::Event::TYPE
  Type() const;

  zEvent::Event*
  GetEvent() const;

protected:

  void
  type(zEvent::Event::TYPE type_);

private:

  zEvent::Event::TYPE _type;
  zEvent::Event *_event;

};

//**********************************************************************
// Class: EventObserver
//**********************************************************************
class EventObserver
{
public:
  virtual bool
  EventHandler(const zEvent::EventNotification* notification_) = 0;
};

//**********************************************************************
// Class: EventHandler
//**********************************************************************
class EventHandler
{

  friend class Event;

public:
  EventHandler();

  virtual
  ~EventHandler();

  bool
  RegisterEvent(Event* event_);

  bool
  UnregisterEvent(Event* event_);

  bool
  RegisterObserver(EventObserver* obs_);

  bool
  UnregisterObserver(EventObserver* obs_);

protected:

  void
  notify(const EventNotification* notification_);

private:

  zSem::Mutex _event_lock;
  std::list<Event *> _event_list;
  std::list<EventObserver*> _obs_list;

  EventHandler(EventHandler const &);

  void
  operator=(EventHandler const &);

};

//**********************************************************************
// Class: EventManager
//**********************************************************************

class EventManager : public EventHandler
{
public:

  static EventManager&
  Instance()
  {
    static EventManager instance;
    return instance;
  }

protected:

private:

  EventManager()
  {
  }

  EventManager(EventManager const&);

  void
  operator=(EventManager const&);

};

}
}

#endif /* __EVENT_H__ */
