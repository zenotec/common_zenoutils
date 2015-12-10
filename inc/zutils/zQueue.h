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
  class zQueue : private std::queue<T>, public zEvent::Event, public zSem::Semaphore
  {
  public:
    zQueue()
    {
      this->_lock.Unlock();
    }

    virtual
    ~zQueue()
    {
      this->_lock.Lock();
    }

    T
    Front()
    {
      T item;
      if(this->_lock.Lock())
      {
        item = this->front();
        this->_lock.Unlock();
      }
      return (item);
    }

    T
    Back()
    {
      T item;
      if(this->_lock.Lock())
      {
        item = this->back();
        this->_lock.Unlock();
      }
      return (item);
    }

    void
    Push(T item_)
    {
      if (this->_lock.Lock())
      {
        this->push(item_);
        this->Post();
        this->Notify();
        this->_lock.Unlock();
      }
    }

    void
    Pop()
    {
      if(this->_lock.Lock())
      {
        this->pop();
        this->_lock.Unlock();
      }
    }

    ssize_t
    Size()
    {
      ssize_t size = -1;
      if (this->_lock.Lock())
      {
        size = this->size();
        this->_lock.Unlock();
      }
      return (size);
    }

    bool
    Empty()
    {
      bool empty = true;
      if(this->_lock.Lock())
      {
        empty = this->empty();
        this->_lock.Unlock();
      }
      return (empty);
    }

    void
    Clear()
    {
      if(this->_lock.Lock())
      {
        while(!this->empty())
        {
          this->pop();
        }
        this->_lock.Unlock();
      }
      return;
    }

  protected:

  private:
    zSem::Mutex _lock;

  };

}

#endif /* __ZQUEUE_H__ */
