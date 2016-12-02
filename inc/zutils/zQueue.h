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

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zEvent.h>

namespace zUtils
{

template<typename T>
  class zQueue : private std::queue<T>, public zSem::Semaphore
  {
  public:
    zQueue()
    {
      this->_queue_lock.Unlock();
    }

    virtual
    ~zQueue()
    {
      this->_queue_lock.Lock();
    }

    T
    Front()
    {
      T item;
      if (this->_queue_lock.Lock())
      {
        item = this->front();
        this->_queue_lock.Unlock();
      }
      return (item);
    }

    T
    Back()
    {
      T item;
      if (this->_queue_lock.Lock())
      {
        item = this->back();
        this->_queue_lock.Unlock();
      }
      return (item);
    }

    void
    Push(T item_)
    {
      if (this->_queue_lock.Lock())
      {
        this->push(item_);
        this->Post();
        this->_queue_lock.Unlock();
      }
    }

    void
    Pop()
    {
      if (this->_queue_lock.Lock())
      {
        this->pop();
        this->_queue_lock.Unlock();
      }
    }

    ssize_t
    Size()
    {
      ssize_t size = -1;
      if (this->_queue_lock.Lock())
      {
        size = this->size();
        this->_queue_lock.Unlock();
      }
      return (size);
    }

    bool
    Empty()
    {
      bool empty = true;
      if (this->_queue_lock.Lock())
      {
        empty = this->empty();
        this->_queue_lock.Unlock();
      }
      return (empty);
    }

    void
    Clear()
    {
      if (this->_queue_lock.Lock())
      {
        while (this->TryWait());
        while (!this->empty())
        {
          this->pop();
        }
        this->_queue_lock.Unlock();
      }
      return;
    }

  protected:

  private:
    zSem::Mutex _queue_lock;

  };

}

#endif /* __ZQUEUE_H__ */
