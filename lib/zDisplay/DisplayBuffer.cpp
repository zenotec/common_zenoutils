/*
 * Lcd.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: kmahoney
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
  }
}

DisplayBuffer::DisplayBuffer(DisplayBuffer &other_) :
    _buffer_lock(zSem::Mutex::LOCKED), _rows(other_._rows), _cols(other_._cols), _buffer(NULL)
{
  this->_buffer = (char*) calloc(this->_rows, this->_cols);
  this->_buffer_lock.Unlock();
}

DisplayBuffer::DisplayBuffer(const DisplayBuffer &other_) :
    _buffer_lock(zSem::Mutex::LOCKED), _rows(other_._rows), _cols(other_._cols), _buffer(NULL)
{
  this->_buffer = (char*) calloc(this->_rows, this->_cols);
  this->_buffer_lock.Unlock();
}

bool
DisplayBuffer::operator==(const DisplayBuffer &other_) const
{
  bool status = true;
  status &= (this->GetRows() == other_.GetRows());
  status &= (this->GetColumns() == other_.GetColumns());
  status &= (this->GetSize() == other_.GetSize());
  status &= (memcmp(this->_buffer, other_._buffer, this->GetSize()) == 0);
  return(status);
}

bool
DisplayBuffer::operator!=(const DisplayBuffer &other_) const
{
  bool status = true;
  status &= (this->GetRows() == other_.GetRows());
  status &= (this->GetColumns() == other_.GetColumns());
  status &= (this->GetSize() == other_.GetSize());
  status &= (memcmp(this->_buffer, other_._buffer, this->GetSize()) == 0);
  return(!status);
}

DisplayBuffer&
DisplayBuffer::operator=(DisplayBuffer &other_)
{
  if (this->GetSize() < other_.GetSize())
  {
    if (this->_buffer)
    {
      free(this->_buffer);
    }
    this->_buffer = (char*) calloc(other_._rows, other_._cols);
  }
  memcpy(this->_buffer, other_._buffer, other_.GetSize());
  return(*this);
}

DisplayBuffer&
DisplayBuffer::operator=(const DisplayBuffer &other_)
{
  if (this->GetSize() < other_.GetSize())
  {
    if (this->_buffer)
    {
      free(this->_buffer);
    }
    this->_buffer = (char*) calloc(other_._rows, other_._cols);
  }
  memcpy(this->_buffer, other_._buffer, other_.GetSize());
  return(*this);
}

char
DisplayBuffer::operator [](const size_t i_)
{
  char c = 0;
  if (this->_buffer_lock.Lock())
  {
    if (i_ < this->GetSize())
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
  std::string str;
  if ((row_ < this->_rows) && (col_ < this->_cols) && this->_buffer_lock.Lock())
  {
    char* p = &this->_buffer[row_ * this->_cols];
    for (int i = 0; ((i < this->_cols) && (p[i])); i++)
    {
      str += p[i];
    }
    this->_buffer_lock.Unlock();
  }
  return (str);
}

ssize_t
DisplayBuffer::Update(const std::string& str_, const size_t col_, const size_t row_)
{
  ssize_t size = -1;
  size_t col = 0;
  if ((row_ < this->_rows) && (col_ < this->_cols) && this->_buffer_lock.Lock())
  {
    char* p = &this->_buffer[row_ * this->_cols];
    for (size = 0, col = col_; ((size < str_.size()) && (col < this->_cols)); size++, col++)
    {
      std::cout << "Update[" << row_ << "][" << col << "]: " << str_[size] << std::endl;
      p[col] = str_[size];
    }
    this->_buffer_lock.Unlock();
  }
  return (size);
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

}
}
