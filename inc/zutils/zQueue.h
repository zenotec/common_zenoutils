/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ZQUEUE_H__
#define __ZQUEUE_H__

#include <queue>

#include <zutils/zSem.h>

namespace zUtils
{
namespace zQueue
{

template<typename T>
  class Queue :
      private std::queue<T>,
      public zSem::Semaphore
  {

  public:

    Queue()
    {
      this->_lock.Unlock();
    }

    virtual
    ~Queue()
    {
      this->_lock.Lock();
    }

    T
    Front()
    {
      T item;
      if (this->_lock.Lock())
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
      if (this->_lock.Lock())
      {
        item = this->back();
        this->_lock.Unlock();
      }
      return (item);
    }

    bool
    Push(T item_)
    {
      bool status = false;
      if (this->_lock.Lock())
      {
        this->push(item_);
        status = this->Post();
        status &= this->_lock.Unlock();
      }
      return (status);
    }

    bool
    Pop()
    {
      bool status = false;
      if (this->_lock.Lock())
      {
        this->pop();
        status = this->_lock.Unlock();
      }
      return (status);
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
      if (this->_lock.Lock())
      {
        empty = this->empty();
        this->_lock.Unlock();
      }
      return (empty);
    }

    void
    Clear()
    {
      if (this->_lock.Lock())
      {
        while (this->TryWait());
        while (!this->empty())
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
}

#endif /* __ZQUEUE_H__ */
