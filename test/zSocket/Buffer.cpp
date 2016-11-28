#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <netinet/in.h>
#include <string.h>

#include <string>
#include <list>
#include <mutex>
#include <memory>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>

#include <zutils/zSocket.h>
#include <zutils/zLoopSocket.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_BufferCompare(void* arg_)
{

  // Create new packet and validate
  zSocket::SocketBuffer mySb1;
  TEST_ISNOT_NULL(mySb1.Head());
  TEST_EQ(mySb1.Head(), mySb1.Data());
  TEST_EQ(mySb1.Head(), mySb1.Tail());
  TEST_ISNOT_NULL(mySb1.Data());
  TEST_IS_ZERO(mySb1.Length());
  TEST_IS_ZERO(mySb1.Size());
  TEST_EQ(mySb1.TotalSize(), 1500);
  TEST_EQ(std::string(""), mySb1.Str());

  // Create new packet of set size and validate
  zSocket::SocketBuffer mySb2;
  TEST_ISNOT_NULL(mySb2.Head());
  TEST_EQ(mySb2.Head(), mySb2.Data());
  TEST_EQ(mySb2.Head(), mySb2.Tail());
  TEST_ISNOT_NULL(mySb2.Data());
  TEST_IS_ZERO(mySb2.Length());
  TEST_IS_ZERO(mySb2.Size());
  TEST_EQ(mySb2.TotalSize(), 1500);
  TEST_EQ(std::string(""), mySb2.Str());

  // Validate
  TEST_TRUE((mySb1 == mySb2));
  TEST_EQ(mySb1.Str(), mySb2.Str());

  // Write to first packet and validate
  memset(mySb1.Data(), 0xed, 100);
  mySb1.Put(100);
  TEST_TRUE((mySb1 != mySb2));
  TEST_NEQ(mySb1.Str(), mySb2.Str());

  // Write to second packet and validate
  memset(mySb2.Data(), 0xde, 100);
  mySb2.Put(100);
  TEST_TRUE((mySb1 != mySb2));
  TEST_NEQ(mySb1.Str(), mySb2.Str());

  // Write to second packet and validate
  memset(mySb2.Data(), 0xed, 100);
  TEST_TRUE((mySb1 == mySb2));
  TEST_EQ(mySb1.Str(), mySb2.Str());

  // Return success
  return (0);

}

int
zSocketTest_BufferString(void* arg_)
{

  // Create new packet and validate
  zSocket::SocketBuffer myBuffer;
  TEST_ISNOT_NULL(myBuffer.Head());
  TEST_EQ(myBuffer.Head(), myBuffer.Data());
  TEST_EQ(myBuffer.Head(), myBuffer.Tail());
  TEST_ISNOT_NULL(myBuffer.Data());
  TEST_IS_ZERO(myBuffer.Length());
  TEST_IS_ZERO(myBuffer.Size());
  TEST_EQ(myBuffer.TotalSize(), 1500);
  TEST_EQ(std::string(""), myBuffer.Str());

  // Copy string to buffer
  std::string expStr = "Test Buffer String";
  TEST_TRUE(myBuffer.Str(expStr));
  TEST_EQ(expStr, myBuffer.Str());
  TEST_EQ((int )expStr.size(), (int )myBuffer.Size());

  // Return success
  return (0);

}
