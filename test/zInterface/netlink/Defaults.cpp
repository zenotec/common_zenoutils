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
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include "UnitTest.h"
#include "NetlinkTest.h"
using namespace Test;

#include <zutils/netlink/GenericMessage.h>

int
NetlinkTest_GenericMessage(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# NetlinkTest_GenericMessage()");
  ZLOG_DEBUG("#############################################################");

  struct nl_msg* msg = nlmsg_alloc();

  // Assemble generic message with a variety of attributes

  netlink::GenericMessage MyMessage1;
  TEST_IS_ZERO(MyMessage1.GetFamily());
  TEST_IS_ZERO(MyMessage1.GetFlags());
  TEST_IS_ZERO(MyMessage1.GetCommand());

  netlink::Attribute MyNestedAttr1(1, true);
  TEST_EQ(1, MyNestedAttr1.GetId());
  TEST_IS_ZERO(MyNestedAttr1.GetLength());

  netlink::Attribute MyAttr1(2);
  TEST_EQ(2, MyAttr1.GetId());
  TEST_IS_ZERO(MyAttr1.GetLength());

  uint32_t ExpVal = 0x12345678;
  uint32_t ObsVal = 0;

  TEST_TRUE(MyAttr1.Set(ExpVal));
  TEST_EQ(sizeof(ExpVal), MyAttr1.GetLength());

  TEST_TRUE(MyNestedAttr1.Put(MyAttr1));

  TEST_TRUE(MyMessage1.PutAttribute(MyNestedAttr1));

  TEST_TRUE(MyMessage1.Assemble(msg));

  // Disassemble the generic message contents assembled above back into its parts

  netlink::GenericMessage MyMessage2;
  TEST_IS_ZERO(MyMessage2.GetFamily());
  TEST_IS_ZERO(MyMessage2.GetFlags());
  TEST_IS_ZERO(MyMessage2.GetCommand());

  TEST_TRUE(MyMessage2.Disassemble(msg));

  netlink::Attribute MyNestedAttr2(1, true);
  TEST_EQ(1, MyNestedAttr2.GetId());
  TEST_TRUE(MyMessage1.GetAttribute(MyNestedAttr2));

  netlink::Attribute MyAttr2(2);
  TEST_EQ(2, MyAttr2.GetId());
  TEST_IS_ZERO(MyAttr2.GetLength());
  TEST_TRUE(MyNestedAttr2.Get(MyAttr2));

  // Return success
  UTEST_RETURN(UTEST_PASS);
}


