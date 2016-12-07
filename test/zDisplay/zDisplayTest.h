/*
 * zDisplayTest.h
 *
 *  Created on: Jan 16, 2016
 *      Author: kmahoney
 */

#ifndef _ZLCDTEST_H_
#define _ZLCDTEST_H_

#include "UnitTest.h"

#include <zutils/zDisplay.h>

int
zDisplayTest_DisplayBufferDefaults(void *arg_);
int
zDisplayTest_DisplayVarDefaults(void *arg_);
int
zDisplayTest_DisplayDefaults(void *arg_);
int
zDisplayTest_DisplayBufferUpdate(void *arg_);
int
zDisplayTest_DisplayBufferCopy(void *arg_);
int
zDisplayTest_DisplayVarGetSet(void *arg_);
int
zDisplayTest_DisplayCreateVar(void *arg_);
int
zDisplayTest_DisplayUpdate(void *arg_);

using namespace Test;
using namespace zUtils;

class TestDisplay : public zDisplay::Display
{

public:

  TestDisplay(const size_t cols_, const size_t rows_ = 1) :
    zDisplay::Display(cols_, rows_)
  {
  }

  virtual
  ~TestDisplay()
  {
  }

  std::string
  GetBuffer()
  {
    return (this->_buf);
  }

protected:

private:

  virtual bool
  update(const zDisplay::DisplayBuffer& buf_)
  {
    return(false);
  }

  std::string _buf;
  virtual bool
  _update(std::vector<std::vector<char> > &buf_)
  {
    this->_buf.clear();
    for (int row = 0; row < buf_.size(); row++)
    {
      for (int col = 0; col < buf_[row].size(); col++)
      {
        ZLOG_DEBUG("Updating display: " + ZLOG_INT(row) + "," + ZLOG_INT(col) + " = " + buf_[row][col]);
        if (buf_[row][col])
        {
          this->_buf += buf_[row][col];
        }
      }
      this->_buf += '\n';
    }
  }

  virtual bool
  _clear()
  {
    this->_buf.clear();
  }

};

#endif /* _ZLCDTEST_H_ */
