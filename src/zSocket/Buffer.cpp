//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Buffer.cpp
//    Description:
//
//*****************************************************************************

#include <stdlib.h>
#include <string.h>

#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

//*****************************************************************************
// Class Buffer
//*****************************************************************************
Buffer::Buffer(size_t size_) :
    _head(0), _data(0), _tail(0), _end(size_)
{
  this->_head = (uint8_t *) malloc(size_);
  if (this->_head)
  {
    memset(this->_head, 0, size_);
  }
}

Buffer::Buffer(Buffer &other_) :
    _head(0), _data(other_._data), _tail(other_._tail), _end(other_._end)
{
  this->_head = (uint8_t *) malloc(other_._end);
  if (this->_head)
  {
    memset(this->_head, 0, other_._end);
    memcpy(this->_head, other_._head, other_._tail);
  }
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
  this->_head = (uint8_t *) malloc(other_._end);
  if (this->_head)
  {
    this->_data = other_._data;
    this->_tail = other_._tail;
    this->_end = other_._end;
    memset(this->_head, 0, other_._end);
    memcpy(this->_head, other_._head, other_._tail);
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
  return (std::string((const char *) this->_head, this->_tail));
}

}
}
