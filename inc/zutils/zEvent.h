//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

#ifndef __EVENT_H__
#define __EVENT_H__

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
    TYPE_LAST
  };

  Event(Event::TYPE type_ = TYPE_NONE);

  virtual
  ~Event();

  Event::TYPE
  Type() const;

  void
  Notify(EventNotification* notification_);

protected:

  void
  registerHandler(EventHandler *list_);

  void
  unregisterHandler(EventHandler *list_);

private:

  mutable std::mutex _lock;
  std::list<EventHandler *> _handler_list;
  Event::TYPE _type;

  Event(Event &other_);
  Event(const Event &other_);

};

//**********************************************************************
// Class: Notification
//**********************************************************************

class EventNotification
{

  friend Event;

public:

  EventNotification();

  virtual
  ~EventNotification();

  Event::TYPE
  Type() const;

protected:

  bool
  type(const Event::TYPE type_);

private:

  Event::TYPE _type;

};

//**********************************************************************
// Class: EventObserver
//**********************************************************************
class EventObserver
{
public:
  virtual bool
  EventHandler(const EventNotification* notification_) = 0;
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

  void
  RegisterEvent(Event *event_);

  void
  UnregisterEvent(Event *event_);

  bool
  RegisterObserver(EventObserver *obs_);

  bool
  UnregisterObserver(EventObserver *obs_);

protected:

  void
  notify(const EventNotification* notification_);

private:

  std::mutex _lock;
  std::list<Event *> _event_list;
  std::list<EventObserver *> _obs_list;

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
