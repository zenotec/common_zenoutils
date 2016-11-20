/*
 * LcdVar.cpp
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

#include <zutils/zLcd.h>

namespace zUtils
{
namespace zLcd
{

//*****************************************************************************
// zLcd::LcdVar Class
//*****************************************************************************

LcdVar::LcdVar(const std::string &name_) :
    _name(name_), _row(0), _col(0), _len(0)
{
  this->_lock.Unlock();
}

LcdVar::~LcdVar()
{
}

std::string
LcdVar::GetName()
{
  std::string name;
  if (this->_lock.Lock())
  {
    name = this->_name;
    this->_lock.Unlock();
  }
  return (name);
}

std::string
LcdVar::GetValue()
{
  std::string value;
  if (this->_lock.Lock())
  {
    value = this->_value;
    this->_lock.Unlock();
  }
  return (value);
}

bool
LcdVar::SetValue(const std::string &value_)
{
  bool status = false;
  if ((value_.size() <= this->_len) && (this->_lock.Lock()))
  {
    this->_value = value_;
    status = true;
    this->_lock.Unlock();
  }
  return (status);
}

ssize_t
LcdVar::GetCol()
{
  ssize_t col = -1;
  if (this->_lock.Lock())
  {
    col = this->_col;
    this->_lock.Unlock();
  }
  return (col);
}

void
LcdVar::setCol(size_t col_)
{
  if (this->_lock.Lock())
  {
    this->_col = col_;
    this->_lock.Unlock();
  }
}

ssize_t
LcdVar::GetRow()
{
  ssize_t row = -1;
  if (this->_lock.Lock())
  {
    row = this->_row;
    this->_lock.Unlock();
  }
  return (row);
}

void
LcdVar::setRow(size_t row_)
{
  if (this->_lock.Lock())
  {
    this->_row = row_;
    this->_lock.Unlock();
  }
}

ssize_t
LcdVar::GetLength()
{
  ssize_t size = -1;
  if (this->_lock.Lock())
  {
    size = this->_len;
    this->_lock.Unlock();
  }
  return (size);
}

void
LcdVar::setLength(size_t size_)
{
  if (this->_lock.Lock())
  {
    this->_len = size_;
    this->_lock.Unlock();
  }
}

}
}
