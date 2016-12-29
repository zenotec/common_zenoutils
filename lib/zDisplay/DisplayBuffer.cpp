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

#include <signal.h>

#include <string>
#include <mutex>
#include <list>
#include <vector>
#include <map>

#include <zutils/zSem.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>
#include <zutils/zTimer.h>
#include <zutils/zLog.h>
#include <zutils/zData.h>
#include <zutils/zSocket.h>
#include <zutils/zMessage.h>
#include <zutils/zCommand.h>

#include <zutils/zDisplay.h>

namespace zUtils
{
namespace zDisplay
{

//*****************************************************************************
// Class: zDisplay::DisplayBuffer
//*****************************************************************************

DisplayBuffer::DisplayBuffer(const size_t cols_, const size_t rows_) :
    _buffer_lock(zSem::Mutex::LOCKED), _rows(rows_), _cols(cols_), _buffer(NULL)
{
  this->_buffer = (char*) calloc(rows_, cols_);
  this->_buffer_lock.Unlock();
}

DisplayBuffer::~DisplayBuffer()
{
  this->_buffer_lock.Lock();
  if (this->_buffer)
  {
    free(this->_buffer);
    this->_buffer = NULL;
  }
}

DisplayBuffer::DisplayBuffer(DisplayBuffer &other_) :
    _buffer_lock(zSem::Mutex::LOCKED), _rows(other_._rows), _cols(other_._cols), _buffer(NULL)
{
  this->_buffer = (char*) calloc(this->_rows, this->_cols);
  if (other_._buffer_lock.Lock())
  {
    size_t other_size = (other_._rows * other_._cols);
    memcpy(this->_buffer, other_._buffer, other_size);
    other_._buffer_lock.Unlock();
  }
  this->_buffer_lock.Unlock();
}

DisplayBuffer::DisplayBuffer(const DisplayBuffer &other_) :
    _buffer_lock(zSem::Mutex::LOCKED), _rows(other_._rows), _cols(other_._cols), _buffer(NULL)
{
  this->_buffer = (char*) calloc(this->_rows, this->_cols);
  if (other_._buffer_lock.Lock())
  {
    size_t other_size = (other_._rows * other_._cols);
    memcpy(this->_buffer, other_._buffer, other_size);
    other_._buffer_lock.Unlock();
  }
  this->_buffer_lock.Unlock();
}

bool
DisplayBuffer::operator==(const DisplayBuffer &other_) const
    {
  bool status = false;
  if ((status = this->_buffer_lock.Lock()))
  {
    status &= (this->_rows == other_.GetRows());
    status &= (this->_cols == other_.GetColumns());
    status &= ((this->_rows * this->_cols) == other_.GetSize());
    status &= (memcmp(this->_buffer, other_._buffer, (this->_rows * this->_cols)) == 0);
    this->_buffer_lock.Unlock();
  }
  return (status);
}

bool
DisplayBuffer::operator!=(const DisplayBuffer &other_) const
    {
  bool status = false;
  if ((status = this->_buffer_lock.Lock()))
  {
    status &= (this->_rows == other_.GetRows());
    status &= (this->_cols == other_.GetColumns());
    status &= ((this->_rows * this->_cols) == other_.GetSize());
    status &= (memcmp(this->_buffer, other_._buffer, (this->_rows * this->_cols)) == 0);
    this->_buffer_lock.Unlock();
  }
  return (!status);
}

DisplayBuffer&
DisplayBuffer::operator=(DisplayBuffer &other_)
{
  size_t size = 0;
  size_t other_size = 0;

  if (this->_buffer_lock.Lock())
  {
    if (other_._buffer_lock.Lock())
    {
      size = (this->_rows * this->_cols);
      other_size = (other_._rows * other_._cols);
      if (size < other_size)
      {
        if (this->_buffer)
        {
          free(this->_buffer);
        }
        this->_buffer = (char*) calloc(other_._rows, other_._cols);
      }
      memcpy(this->_buffer, other_._buffer, other_size);
      other_._buffer_lock.Unlock();
    }
    this->_buffer_lock.Unlock();
  }

  return (*this);
}

DisplayBuffer&
DisplayBuffer::operator=(const DisplayBuffer &other_)
{
  size_t size = 0;
  size_t other_size = 0;

  if (this->_buffer_lock.Lock())
  {
    if (other_._buffer_lock.Lock())
    {
      size = (this->_rows * this->_cols);
      other_size = (other_._rows * other_._cols);
      if (size < other_size)
      {
        if (this->_buffer)
        {
          free(this->_buffer);
        }
        this->_buffer = (char*) calloc(other_._rows, other_._cols);
      }
      memcpy(this->_buffer, other_._buffer, other_size);
      other_._buffer_lock.Unlock();
    }
    this->_buffer_lock.Unlock();
  }

  return (*this);
}

char
DisplayBuffer::operator [](const size_t i_) const
    {
  char c = 0;
  if (this->_buffer_lock.Lock())
  {
    if (this->_index2rowcol(i_))
    {
      c = this->_buffer[i_];
    }
    this->_buffer_lock.Unlock();
  }
  return (c);
}

ssize_t
DisplayBuffer::GetRows() const
{
  ssize_t rows = -1;
  if (this->_buffer_lock.Lock())
  {
    rows = this->_rows;
    this->_buffer_lock.Unlock();
  }
  return (rows);
}

ssize_t
DisplayBuffer::GetColumns() const
{
  ssize_t cols = -1;
  if (this->_buffer_lock.Lock())
  {
    cols = this->_cols;
    this->_buffer_lock.Unlock();
  }
  return (cols);
}

ssize_t
DisplayBuffer::GetSize() const
{
  ssize_t size = -1;
  if (this->_buffer_lock.Lock())
  {
    size = (this->_rows * this->_cols);
    this->_buffer_lock.Unlock();
  }
  return (size);
}

std::string
DisplayBuffer::GetString(const size_t col_, const size_t row_) const
    {
  size_t index = 0;
  std::string str;

  if (this->_rowcol2index(row_, col_, &index) && this->_buffer_lock.Lock())
  {
    while (this->_index2rowcol(index) && this->_buffer[index])
    {
      ZLOG_DEBUG(
          "[" + ZLOG_UINT(row_) + "][" + ZLOG_UINT(col_) + "][" + ZLOG_UINT(index) + "]: " + this->_buffer[index]);
      str += this->_buffer[index++];
    }
    this->_buffer_lock.Unlock();
  }
  return (str);
}

ssize_t
DisplayBuffer::Update(const std::string& str_, const size_t col_, const size_t row_)
{
  ssize_t size = -1;
  size_t index = 0;

  if (this->_rowcol2index(row_, col_, &index) && this->_buffer_lock.Lock())
  {
    size = 0;
    while (this->_index2rowcol(index) && (size < str_.size()))
    {
      ZLOG_DEBUG("[" + ZLOG_UINT(row_) + "][" + ZLOG_UINT(col_) +
          "][" + ZLOG_UINT(index) + "]: " + str_[size]);
      this->_buffer[index++] = str_[size++];
    }
    this->_buffer_lock.Unlock();
  }
  return (size);
}

void
DisplayBuffer::Print() const
{
  size_t index = 0;

  if (this->_buffer_lock.Lock())
  {
    while (this->_index2rowcol(index))
    {
      std::string c;
      if (isprint(this->_buffer[index]) != 0)
      {
        c += this->_buffer[index];
      }
      else
      {
        c += ZLOG_HEX(this->_buffer[index]);
      }
      if (!(index % this->_cols))
      {
        std::cout << std::endl << "[" << index << "]:\t";
      }
      std::cout << c;
      index++;
    }
    std::cout << std::endl;
    this->_buffer_lock.Unlock();
  }
}

void
DisplayBuffer::Clear()
{
  if (this->_buffer_lock.Lock())
  {
    memset(this->_buffer, 0, (this->_rows * this->_cols));
    this->_buffer_lock.Unlock();
  }
}

bool
DisplayBuffer::_index2rowcol(size_t index_, size_t* row_, size_t* col_) const
    {
  // Assume callers have obtained lock
  bool status = false;
  if (index_ < (this->_rows * this->_cols))
  {
    if (row_)
    {
      *row_ = (index_ / this->_cols);
    }
    if (col_)
    {
      *col_ = (index_ % this->_cols);
    }
    status = true;
  }
  return (status);
}

bool
DisplayBuffer::_rowcol2index(size_t row_, size_t col_, size_t* index_) const
    {
  // Assume callers have obtained lock
  bool status = false;
  if ((row_ < this->_rows) && (col_ < this->_cols))
  {
    if (index_)
    {
      *index_ = ((row_ * this->_cols) + col_);
    }
    status = true;
  }
  return (status);
}

}
}
