#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_BufferDefaults(void* arg_)
{

  // Create new packet and validate
  zSocket::Buffer MySB1;
  TEST_ISNOT_NULL(MySB1.Head());
  TEST_EQ(MySB1.Head(), MySB1.Data());
  TEST_IS_ZERO(MySB1.Length());
  TEST_IS_ZERO(MySB1.Size());
  TEST_EQ(MySB1.TotalSize(), 1500);

  // Create new packet of set size and validate
  zSocket::Buffer MySB2(500);
  TEST_ISNOT_NULL(MySB2.Head());
  TEST_EQ(MySB1.Head(), MySB1.Data());
  TEST_ISNOT_NULL(MySB2.Data());
  TEST_IS_ZERO(MySB2.Length());
  TEST_IS_ZERO(MySB2.Size());
  TEST_EQ(MySB2.TotalSize(), 500);

  // Return success
  return (0);

}

int
zSocketTest_AddressDefaults(void* arg_)
{
  // Create new socket address and validate
  zSocket::Address myAddr1;
  TEST_EQ(zSocket::Address::TYPE_ERR, myAddr1.GetType());
  TEST_EQ(std::string(""), myAddr1.GetAddr());

  // Create Socket address using string notation
  zSocket::Address myAddr2(zSocket::Address::TYPE_INET, "1.2.3.4:56789");
  TEST_EQ(zSocket::Address::TYPE_INET, myAddr2.GetType());
  TEST_EQ(std::string("1.2.3.4:56789"), myAddr2.GetAddr());

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
  TestSocket mySocket;

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
