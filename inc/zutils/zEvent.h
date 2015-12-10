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

#include <zutils/zLog.h>
#include <zutils/zSem.h>

namespace zUtils
{
namespace zEvent
{

class EventHandler;

//**********************************************************************
// Event Class
//**********************************************************************
class Event
{
  friend class EventHandler;

public:

  Event();

  virtual
  ~Event();

  void
  Notify();

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
  EventHandler(EventHandler *handler_) = 0;
};

//**********************************************************************
// EventHandler Class
//**********************************************************************
class EventHandler : private std::queue <Event *>, public zSem::Semaphore
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

  Event *
  GetEvent();

  bool
  Empty();

  size_t
  Size();

protected:

  void
  notify(Event *event_);

private:

  zSem::Mutex _lock;
  std::list <Event *> _event_list;
  std::list <EventObserver *> _obs_list;

};

}
}

#endif /* __ZEVENT_H__ */
