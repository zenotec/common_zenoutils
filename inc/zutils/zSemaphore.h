//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zSemaphore.h
//    Description:
//
//*****************************************************************************

#ifndef __ZSEMAPHORE_H__
#define __ZSEMAPHORE_H__

#include <stdint.h>
#include <semaphore.h>

namespace zUtils
{

//**********************************************************************
// zMutex Class
//**********************************************************************
class zMutex
{
public:
  enum state
  {
    LOCKED, UNLOCKED
  };

  zMutex(zMutex::state state_ = zMutex::LOCKED);
  ~zMutex();

  bool
  Lock();

  bool
  TryLock();

  bool
  TimedLock(uint32_t ms_);

  bool
  Unlock();

  zMutex::state
  State();

protected:

private:
  sem_t _sem;

};

//**********************************************************************
// Semaphore Class
//**********************************************************************
class zSemaphore
{
public:
  zSemaphore(const uint32_t value_ = 0);
  ~zSemaphore();

  bool
  Post(uint32_t value_ = 1);

  bool
  Wait();

  bool
  TryWait();

  bool
  TimedWait(uint32_t us_);

  uint32_t
  Value();

protected:

private:
  sem_t _sem;

};

}

#endif /* __ZSEMAPHORE_H__ */
