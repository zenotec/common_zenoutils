/*
 * zzQueue.h
 *
 *  Created on: Jan 28, 2014
 *      Author: Kevin Mahoney
 */

#ifndef _ZQUEUE_H_
#define _ZQUEUE_H_

#include <sstream>
#include <semaphore.h>
#include <queue>

#include "zutils/zEvent.h"
#include "zutils/zSemaphore.h"

namespace zUtils
{

template<typename T>
  class zQueue : private std::queue<T>, public zEvent
  {
  public:
    zQueue()
    {
      // Initialize lock to 'locked'
      int ret = sem_init(&this->_lockSem, 0, 0);
      if (ret != 0)
      {
        std::stringstream errmsg;
        errmsg << "zQueue: Error initializing lock: " << ret;
        throw errmsg.str();
      } // end if

      // Set event FD
      this->_setFd(this->_sem.GetFd());

      // Unlock
      this->_unlock();
    }

    virtual
    ~zQueue()
    {
      int ret = sem_destroy(&this->_lockSem);
      if (ret != 0)
      {
        std::string errmsg;
        errmsg = "zQueue: Cannot destroy lock";
        throw errmsg;
      } // end if
    }

    T
    Front()
    {
      this->_lock();
      T item = this->front();
      this->_unlock();
      return (item);
    }

    T
    Back()
    {
      this->_lock();
      T item = this->back();
      this->_unlock();
      return (item);
    }

    void
    Push(T item_)
    {
      this->_lock();
      this->push(item_);
      this->_notify();
      this->_sem.Post();
      this->_unlock();
    }

    void
    Pop()
    {
      this->_lock();
      this->pop();
      this->_acknowledge();
      this->_sem.Acknowledge();
      this->_unlock();
    }

    size_t
    Size()
    {
      this->_lock();
      size_t size = this->size();
      this->_unlock();
      return (size);
    }

    bool
    Empty()
    {
      this->_lock();
      bool empty = this->empty();
      this->_unlock();
      return (empty);
    }

  protected:

  private:
    void
    _lock()
    {
      sem_wait(&this->_lockSem);
    }

    void
    _unlock()
    {
      sem_post(&this->_lockSem);
    }
    sem_t _lockSem;
    zSemaphore _sem;

  };

}

#endif /* _ZQUEUE_H_ */
