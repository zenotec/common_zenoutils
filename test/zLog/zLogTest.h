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

#ifndef _ZLOGTEST_H_
#define _ZLOGTEST_H_

#include "UnitTest.h"
#include <zutils/zSem.h>
#include <zutils/zQueue.h>
#include <zutils/zLog.h>

int
zLogTest_Defaults(void* arg_);
int
zLogTest_FileConnector(void* arg_);

using namespace zUtils;

class TestConnector : public zLog::Connector
{
public:

  zQueue<std::string> MsgQueue;

  virtual void
  Logger(std::string msg_)
  {
    this->MsgQueue.Push(msg_);
  }
};

#endif /* _ZLOGTEST_H_ */
