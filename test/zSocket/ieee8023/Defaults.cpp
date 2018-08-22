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

#include <zutils/zLog.h>
#include <zutils/ieee8023/Frame.h>
using namespace zUtils;

#include "Ieee8023Test.h"
#include "UnitTest.h"

ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

int
Ieee8023Test_Defaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee8023Test_Defaults()");
  ZLOG_DEBUG("#############################################################");

  zSocket::ieee8023::Frame f;
  TEST_EQ(f.GetType(), zSocket::ieee8023::Frame::TYPE_NONE);
  TEST_EQ(f.GetProto(), zSocket::ieee8023::Frame::PROTO_NONE);
  TEST_IS_ZERO(f.GetPayloadLength());

  // Return success
  return (UTEST_PASS);
}

int
Ieee8023Test_GetSet(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee8023Test_GetSet()");
  ZLOG_DEBUG("#############################################################");

  zSocket::ieee8023::Frame f;
  TEST_EQ(f.GetType(), zSocket::ieee8023::Frame::TYPE_NONE);
  TEST_EQ(f.GetProto(), zSocket::ieee8023::Frame::PROTO_NONE);
  TEST_IS_ZERO(f.GetPayloadLength());

  // Return success
  return (UTEST_PASS);
}

int
Ieee8023Test_Assemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee8023Test_Assemble()");
  ZLOG_DEBUG("#############################################################");

  zSocket::ieee8023::Frame f;

  // Return success
  return (UTEST_PASS);
}

int
Ieee8023Test_Disassemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee8023Test_Disassemble()");
  ZLOG_DEBUG("#############################################################");

  zSocket::ieee8023::Frame f;

  // Return success
  return (UTEST_PASS);
}
