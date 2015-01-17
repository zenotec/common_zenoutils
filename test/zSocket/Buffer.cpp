#include <string.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_SocketBufferCompare(void* arg_)
{

  // Create new packet and validate
  zSocket::Buffer mySb1;
  TEST_ISNOT_NULL(mySb1.Head());
  TEST_EQ(mySb1.Head(), mySb1.Data());
  TEST_ISNOT_NULL(mySb1.Data());
  TEST_IS_ZERO(mySb1.Length());
  TEST_IS_ZERO(mySb1.Size());
  TEST_EQ(mySb1.TotalSize(), 1500);
  TEST_EQ(std::string(""), mySb1.Str());

  // Create new packet of set size and validate
  zSocket::Buffer mySb2;
  TEST_ISNOT_NULL(mySb2.Head());
  TEST_EQ(mySb2.Head(), mySb2.Data());
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

