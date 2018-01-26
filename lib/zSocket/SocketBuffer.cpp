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

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <string>

#include <zutils/zLog.h>
#include <zutils/zSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

struct skbmem
{
  zSem::Mutex lock;
  uint32_t refcnt;
  uint8_t data[8192];
};

struct timespec
_get_ts()
{
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return (ts);
}

//*****************************************************************************
// Class: Buffer
//*****************************************************************************

Buffer::Buffer(size_t size_) :
    _ts(_get_ts()), _skbmem(new skbmem), _head(NULL), _data(0), _tail(0), _end(size_)
{
  if (!this->_skbmem)
  {
    std::string errMsg = "Error allocating memory for socket buffer";
    ZLOG_CRIT(errMsg);
    throw errMsg;
  }
  this->_skbmem->refcnt = 1;
  this->_head = &this->_skbmem->data[1024];
  memset(this->_head, 0, this->_end);
  this->_skbmem->lock.Unlock();
}

Buffer::Buffer(const std::string &str_) :
    _ts(_get_ts()), _skbmem(new skbmem), _head(NULL), _data(0), _tail(0), _end(str_.size())
{
  if (!this->_skbmem)
  {
    std::string errMsg = "Error allocating memory for socket buffer";
    ZLOG_CRIT(errMsg);
    throw errMsg;
  }
  this->_skbmem->refcnt = 1;
  this->_head = &this->_skbmem->data[1024];
  memset(this->_head, 0, this->_end);
  this->_skbmem->lock.Unlock();
  this->Str(str_);
}

Buffer::Buffer(const Buffer &other_) :
    _ts(_get_ts()), _skbmem(other_._skbmem), _head(other_._head), _data(other_._data),
    _tail(other_._tail), _end(other_._end)
{
  if (this->_skbmem && this->_skbmem->lock.Lock())
  {
    this->_skbmem->refcnt++;
    this->_skbmem->lock.Unlock();
  }
}

Buffer::~Buffer()
{
  if (this->_skbmem && this->_skbmem->lock.Lock())
  {
    if (this->_skbmem->refcnt-- == 1)
    {
      delete (this->_skbmem);
    }
    else
    {
      this->_skbmem->lock.Unlock();
    }
    this->_skbmem = NULL;
    this->_head = NULL;
    this->_data = 0;
    this->_tail = 0;
    this->_end = 0;
  }
}

Buffer &
Buffer::operator=(const Buffer &other_)
{
  if (this->_skbmem && this->_skbmem->lock.Lock())
  {
    if (this->_skbmem->refcnt-- == 1)
    {
      delete (this->_skbmem);
    }
    else
    {
      this->_skbmem->lock.Unlock();
    }
  }
  this->_skbmem = other_._skbmem;
  this->_head = other_._head;
  this->_data = other_._data;
  this->_tail = other_._tail;
  this->_end = other_._end;
  if (this->_skbmem && this->_skbmem->lock.Lock())
  {
    this->_skbmem->refcnt++;
    this->_skbmem->lock.Unlock();
  }
  return (*this);
}

bool
Buffer::operator ==(Buffer &other_)
{
  bool stat = false;
  if (this->Size() == other_.Size())
  {
    stat = (memcmp(this->_head, other_._head, this->Size()) == 0);
  } // end if
  return (stat);
}

bool
Buffer::operator !=(Buffer &other_)
{
  bool stat = false;
  if (this->Size() == other_.Size())
  {
    stat = (memcmp(this->_head, other_._head, this->Size()) == 0);
  } // end if
  return (!stat);
}

uint8_t *
Buffer::Head()
{
  return (this->_head);
}

bool
Buffer::Put(off_t off_)
{
  bool ret = false;
  if ((this->_tail + off_) <= this->_end)
  {
    this->_tail += off_;
    ret = true;
  } // end if
  return (ret);
}

bool
Buffer::Push(off_t off_)
{
  bool ret = false;
  if ((this->_data - off_) >= 0)
  {
    this->_data -= off_;
    ret = true;
  } // end if
  return (ret);
}

bool
Buffer::Pull(off_t off_)
{
  bool ret = false;
  if ((this->_data + off_) <= this->_tail)
  {
    this->_data += off_;
    ret = true;
  } // end if
  return (ret);
}

bool
Buffer::Reset()
{
  this->_data = 0;
  this->_tail = 0;
  return true;
}

uint8_t *
Buffer::Data(off_t off_)
{
  uint8_t *data = 0;
  if ((this->_data + off_) <= this->_tail)
  {
    data = (this->_head + this->_data);
    this->_data += off_;
  } // end if
  return (data);
}

uint8_t *
Buffer::Tail()
{
  return (this->_head + this->_tail);
}

uint8_t *
Buffer::End()
{
  return (this->_head + this->_end);
}

size_t
Buffer::Length()
{
  return (this->_tail - this->_data);
}

size_t
Buffer::Size()
{
  return (this->_tail);
}

size_t
Buffer::TotalSize()
{
  return (this->_end);
}

std::string
Buffer::Str()
{
  return (std::string((const char *) this->Head(), this->Size()));
}

bool
Buffer::Str(const std::string &str_)
{
  bool status = false;
  void *ret = memcpy(this->Head(), str_.c_str(), str_.size());
  if ((ret == this->Head()) && this->Put(str_.size()))
  {
    status = true;
  }
  return (status);
}

void
Buffer::Display() const
{
  printf("zSocket::Buffer(): %p\n", this);
}

}
}
