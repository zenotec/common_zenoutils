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
namespace zSem
{

//**********************************************************************
// Mutex Class
//**********************************************************************
class Mutex
{
public:
  enum STATE
  {
    LOCKED, UNLOCKED
  };

  Mutex(Mutex::STATE state_ = Mutex::LOCKED);
  ~Mutex();

  bool
  Lock();

  bool
  TryLock();

  bool
  TimedLock(uint32_t ms_);

  bool
  Unlock();

  Mutex::STATE
  State();

protected:

private:
  sem_t _sem;

};

//**********************************************************************
// Semaphore Class
//**********************************************************************
class Semaphore
{
public:
  Semaphore(const uint32_t value_ = 0);
  ~Semaphore();

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
}

#endif /* __ZSEMAPHORE_H__ */
