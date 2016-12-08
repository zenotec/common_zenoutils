/*
 * zDisplayTest.h
 *
 *  Created on: Jan 16, 2016
 *      Author: kmahoney
 */

#ifndef _ZLCDTEST_H_
#define _ZLCDTEST_H_

#include <stdlib.h>

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
    zDisplay::Display(cols_, rows_), _buf(NULL), _buflen(0)
  {
    this->_buf = (char*)calloc(cols_, rows_);
    this->_buflen = (cols_ * rows_);
  }

  virtual
  ~TestDisplay()
  {
    if (this->_buf)
    {
      free(this->_buf);
      this->_buf = NULL;
    }
  }

  std::string
  GetBuffer()
  {
    std::string str;
    for (int i = 0; i < this->_buflen; i++)
    {
      if (this->_buf[i])
      {
        str += this->_buf[i];
      }
      else
      {
        str += '.';
      }
    }
    return (str);
  }

protected:

private:

  char* _buf;
  int _buflen;

  virtual bool
  update(const zDisplay::DisplayBuffer& buf_)
  {
    this->clear();
    for (int i = 0; i < this->_buflen; i++)
    {
      this->_buf[i] = buf_[i];
    }
    return(true);
  }

  void
  clear()
  {
    memset(this->_buf, 0, this->_buflen);
  }

  void
  print()
  {
    std::cout << this->GetBuffer() << std::endl;
  }

};

#endif /* _ZLCDTEST_H_ */
