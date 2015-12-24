//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zEvent.h
//    Description:
//
//*****************************************************************************

#ifndef __ZEVENT_H__
#define __ZEVENT_H__

#include <list>
#include <queue>
#include <vector>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zData.h>

namespace zUtils
{
namespace zEvent
{

class EventHandler;

//**********************************************************************
// Event Class
//**********************************************************************
class Event : zData::Data
{

  friend class EventHandler;

  static const std::string STR_ROOT;
  static const std::string STR_TYPE;
  static const std::string STR_TYPE_NONE;
  static const std::string STR_TYPE_TEST;
  static const std::string STR_TYPE_COM;
  static const std::string STR_TYPE_TEMP;
  static const std::string STR_TYPE_GPIO;
  static const std::string STR_TYPE_SOCKET;
  static const std::string STR_TYPE_MSG;
  static const std::string STR_ID;

public:

  enum TYPE
  {
    TYPE_ERR = -1,
    TYPE_NONE = 0,
    TYPE_TEST = 1,
    TYPE_COM = 2,
    TYPE_TEMP = 3,
    TYPE_GPIO = 4,
    TYPE_SOCKET = 5,
    TYPE_MSG = 6,
    TYPE_LAST
  };

  Event(Event::TYPE type_ = TYPE_NONE, uint32_t id_ = 0);

  virtual
  ~Event();

  Event::TYPE
  GetType() const;

  bool
  SetType(const Event::TYPE &type_);

  uint32_t
  GetId();

  bool
  SetId(const uint32_t &id_);

  void
  Notify(void *arg_);

protected:

  void
  registerHandler(EventHandler *list_);

  void
  unregisterHandler(EventHandler *list_);

private:

  zSem::Mutex _lock;
  std::list<EventHandler *> _handler_list;

};

//**********************************************************************
// EventObserver Class
//**********************************************************************
class EventObserver
{
public:
  virtual bool
  EventHandler(Event *event_, void *arg_) = 0;
};

//**********************************************************************
// EventHandler Class
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

  void
  RegisterObserver(EventObserver *obs_);

  void
  UnregisterObserver(EventObserver *obs_);

protected:

  void
  notify(Event *event_, void *arg_);

private:

  zSem::Mutex _lock;
  std::list<Event *> _event_list;
  std::list<EventObserver *> _obs_list;

  EventHandler(EventHandler const &);

  void
  operator=(EventHandler const &);

};

class EventManager
{

public:

  static EventManager &
  GetInstance();

  virtual
  ~EventManager();

  bool
  RegisterEvent(Event *event_);

  bool
  UnregisterEvent(Event *event_);

  bool
  RegisterObserver(Event::TYPE type_, EventObserver *obs_);

  bool
  UnregisterObserver(Event::TYPE type_, EventObserver *obs_);

protected:

private:

  std::vector<EventHandler> _handlers;

  EventManager();

  EventManager(const EventManager &);

  EventManager &
  operator=(const EventManager &);

};

}
}

#endif /* __ZEVENT_H__ */
