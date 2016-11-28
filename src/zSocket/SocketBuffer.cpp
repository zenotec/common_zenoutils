//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Buffer.cpp
//    Description:
//
//*****************************************************************************

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

namespace zUtils
{
namespace zSocket
{

//*****************************************************************************
// Class: SocketBuffer
//*****************************************************************************

SocketBuffer::SocketBuffer(size_t size_) :
    _head(0), _data(0), _tail(0), _end(size_)
{
  this->_head = (uint8_t *) malloc(size_);
  if (!this->_head)
  {
    std::string errMsg = "Error allocating memory for socket buffer";
    ZLOG_CRIT(errMsg);
    throw errMsg;
  }
  memset(this->_head, 0, size_);
}

SocketBuffer::SocketBuffer(const std::string &str_)
{
  this->Str(str_);
}

SocketBuffer::SocketBuffer(SocketBuffer &other_) :
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


SocketBuffer::SocketBuffer(const SocketBuffer &other_) :
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

SocketBuffer::~SocketBuffer()
{
  if (this->_head)
  {
    free(this->_head);
    this->_head = 0;
  } // end if
}

SocketBuffer &
SocketBuffer::operator=(SocketBuffer &other_)
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

SocketBuffer &
SocketBuffer::operator=(const SocketBuffer &other_)
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
SocketBuffer::operator ==(SocketBuffer &other_)
{
  bool stat = false;
  if (this->Size() == other_.Size())
  {
    stat = (memcmp(this->_head, other_._head, this->Size()) == 0);
  } // end if
  return (stat);
}

bool
SocketBuffer::operator !=(SocketBuffer &other_)
{
  bool stat = false;
  if (this->Size() == other_.Size())
  {
    stat = (memcmp(this->_head, other_._head, this->Size()) == 0);
  } // end if
  return (!stat);
}

uint8_t *
SocketBuffer::Head()
{
  return (this->_head);
}

bool
SocketBuffer::Put(off_t off_)
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
SocketBuffer::Push(off_t off_)
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
SocketBuffer::Pull(off_t off_)
{
  bool ret = false;
  if ((this->_data + off_) <= this->_tail)
  {
    this->_data += off_;
    ret = true;
  } // end if
  return (ret);
}

uint8_t *
SocketBuffer::Data(off_t off_)
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
SocketBuffer::Tail()
{
  return (this->_head + this->_tail);
}

uint8_t *
SocketBuffer::End()
{
  return (this->_head + this->_end);
}

size_t
SocketBuffer::Length()
{
  return (this->_tail - this->_data);
}

size_t
SocketBuffer::Size()
{
  return (this->_tail);
}

size_t
SocketBuffer::TotalSize()
{
  return (this->_end);
}

std::string
SocketBuffer::Str()
{
  return (std::string((const char *) this->Head(), this->Size()));
}

bool
SocketBuffer::Str(const std::string &str_)
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
