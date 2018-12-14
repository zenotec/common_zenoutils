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
#include <unistd.h>
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

static struct timespec
_get_ts()
{
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return (ts);
}

static double
_diff_time(const struct timespec& ts1_, const struct timespec& ts2_)
{

  double t1 = double(ts1_.tv_sec) + (double(ts1_.tv_nsec) / 1000000000.0);
  double t2 = double(ts2_.tv_sec) + (double(ts2_.tv_nsec) / 1000000000.0);
  return (t1 - t2);
}

static void
_display_time(const struct timespec& ts_, const char* prefix_ = "Time")
{
  double t = double(ts_.tv_sec) + (double(ts_.tv_nsec) / 1000000000.0);
  fprintf(stdout, "%s: %f\n", prefix_, t);
}

static void
_display_time(const double ts_, const char* prefix_ = "Time")
{
  fprintf(stdout, "%s: %f\n", prefix_, ts_);
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

struct skbmem
{
  struct timespec ts;
  uint8_t* data;
  size_t len;

  skbmem(size_t len_ = 0) :
    data(NULL), len(len_)
  {
    ts = _get_ts();
    len = (len > 0) ? len : sysconf(_SC_PAGESIZE);
    data = (uint8_t*)malloc(len);
    memset(data, 0, len);
  }

  ~skbmem()
  {
    if (data)
    {
      free(data);
      data = NULL;
    }
//    double diff = _diff_time(_get_ts(), this->ts);
//    if (diff > 0.1)
//    {
//      fprintf(stderr, "Stale socket buffer: %f\n", diff);
//    }
  }

};

//*****************************************************************************
// Class: Buffer
//*****************************************************************************

Buffer::Buffer(size_t size_) :
    _skbmem(NULL), _head(NULL), _data(0), _tail(0), _end(0)
{
  this->_init(size_);
}

Buffer::Buffer(const uint8_t* data_, const size_t len_) :
    _skbmem(NULL), _head(NULL), _data(0), _tail(0), _end(0)
{
  this->_init(len_);
  this->Write(data_, len_);
}

Buffer::Buffer(const std::string& str_) :
    _skbmem(NULL), _head(NULL), _data(0), _tail(0), _end(0)
{
  this->_init(str_.size());
  this->String(str_);
}

Buffer::Buffer(const Buffer &other_) :
    _skbmem(NULL), _head(NULL), _data(0), _tail(0), _end(0)
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

size_t
Buffer::Headroom() const
{
  return (this->_data);
}

uint8_t*
Buffer::Data() const
{
  return (this->_head + this->_data);
}

uint8_t*
Buffer::Tail() const
{
  return (this->_head + this->_tail);
}

size_t
Buffer::Tailroom() const
{
  return (this->_end - this->_tail);
}

uint8_t*
Buffer::End() const
{
  return (this->_head + this->_end);
}

bool
Buffer::Reserve(off_t off_)
{
  bool ret = false;
  if (((this->_data + off_) <= this->_end) && ((this->_tail + off_) <= this->_end))
  {
    this->_data += off_;
    this->_tail += off_;
    ret = true;
  } // end if
  return (ret);
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
Buffer::Pop(off_t off_)
{
  bool ret = false;
  if ((this->_tail - off_) >= 0)
  {
    this->_tail -= off_;
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
Buffer::Reset()
{
  this->_data = 0;
  this->_tail = 0;
  return true;
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

const struct timespec&
Buffer::Timestamp() const
{
  return (this->_skbmem->ts);
}

bool
Buffer::Timestamp(const struct timespec& ts_)
{
  this->_skbmem->ts = ts_;
  return (true);
}

bool
Buffer::Read(uint8_t* data_, const size_t len_)
{
  bool status = false;
  if (this->Length() >= len_)
  {
    memcpy(data_, this->Data(), len_);
    status = this->Pull(len_);
  }
  return (status);
}

bool
Buffer::Write(const uint8_t* data_, const size_t len_)
{
  bool status = false;
  if (this->Put(len_))
  {
    memcpy(this->Data(), data_, len_);
    status = this->Pull(len_);
  }
  return (status);
}

std::string
Buffer::String() const
{
  std::string str;
  str.resize(this->Length());
  memcpy((uint8_t*)str.data(), this->Data(), this->Length());
  return (str);
}

bool
Buffer::String(const std::string& str_)
{
  bool status = false;
  if (str_.size() <= this->Tailroom())
  {
    memcpy(this->Data(), (uint8_t*)str_.data(), str_.size());
    status = this->Put(str_.size());
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
  printf("\tTS:   %ld.%ld\n", this->Timestamp().tv_sec, this->Timestamp().tv_nsec);
  __dump_hex("", this->Head(), this->Size(), true);
}

void
Buffer::_init(const size_t size_)
{
  this->_skbmem = SHPTR(struct skbmem)(new skbmem(size_));
  if (!this->_skbmem.get())
  {
    std::string errMsg = "Error allocating memory for socket buffer";
    ZLOG_CRIT(errMsg);
    throw errMsg;
  }
  this->_head = this->_skbmem->data;
  this->_end = this->_skbmem->len;
}

void
Buffer::_copy (const Buffer& other_)
{
  this->_skbmem = other_._skbmem;
  this->_head = other_._head;
  this->_data = other_._data;
  this->_tail = other_._tail;
  this->_end = other_._end;
}

}
}
