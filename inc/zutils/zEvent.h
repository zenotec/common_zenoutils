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

#include <zutils/zLog.h>
#include <zutils/zSem.h>

namespace zUtils
{
namespace zEvent
{

class EventList;

//**********************************************************************
// Event Class
//**********************************************************************
class Event
{
  friend class EventList;
public:
  Event();
  virtual
  ~Event();

protected:
  void
  _notify();
  void
  _addList(EventList *list_);
  void
  _remList(EventList *list_);

private:
  zSem::Mutex _lock;
  std::list<EventList *> _eventlists;

};

//**********************************************************************
// EventList Class
//**********************************************************************
class EventList
{
  friend class Event;
public:
  EventList();
  virtual
  ~EventList();

  void
  Register(Event *event_);
  void
  Unregister(Event *event_);

  bool
  Wait(uint32_t ms_);

protected:
  void
  _notify();

private:
  zSem::Semaphore _sem;

};

}
}

#endif /* __ZEVENT_H__ */
