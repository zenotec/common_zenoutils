#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_SocketAddrSet(void* arg_)
{

  // Create new socket address and validate
  zSocket::SocketAddr MySockAddr;
  TEST_EQ(MySockAddr.GetIpAddrStr(), std::string("0.0.0.0"));
  TEST_EQ(MySockAddr.GetPortStr(), std::string("0"));

  // Set socket address using string notation
  MySockAddr.SetAddr("1.2.3.4:56789");
  TEST_EQ(MySockAddr.GetIpAddrStr(), std::string("1.2.3.4"));
  TEST_EQ(MySockAddr.GetPortStr(), std::string("56789"));

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(0x09080706);
  addr.sin_port = htons(54321);
  MySockAddr.SetAddr(addr);
  TEST_EQ(MySockAddr.GetIpAddrStr(), std::string("9.8.7.6"));
  TEST_EQ(MySockAddr.GetPortStr(), std::string("54321"));

  // Return success
  return (0);

}

int
zSocketTest_SocketAddrGet(void* arg_)
{
  struct sockaddr_in addr_exp, addr_obs;
  addr_exp.sin_family = AF_INET;
  addr_exp.sin_addr.s_addr = htonl(0x01020304);
  addr_exp.sin_port = htons(56789);

  // Create new socket address and validate
  zSocket::SocketAddr MySockAddr;
  TEST_EQ(MySockAddr.GetIpAddrStr(), std::string("0.0.0.0"));
  TEST_EQ(MySockAddr.GetPortStr(), std::string("0"));

  // Set socket address using string notation
  MySockAddr.SetAddr("1.2.3.4:56789");
  TEST_EQ(MySockAddr.GetIpAddrStr(), std::string("1.2.3.4"));
  TEST_EQ(MySockAddr.GetPortStr(), std::string("56789"));

  addr_obs = MySockAddr.GetAddr();
  TEST_EQ(addr_exp.sin_family, addr_obs.sin_family);
  TEST_EQ(addr_exp.sin_addr.s_addr, addr_obs.sin_addr.s_addr);
  TEST_EQ(addr_exp.sin_port, addr_obs.sin_port);

  // Return success
  return (0);

}

int
zSocketTest_SocketAddrCompare(void* arg_)
{

  // Create new socket address and validate
  zSocket::SocketAddr MySockAddr1;
  TEST_EQ(MySockAddr1.GetIpAddrStr(), std::string("0.0.0.0"));
  TEST_EQ(MySockAddr1.GetPortStr(), std::string("0"));

  // Create second socket address and validate
  zSocket::SocketAddr MySockAddr2;
  TEST_EQ(MySockAddr2.GetIpAddrStr(), std::string("0.0.0.0"));
  TEST_EQ(MySockAddr2.GetPortStr(), std::string("0"));

  // Compare address (match)
  TEST_TRUE( MySockAddr1 == MySockAddr2);

  // Set socket address using string notation
  MySockAddr1.SetAddr("1.2.3.4:56789");
  TEST_EQ(MySockAddr1.GetIpAddrStr(), std::string("1.2.3.4"));
  TEST_EQ(MySockAddr1.GetPortStr(), std::string("56789"));

  // Compare address (no match)
  TEST_TRUE( MySockAddr1 != MySockAddr2);

  // Set socket address using string notation
  MySockAddr2.SetAddr("1.2.3.4:56789");
  TEST_EQ(MySockAddr2.GetIpAddrStr(), std::string("1.2.3.4"));
  TEST_EQ(MySockAddr2.GetPortStr(), std::string("56789"));

  // Compare address (match)
  TEST_TRUE( MySockAddr1 == MySockAddr2);

  // Set socket address using string notation
  MySockAddr1.SetAddr("1.2.3.4:56788");
  TEST_EQ(MySockAddr1.GetIpAddrStr(), std::string("1.2.3.4"));
  TEST_EQ(MySockAddr1.GetPortStr(), std::string("56788"));

  // Compare address (no match)
  TEST_TRUE( MySockAddr1 != MySockAddr2);

  // Compare address (less than)
  TEST_TRUE( MySockAddr1 < MySockAddr2);

  // Set socket address using string notation
  MySockAddr2.SetAddr("1.2.3.3:56789");
  TEST_EQ(MySockAddr2.GetIpAddrStr(), std::string("1.2.3.3"));
  TEST_EQ(MySockAddr2.GetPortStr(), std::string("56789"));

  // Compare address (greater than)
  TEST_TRUE( MySockAddr1 > MySockAddr2);

  // Return success
  return (0);

}

