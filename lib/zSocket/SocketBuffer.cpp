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
#include <string.h>
#include <stdint.h>
#include <netinet/in.h>
#include <string.h>

#include <string>
#include <list>
#include <mutex>
#include <memory>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>

#include <zutils/zSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

//*****************************************************************************
// Class: Buffer
//*****************************************************************************

Buffer::Buffer(size_t size_) :
    _head(0), _data(0), _tail(0), _end(size_)
{
  this->_head = (uint8_t *) malloc(this->_end);
  if (!this->_head)
  {
    std::string errMsg = "Error allocating memory for socket buffer";
    ZLOG_CRIT(errMsg);
    throw errMsg;
  }
  memset(this->_head, 0, this->_end);
}

Buffer::Buffer(const std::string &str_) :
    _head(0), _data(0), _tail(0), _end(str_.size())
{
  this->_head = (uint8_t *) malloc(this->_end);
  if (!this->_head)
  {
    std::string errMsg = "Error allocating memory for socket buffer";
    ZLOG_CRIT(errMsg);
    throw errMsg;
  }
  memset(this->_head, 0, this->_end);
  this->Str(str_);
}

Buffer::Buffer(Buffer &other_) :
    _head(0), _data(other_._data), _tail(other_._tail), _end(other_._end)
{
  this->_head = (uint8_t *) malloc(other_._end);
  if (!this->_head)
  {
    std::string errMsg = "Error allocating memory for socket buffer";
    ZLOG_CRIT(errMsg);
    throw errMsg;
  }
  memset(this->_head, 0, other_._end);
  memcpy(this->_head, other_._head, other_._tail);
}

Buffer::Buffer(const Buffer &other_) :
    _head(0), _data(other_._data), _tail(other_._tail), _end(other_._end)
{
  this->_head = (uint8_t *) malloc(other_._end);
  if (!this->_head)
  {
    std::string errMsg = "Error allocating memory for socket buffer";
    ZLOG_CRIT(errMsg);
    throw errMsg;
  }
  memset(this->_head, 0, other_._end);
  memcpy(this->_head, other_._head, other_._tail);
}

Buffer::~Buffer()
{
  if (this->_head)
  {
    free(this->_head);
    this->_head = 0;
  } // end if
}

Buffer &
Buffer::operator=(Buffer &other_)
{
  // Free previous buffer if it is a different size
  if (this->_head && (this->_end != other_._end))
  {
    free(this->_head);
    this->_head = (uint8_t *) malloc(other_._end);
    if (!this->_head)
    {
      std::string errMsg = "Error allocating memory for socket buffer";
      ZLOG_CRIT(errMsg);
      throw errMsg;
    }
  } // end if
  this->_data = other_._data;
  this->_tail = other_._tail;
  this->_end = other_._end;
  memcpy(this->_head, other_._head, this->_tail);
  memset(this->_head + this->_tail, 0, this->_end - this->_tail);
  return (*this);
}

Buffer &
Buffer::operator=(const Buffer &other_)
{
  // Free previous buffer if it is a different size
  if (this->_head && (this->_end != other_._end))
  {
    free(this->_head);
    this->_head = (uint8_t *) malloc(other_._end);
    if (!this->_head)
    {
      std::string errMsg = "Error allocating memory for socket buffer";
      ZLOG_CRIT(errMsg);
      throw errMsg;
    }
  } // end if
  this->_data = other_._data;
  this->_tail = other_._tail;
  this->_end = other_._end;
  memcpy(this->_head, other_._head, this->_tail);
  memset(this->_head + this->_tail, 0, this->_end - this->_tail);
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

}
}
