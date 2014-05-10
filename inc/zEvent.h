/*
 * Event.h
 *
 *  Created on: Jan 28, 2014
 *      Author: kmahoney
 */

#ifndef _ZEVENT_H_
#define _ZEVENT_H_

#include <stdint.h>
#include <semaphore.h>

namespace zUtils
{

class zEvent
{
public:
  zEvent();
  virtual
  ~zEvent();

  int
  GetFd();
  uint64_t
  GetPending();

protected:
  void
  _setFd(const int id_);
  void
  _notify(const uint64_t &cnt_ = 1);
  uint64_t
  _acknowledge(const uint64_t &cnt_ = 1);

private:
  void
  _lock();
  void
  _unlock();
  sem_t _sem;
  int _fd;
  uint64_t _cnt;
};

}

#endif /* _ZEVENT_H_ */
