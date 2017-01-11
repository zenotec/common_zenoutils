/*
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

#ifndef _ZOPTTEST_H_
#define _ZOPTTEST_H_

#include <zutils/zProgram.h>

#include "UnitTest.h"

int
zProgramTest_ProgramOptionDefaults(void* arg_);
int
zProgramTest_ProgramArgumentDefaults(void* arg_);
int
zProgramTest_ProgramDefaults(void* arg_);

int
zProgramTest_ProgramOptionString(void* arg_);

int
zProgramTest_ProgramArgumentString(void* arg_);

int
zProgramTest_ParseCommandLine(void* arg_);

using namespace Test;
using namespace zUtils;
using namespace zUtils::zProgram;

class TestProgram : public Program
{
public:

  TestProgram()
  {
  }

  virtual
  ~TestProgram()
  {
  }

  virtual void
  Run(zThread::ThreadArg *arg_)
  {

  }

protected:

private:

};

#endif /* _ZOPTTEST_H_ */
