/*
 * Lcd.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: kmahoney
 */

#include <zutils/zLog.h>
#include <zutils/zLcd.h>

namespace zUtils
{
namespace zLcd
{

Lcd::Lcd() :
    _rows(0), _cols(0)
{
  this->_timer.RegisterObserver(this);
  this->_lock.Unlock();
}

Lcd::~Lcd()
{
  this->_timer.Stop();
}

bool
Lcd::SetLimits(const size_t rows_, const size_t cols_)
{
  this->_disp_buf.resize(rows_);
  this->_rows = rows_;
  for (int row = 0; row < rows_; row++)
  {
    this->_disp_buf[row].resize(cols_);
  }
  this->_cols = cols_;
}

bool
Lcd::SetRefresh(const size_t rate_)
{
  // Convert from rate in Hz to microseconds
  uint32_t usec;
  usec = 1000000 / rate_;
  ZLOG_DEBUG("Setting refresh rate to: " + zLog::IntStr(rate_) + "(" + zLog::IntStr(usec) + ")");
  this->_timer.Start(usec);
}

LcdVar *
Lcd::CreateVar(const std::string &name_, const size_t row_, const size_t col_,
    const size_t len_)
{
  LcdVar *var = NULL;
  if ((row_ < this->_rows) && ((col_ + len_) < this->_cols))
  {
    var = new LcdVar(name_);
    var->setRow(row_);
    var->setCol(col_);
    var->setLength(len_);
    if (this->_lock.Lock())
    {
      this->_vars.push_back(var);
      this->_lock.Unlock();
    }
    else
    {
      delete (var);
      var = NULL;
    }
  }
  return (var);
}

bool
Lcd::DeleteVar(zLcd::LcdVar *var_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    this->_vars.remove(var_);
    this->_lock.Unlock();
    status = true;
  }
  return (status);
}

bool
Lcd::Update(const std::string &str_, const size_t row_, const size_t col_)
{
  bool status = false;
  ZLOG_INFO("Updating display buffer: [" + zLog::IntStr(row_) + "," + zLog::IntStr(col_) + "]: '" + str_ + "'");
  ZLOG_DEBUG("Limits: " + zLog::IntStr(this->_rows) + "," + zLog::IntStr(this->_cols) + ": " + zLog::IntStr(str_.size()));
  if ((row_ < this->_rows) && (col_ < this->_cols))
  {
    if (this->_lock.Lock())
    {
      for (int i = 0, row = row_; ((i < str_.size()) && (row < this->_rows)); row++)
      {
        for (int col = col_; ((i < str_.size()) && (col < this->_cols)); i++, col++)
        {
          ZLOG_DEBUG("Updating display char: " + zLog::IntStr(row) + "," + zLog::IntStr(col) + " = " + str_[i]);
          this->_disp_buf[row][col] = str_[i];
        }
      }
      status = true;
      this->_lock.Unlock();
    }
  }
  return (status);
}

bool
Lcd::Clear()
{
  bool status = false;
  if (this->_lock.Lock())
  {
    for (int row = 0; row < this->_rows; row++)
    {
      for (int col = 0; col < this->_cols; col++)
      {
        this->_disp_buf[row][col] = 0;
      }
    }
    status = true;
    this->_clear();
    this->_lock.Unlock();
  }
  return (status);
}

bool
Lcd::EventHandler(zEvent::Event *event_, void *arg_)
{
  bool status = false;
  if (event_ && (event_->GetType() == zEvent::Event::TYPE_TIMER) && this->_lock.Lock())
  {
    ZLOG_DEBUG("Handling LCD timer event");
    // Iterate over all variables and update buffer
    std::list<zLcd::LcdVar *>::iterator it = this->_vars.begin();
    std::list<zLcd::LcdVar *>::iterator end = this->_vars.end();
    for (; it != end; ++it)
    {
      size_t row = (*it)->GetRow();
      size_t col = (*it)->GetCol();
      size_t len = (*it)->GetLength();
      std::string val = (*it)->GetValue();
      for (int i = 0; i < len; i++, col++)
      {
        this->_disp_buf[row][col] = val[i];
      }
    }
    this->_update(this->_disp_buf);
    status = true;
    this->_lock.Unlock();
  }
  return (status);
}

}
}
