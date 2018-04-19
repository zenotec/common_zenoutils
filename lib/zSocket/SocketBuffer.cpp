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

#define SKBMEM_DATA_LEN     (8 * 1024)

struct skbmem
{
  zSem::Mutex lock;
  struct timespec ts;
  uint8_t data[SKBMEM_DATA_LEN];
};

struct timespec
_get_ts()
{
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return (ts);
}

void
__dump_hex(const char* prefix_, const uint8_t* addr_, size_t len_, bool verbose_)
{
  const char* nl = "";
  unsigned long pad = ((unsigned long)addr_ & 0x07);

  if (!len_)
    return;

  if (!verbose_)
    len_ = std::min(int(len_), 16);

  for (int cnt = 0, i = -pad; i < len_; cnt++, i++)
  {
    if (!(cnt % 8))
      printf("%s%s%p: ", nl, prefix_, &addr_[i]);
    if (i < 0)
      printf("-- ");
    else
      printf("%02x ", addr_[i]);
    nl = "\n";
  }
  printf("\n");
}

//*****************************************************************************
// Class: Buffer
//*****************************************************************************

Buffer::Buffer(size_t size_) :
    _ts{ 0 }, _skbmem(NULL), _head(NULL), _data(0), _tail(0), _end(0)
{
  this->_init(size_);
  this->_skbmem->lock.Unlock();
}

Buffer::Buffer(const std::string &str_) :
    _ts{ 0 }, _skbmem(NULL), _head(NULL), _data(0), _tail(0), _end(0)
{
  this->_init(str_.size());
  this->Str(str_);
}

Buffer::Buffer(const Buffer &other_) :
    _ts(_get_ts()), _skbmem(NULL), _head(NULL), _data(0), _tail(0), _end(0)
{
  this->_copy(other_);
}

Buffer::~Buffer()
{
  this->_skbmem = NULL;
  this->_head = NULL;
  this->_data = 0;
  this->_tail = 0;
  this->_end = 0;
}

Buffer &
Buffer::operator=(const Buffer& other_)
{
  this->_copy(other_);
  return (*this);
}

bool
Buffer::operator ==(const Buffer &other_) const
{
  bool stat = false;
  if (this->Size() == other_.Size())
  {
    stat = (memcmp(this->_head, other_._head, this->Size()) == 0);
  } // end if
  return (stat);
}

bool
Buffer::operator !=(const Buffer &other_) const
{
  return (!(this->operator ==(other_)));
}

uint8_t *
Buffer::Head() const
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
Buffer::Data() const
{
  return (this->_head + this->_data);
}

uint8_t *
Buffer::Tail() const
{
  return (this->_head + this->_tail);
}

uint8_t *
Buffer::End() const
{
  return (this->_head + this->_end);
}


size_t
Buffer::Length() const
{
  return (this->_tail - this->_data);
}

size_t
Buffer::Size() const
{
  return (this->_tail);
}

size_t
Buffer::TotalSize() const
{
  return (this->_end);
}

std::string
Buffer::Str() const
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
  printf("\tHead: %p\n", this->Head());
  printf("\tData: %p (%zd)\n", this->Data(), this->_data);
  printf("\tTail: %p (%zd)\n", this->Tail(), this->_tail);
  printf("\tEnd:  %p (%zd)\n", this->End(), this->_end);
  __dump_hex("", this->Head(), this->Size(), true);
}

void
Buffer::_init(const size_t size_)
{
  this->_skbmem = SHARED_PTR(struct skbmem)(new skbmem);
  if (!this->_skbmem.get())
  {
    std::string errMsg = "Error allocating memory for socket buffer";
    ZLOG_CRIT(errMsg);
    throw errMsg;
  }
  this->_ts = _get_ts();
  this->_skbmem->ts = this->_ts;
  this->_head = &this->_skbmem->data[0];
  this->_end = ((size_ < SKBMEM_DATA_LEN) ? size_ : SKBMEM_DATA_LEN);
  memset(this->_head, 0, this->_end);
}

void
Buffer::_copy (const Buffer& other_)
{
  this->_skbmem = other_._skbmem;
  this->_head = other_._head;
  this->_data = 0;
  this->_tail = other_._tail;
  this->_end = other_._end;
}

}
}
