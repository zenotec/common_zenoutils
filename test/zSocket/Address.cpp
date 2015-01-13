#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;


int
zSocketTest_AddressGetSet(void* arg_)
{
  // Create new socket address and validate
  zSocket::Address myAddr(zSocket::Address::TYPE_INET);
  TEST_EQ(std::string(""), myAddr.GetAddr());
  TEST_TRUE(myAddr.SetAddr(std::string("some_address")));
  TEST_EQ(std::string("some_address"), myAddr.GetAddr());

  // Return success
  return (0);
}

int
zSocketTest_AddressCompare(void* arg_)
{
  // Create new socket address and validate
  zSocket::Address myAddr1(zSocket::Address::TYPE_INET);
  TEST_EQ(std::string(""), myAddr1.GetAddr());

  // Create another new socket address and validate
  zSocket::Address myAddr2(zSocket::Address::TYPE_INET);
  TEST_EQ(std::string(""), myAddr2.GetAddr());

  // Compare the addresses
  TEST_TRUE(myAddr1 == myAddr2);
  TEST_FALSE(myAddr1 != myAddr2);

  // Set first address to new value and validate
  std::string expAddr = "some_address";
  TEST_TRUE(myAddr1.SetAddr(expAddr));
  TEST_EQ(expAddr, myAddr1.GetAddr());

  // Compare the addresses
  TEST_TRUE(myAddr1 != myAddr2);
  TEST_FALSE(myAddr1 == myAddr2);

  // Set first address to new value and validate
  TEST_TRUE(myAddr2.SetAddr(expAddr));
  TEST_EQ(expAddr, myAddr2.GetAddr());

  // Compare the addresses
  TEST_TRUE(myAddr1 == myAddr2);
  TEST_FALSE(myAddr1 != myAddr2);

  // Return success
  return (0);
}

