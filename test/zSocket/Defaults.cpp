#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_BufferDefaults(void* arg_)
{

  // Create new packet and validate
  zSocket::Buffer mySb1;
  TEST_ISNOT_NULL(mySb1.Head());
  TEST_EQ(mySb1.Head(), mySb1.Data());
  TEST_IS_ZERO(mySb1.Length());
  TEST_IS_ZERO(mySb1.Size());
  TEST_EQ(mySb1.TotalSize(), 1500);
  TEST_EQ(std::string(""), mySb1.Str());

  // Create new packet of set size and validate
  zSocket::Buffer mySb2(500);
  TEST_ISNOT_NULL(mySb2.Head());
  TEST_EQ(mySb1.Head(), mySb1.Data());
  TEST_ISNOT_NULL(mySb2.Data());
  TEST_IS_ZERO(mySb2.Length());
  TEST_IS_ZERO(mySb2.Size());
  TEST_EQ(mySb2.TotalSize(), 500);
  TEST_EQ(std::string(""), mySb2.Str());

  // Return success
  return (0);

}

int
zSocketTest_AddressDefaults(void* arg_)
{
  // Create new socket address and validate
  zSocket::Address myAddr1;
  TEST_EQ(zSocket::Address::TYPE_NONE, myAddr1.GetType());
  TEST_EQ(std::string(""), myAddr1.GetAddress());

  // Create Socket address using string notation
  zSocket::Address myAddr2(zSocket::Address::TYPE_INET, "1.2.3.4:56789");
  TEST_EQ(zSocket::Address::TYPE_INET, myAddr2.GetType());
  TEST_EQ(std::string("1.2.3.4:56789"), myAddr2.GetAddress());

  // Return success
  return (0);
}

int
zSocketTest_ObserverDefaults(void* arg_)
{
  TestObserver myObserver;

  // Return success
  return (0);
}

int
zSocketTest_SocketDefaults(void* arg_)
{
  // Create new socket address and validate
  zSocket::Address myAddr;
  TEST_EQ(zSocket::Address::TYPE_NONE, myAddr.GetType());
  TEST_EQ(std::string(""), myAddr.GetAddress());

  TestSocket mySocket(&myAddr);

  // Return success
  return (0);

}

int
zSocketTest_HandlerDefaults(void* arg_)
{
  zSocket::Handler myHandler;
  // Return success
  return (0);
}
