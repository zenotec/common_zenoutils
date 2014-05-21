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

#include "zutils/zSemaphore.h"

namespace zUtils
{

class zEventList;

//**********************************************************************
// zEvent Class
//**********************************************************************
class zEvent
{
  friend class zEventList;
public:
  zEvent();
  virtual
  ~zEvent();

protected:
  void
  _notify();
  void
  _addList(zEventList *list_);
  void
  _remList(zEventList *list_);

private:
  zMutex _lock;
  std::list<zEventList *> _eventlists;

};

//**********************************************************************
// zEventList Class
//**********************************************************************
class zEventList
{
  friend class zEvent;
public:
  zEventList();
  virtual
  ~zEventList();

  void
  Register(zEvent &event_);
  void
  Unregister(zEvent &event_);

  bool
  Wait(uint32_t ms_);

protected:
  void
  _notify();

private:
  zSemaphore _sem;

};

}

#endif /* __ZEVENT_H__ */
