//*****************************************************************************
//
//
//
//
//*****************************************************************************

#include <stdlib.h>
#include <string.h>

#include "zutils/zSocket.h"

namespace zUtils
{
namespace zSocket
{

//*****************************************************************************
// Class SocketBuffer
//*****************************************************************************
SocketBuffer::SocketBuffer(size_t size_) :
    _data(0), _tail(0), _end(size_)
{
  this->_head = (uint8_t *) malloc(size_);
  memset(this->_head, 0, size_);
}

SocketBuffer::~SocketBuffer()
{
  if (this->_head)
  {
    free(this->_head);
    this->_head = 0;
  } // end if
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

}
}
