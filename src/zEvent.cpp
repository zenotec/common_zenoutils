/*
 * Select.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: kmahoney
 */

#include <sstream>
#include <string.h>
#include <errno.h>

#include "zutils/zLog.h"
#include "zutils/zEvent.h"

namespace zUtils
{

//**********************************************************************
// Event
//**********************************************************************

zEvent::zEvent() :
    _fd(0), _cnt(0)
{
  // Initialize lock
  int ret = sem_init(&this->_sem, 0, 1);
  if (ret != 0)
  {
    ZLOG_CRIT("Event: Cannot initialize lock: " + zLog::IntStr(ret));
    throw;
  } // end if
}

zEvent::~zEvent()
{
  // Destroy lock
  int ret = sem_destroy(&this->_sem);
  if (ret != 0)
  {
    ZLOG_CRIT("Event: Cannot destroy lock: " + zLog::IntStr(ret));
    throw;
  } // end if
}

int
zEvent::GetFd()
{
  this->_lock();
  int fd = this->_fd;
  this->_unlock();
  return (fd);
}

uint64_t
zEvent::GetPending()
{
  this->_lock();
  uint64_t cnt = this->_cnt;
  this->_unlock();
  return (cnt);
}

void
zEvent::_setFd(const int fd_)
{
  this->_lock();
  this->_fd = fd_;
  this->_unlock();
}

void
zEvent::_notify(const uint64_t &cnt_)
{
  // Lock
  this->_lock();

  // Conditionally increment count
  if ((this->_cnt + cnt_) > this->_cnt)
  {
    this->_cnt += cnt_;
  } // end if

  // Unlock
  this->_unlock();
}

uint64_t
zEvent::_acknowledge(const uint64_t &cnt_)
{
  // Lock
  this->_lock();

  // Save current count for returning to the caller
  uint64_t cnt = this->_cnt;

  // Decrement count
  if (this->_cnt >= cnt_)
  {
    this->_cnt -= cnt_;
  } // end if
  else
  {
    this->_cnt = 0;
  } // end else

  // Unlock
  this->_unlock();

  // Return
  return (cnt);
}

void
zEvent::_lock()
{
  sem_wait(&this->_sem);
}

void
zEvent::_unlock()
{
  sem_post(&this->_sem);
}

}
