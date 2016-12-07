/*
 * DisplayVar.cpp
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
// Class: zDisplay::DisplayVar
//*****************************************************************************

DisplayVar::DisplayVar(const std::string &name_, const size_t size_) :
    _name(name_), DisplayBuffer(size_), _row(0), _col(0)
{
}

DisplayVar::~DisplayVar()
{
}

bool
DisplayVar::operator==(const DisplayVar &other_) const
{
  bool status = true;
  status |= (this->GetName() == other_.GetName());
  status |= (this->GetRow() == other_.GetRow());
  status |= (this->GetColumn() == other_.GetColumn());
  status |= (this->GetSize() == other_.GetSize());
  return (status);
}

bool
DisplayVar::operator!=(const DisplayVar &other_) const
{
  bool status = true;
  status |= (this->GetName() == other_.GetName());
  status |= (this->GetRow() == other_.GetRow());
  status |= (this->GetColumn() == other_.GetColumn());
  status |= (this->GetSize() == other_.GetSize());
  return (!status);
}

std::string
DisplayVar::GetName() const
{
  return (this->_name);
}

ssize_t
DisplayVar::GetColumn() const
{
  return (this->_col);
}

bool
DisplayVar::SetColumn(size_t col_)
{
  this->_col = col_;
  return (true);
}

ssize_t
DisplayVar::GetRow() const
{
  return (this->_row);
}

bool
DisplayVar::SetRow(size_t row_)
{
  this->_row = row_;
  return (true);
}

}
}
