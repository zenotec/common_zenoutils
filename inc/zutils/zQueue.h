//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zQueue.h
//    Description:
//
//*****************************************************************************

#ifndef __ZQUEUE_H__
#define __ZQUEUE_H__

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
      this->_lock.Unlock();
    }

    virtual
    ~zQueue()
    {
    }

    T
    Front()
    {
      this->_lock.Lock();
      T item = this->front();
      this->_lock.Unlock();
      return (item);
    }

    T
    Back()
    {
      this->_lock.Lock();
      T item = this->back();
      this->_lock.Unlock();
      return (item);
    }

    void
    Push(T item_)
    {
      this->_lock.Lock();
      this->push(item_);
      this->_notify();
      this->_lock.Unlock();
    }

    void
    Pop()
    {
      this->_lock.Lock();
      this->pop();
      this->_lock.Unlock();
    }

    size_t
    Size()
    {
      this->_lock.Lock();
      size_t size = this->size();
      this->_lock.Unlock();
      return (size);
    }

    bool
    Empty()
    {
      this->_lock.Lock();
      bool empty = this->empty();
      this->_lock.Unlock();
      return (empty);
    }

  protected:

  private:
    zMutex _lock;

  };

}

#endif /* __ZQUEUE_H__ */
