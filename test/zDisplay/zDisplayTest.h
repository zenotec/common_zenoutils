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
    this->_buf = (char*) calloc(cols_, rows_);
    this->_buflen = (cols_ * rows_);
    this->_lock.unlock();
  }

  virtual
  ~TestDisplay()
  {
    this->_lock.lock();
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
    this->_lock.lock();
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
    this->_lock.unlock();
    return (str);
  }

protected:

private:

  MUTEX _lock;
  char* _buf;
  int _buflen;

  virtual bool
  update(const zDisplay::DisplayBuffer& buf_)
  {
    this->_lock.lock();
    memset(this->_buf, 0, this->_buflen);
    for (int i = 0; i < this->_buflen; i++)
    {
      this->_buf[i] = buf_[i];
    }
    this->_lock.unlock();
    return (true);
  }

  void
  clear()
  {
    this->_lock.lock();
    memset(this->_buf, 0, this->_buflen);
    this->_lock.unlock();
  }

  void
  print()
  {
    std::cout << this->GetBuffer() << std::endl;
  }

};

#endif /* _ZLCDTEST_H_ */
