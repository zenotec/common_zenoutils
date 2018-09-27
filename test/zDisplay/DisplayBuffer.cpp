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

#include <signal.h>

#include <string>
#include <mutex>
#include <list>
#include <vector>
#include <map>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zQueue.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zTimer.h>
#include <zutils/zSocket.h>
#include <zutils/zMessage.h>
#include <zutils/zCommand.h>

#include <zutils/zDisplay.h>

#include "UnitTest.h"
#include "zDisplayTest.h"

using namespace Test;
using namespace zUtils;

int
zDisplayTest_DisplayBufferUpdate(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDisplayTest_DisplayBufferUpdate()");
  ZLOG_DEBUG("#############################################################");

  std::string str;

  // Create new display buffer and verify
  zDisplay::DisplayBuffer *MyBuffer = new zDisplay::DisplayBuffer(10);
  TEST_ISNOT_NULL(MyBuffer);
  TEST_EQ(1, MyBuffer->GetRows());
  TEST_EQ(10, MyBuffer->GetColumns());
  TEST_EQ(10, MyBuffer->GetSize());
  TEST_TRUE(MyBuffer->GetString().empty());
  TEST_EQ(std::string(""), MyBuffer->GetString());

  // Update and verify (partial)
  str = "val";
  TEST_TRUE(MyBuffer->Update(str));
  TEST_EQ(3, MyBuffer->GetString().size());
  TEST_EQ(str, MyBuffer->GetString());

  // Clear and verify
  MyBuffer->Clear();
  TEST_TRUE(MyBuffer->GetString().empty());
  TEST_EQ(std::string(""), MyBuffer->GetString());

  // Update and verify (full)
  str = std::string("0123456789");
  TEST_TRUE(MyBuffer->Update(str));
  TEST_EQ(10, MyBuffer->GetString().size());
  TEST_EQ(str, MyBuffer->GetString());

  // Clear and verify
  MyBuffer->Clear();
  TEST_TRUE(MyBuffer->GetString().empty());
  TEST_EQ(std::string(""), MyBuffer->GetString());

  // Update and verify (overfill)
  str = std::string("val-0123456789");
  TEST_EQ(10, MyBuffer->Update(str));
  TEST_EQ(10, MyBuffer->GetString().size());
  TEST_EQ(std::string("val-012345"), MyBuffer->GetString());

  // Cleanup
  delete (MyBuffer);

  // Return success
  return (0);

}

int
zDisplayTest_DisplayBufferCopy(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDisplayTest_DisplayBufferCopy()");
  ZLOG_DEBUG("#############################################################");

  // Create new display buffer and verify
  zDisplay::DisplayBuffer *MyBuffer1 = new zDisplay::DisplayBuffer(10);
  TEST_ISNOT_NULL(MyBuffer1);
  TEST_EQ(1, MyBuffer1->GetRows());
  TEST_EQ(10, MyBuffer1->GetColumns());
  TEST_EQ(10, MyBuffer1->GetSize());
  TEST_TRUE(MyBuffer1->GetString().empty());
  TEST_EQ(std::string(""), MyBuffer1->GetString());

  // Create new display buffer and verify
  zDisplay::DisplayBuffer *MyBuffer2 = new zDisplay::DisplayBuffer(10);
  TEST_ISNOT_NULL(MyBuffer2);
  TEST_EQ(1, MyBuffer2->GetRows());
  TEST_EQ(10, MyBuffer2->GetColumns());
  TEST_EQ(10, MyBuffer2->GetSize());
  TEST_TRUE(MyBuffer2->GetString().empty());
  TEST_EQ(std::string(""), MyBuffer2->GetString());

  // Make sure buffers are equal
  TEST_TRUE(*MyBuffer1 == *MyBuffer2);

  // Update first buffer
  TEST_EQ(5, MyBuffer1->Update(std::string("value")));
  TEST_EQ(std::string("value"), MyBuffer1->GetString());

  // Make sure buffers are NOT equal
  TEST_TRUE(*MyBuffer1 != *MyBuffer2);

  // Copy the first buffer to the second
  *MyBuffer2 = *MyBuffer1;

  // Make sure buffers are equal
  TEST_TRUE(*MyBuffer1 == *MyBuffer2);

  // Cleanup
  delete (MyBuffer1);
  delete (MyBuffer2);

  // Return success
  return (0);

}
