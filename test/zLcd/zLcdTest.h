/*
 * zLcdTest.h
 *
 *  Created on: Jan 16, 2016
 *      Author: kmahoney
 */

#ifndef _ZLCDTEST_H_
#define _ZLCDTEST_H_

#include "UnitTest.h"

#include <zutils/zLcd.h>

int
zLcdTest_LcdVarDefaults(void *arg_);
int
zLcdTest_LcdDefaults(void *arg_);
int
zLcdTest_LcdVarGetSet(void *arg_);
int
zLcdTest_LcdCreateVar(void *arg_);
int
zLcdTest_LcdUpdate(void *arg_);

using namespace Test;
using namespace zUtils;

class TestLcd : public zLcd::Lcd
{

public:

  TestLcd()
  {
  }

  virtual
  ~TestLcd()
  {
  }

  std::string
  GetBuffer()
  {
    return (this->_buf);
  }

protected:

private:

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
