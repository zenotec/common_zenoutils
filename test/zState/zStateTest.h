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

#ifndef _ZSTATETEST_H_
#define _ZSTATETEST_H_

#include <string>

#include <zutils/zState.h>

#include "UnitTest.h"

int
zStateTest_Defaults(void* arg_);

using namespace Test;
using namespace zUtils;

class TestStateUpper;
class TestStateLower;

enum STATE_ID
{
  ID_ERR = -1,
  ID_NONE = 0,
  ID_INITIAL = 0,
  ID_UPPER = 1,
  ID_LOWER = 2,
  ID_LAST
};

//**********************************************************************
// Class: TestNotification
//**********************************************************************

class TestNotification :
    public zState::Notification
{

public:

  TestNotification(zState::Context& ctx_) :
    zState::Notification(ctx_)
  {
  }

  virtual
  ~TestNotification()
  {
  }

  std::string
  GetString() const
  {
    return (this->_str);
  }

  bool
  SetString(const std::string& str_)
  {
    this->_str = str_;
    return (true);
  }

protected:

private:

  std::string _str;

};

//**********************************************************************
// Class: TestStateUpper
//**********************************************************************

class TestStateUpper :
    public zState::State
{

public:

  TestStateUpper(SHPTR(zState::Context) context_);

  virtual
  ~TestStateUpper();

protected:

  virtual zEvent::STATUS
  ObserveEvent(SHPTR(zEvent::Notification) n_);

private:

};

//**********************************************************************
// Class: TestStateLower
//**********************************************************************

class TestStateLower :
    public zState::State
{

public:

  TestStateLower(SHPTR(zState::Context) context_);

  virtual
  ~TestStateLower();

protected:

  virtual zEvent::STATUS
  ObserveEvent(SHPTR(zEvent::Notification) n_);

private:

};

#endif /* _ZSTATETEST_H_ */
