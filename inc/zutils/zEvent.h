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
    TYPE_LAST
  };

  Event (Event::TYPE type_ = TYPE_NONE, uint32_t id_ = 0);

  Event (Event &other_);

  virtual
  ~Event ();

  Event::TYPE
  GetType ();

  bool
  SetType (const Event::TYPE &type_);

  uint32_t
  GetId ();

  bool
  SetId (const uint32_t &id_);

  void
  Notify (void *arg_);

protected:

  void
  registerHandler (EventHandler *list_);

  void
  unregisterHandler (EventHandler *list_);

private:

  std::mutex _lock;
  std::list<EventHandler *> _handler_list;
  Event::TYPE _type;
  uint32_t _id;

};

//**********************************************************************
// Class: EventObserver
//**********************************************************************
class EventObserver
{
public:
  virtual bool
  EventHandler (Event *event_, void *arg_) = 0;
};

//**********************************************************************
// Class: EventHandler
//**********************************************************************
class EventHandler
{

  friend class Event;

public:
  EventHandler ();

  virtual
  ~EventHandler ();

  void
  RegisterEvent (Event *event_);

  void
  UnregisterEvent (Event *event_);

  bool
  RegisterObserver (EventObserver *obs_);

  bool
  UnregisterObserver (EventObserver *obs_);

protected:

  void
  notify (Event *event_, void *arg_);

private:

  std::mutex _lock;
  std::list<Event *> _event_list;
  std::list<EventObserver *> _obs_list;

  EventHandler (EventHandler const &);

  void
  operator= (EventHandler const &);

};

//**********************************************************************
// Class: EventManager
//**********************************************************************

class EventManager : public EventHandler
{
public:

  static EventManager&
  Instance ()
  {
    static EventManager instance;
    return instance;
  }

protected:

private:

  EventManager ()
  {
  }

  EventManager (EventManager const&);

  void
  operator= (EventManager const&);

};


}
}

#endif /* __EVENT_H__ */
