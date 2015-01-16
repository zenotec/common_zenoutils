#include "zSocketTest.h"

#include <zutils/zInetSocket.h>

using namespace Test;
using namespace zUtils;

int
zSocketTest_InetAddressGet(void* arg_)
{
  struct sockaddr_in expSockAddr, obsSockAddr;
  expSockAddr.sin_family = AF_INET;
  expSockAddr.sin_addr.s_addr = htonl(0x01020304);
  expSockAddr.sin_port = htons(56789);

  // Create new socket address and validate
  zSocket::InetAddress myAddr;
  TEST_EQ(std::string("0.0.0.0:0"), myAddr.GetAddrString());

  // Set socket address using string notation
  std::string expAddrString = "1.2.3.4:56789";
  myAddr.SetAddr(expAddrString);
  TEST_EQ(expAddrString, myAddr.GetAddrString());

  obsSockAddr = myAddr.GetAddrSockAddr();
  TEST_EQ(expSockAddr.sin_family, obsSockAddr.sin_family);
  TEST_TRUE(expSockAddr.sin_addr.s_addr == obsSockAddr.sin_addr.s_addr);
  TEST_EQ(expSockAddr.sin_port, obsSockAddr.sin_port);

  // Return success
  return (0);

}

int
zSocketTest_InetAddressSet(void* arg_)
{

  // Create new socket address and validate
  zSocket::InetAddress myAddr;
  TEST_EQ(std::string("0.0.0.0:0"), myAddr.GetAddrString());

  // Set socket address using socket address
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(0x01020304);
  addr.sin_port = htons(56789);
  TEST_TRUE(myAddr.SetAddr(addr));
  std::string expAddrString = "1.2.3.4:56789";
  TEST_EQ(expAddrString, myAddr.GetAddrString());

  // Return success
  return (0);

}

int
zSocketTest_InetAddressCompare(void* arg_)
{

  // Create new socket address and validate
  zSocket::Address myAddr(zSocket::Address::TYPE_INET, std::string("0.0.0.0:0"));
  TEST_EQ(std::string("0.0.0.0:0"), myAddr.GetAddr());

  // Create new socket address and validate
  zSocket::InetAddress myAddr1;
  TEST_EQ(std::string("0.0.0.0:0"), myAddr1.GetAddrString());

  // Create second socket address and validate
  zSocket::InetAddress myAddr2;
  TEST_EQ(std::string("0.0.0.0:0"), myAddr2.GetAddrString());

  // Compare address (match)
  TEST_TRUE(myAddr1 == myAddr2);
  TEST_FALSE(myAddr1 != myAddr2);
  TEST_TRUE(myAddr1 == zSocket::InetAddress(myAddr));
  TEST_TRUE(myAddr2 == zSocket::InetAddress(myAddr));

  // Set socket address using string notation
  myAddr1.SetAddr("1.2.3.4:56789");
  TEST_EQ(std::string("1.2.3.4:56789"), myAddr1.GetAddrString());

  // Compare address (no match)
  TEST_FALSE(myAddr1 == myAddr2);
  TEST_TRUE(myAddr1 != myAddr2);
  TEST_TRUE(myAddr1 != zSocket::InetAddress(myAddr));
  TEST_TRUE(myAddr2 == zSocket::InetAddress(myAddr));

  // Set socket address using string notation
  myAddr2.SetAddr("1.2.3.4:56789");
  TEST_EQ(std::string("1.2.3.4:56789"), myAddr2.GetAddrString());

  // Compare address (match)
  TEST_TRUE(myAddr1 == myAddr2);
  TEST_FALSE(myAddr1 != myAddr2);

  // Set socket address using string notation
  myAddr1.SetAddr("1.2.3.4:56788");
  TEST_EQ(std::string("1.2.3.4:56788"), myAddr1.GetAddrString());

  // Compare address (no match)
  TEST_FALSE(myAddr1 == myAddr2);
  TEST_TRUE(myAddr1 != myAddr2);

  // Return success
  return (0);

}


