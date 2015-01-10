#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_SocketBufferDefaults(void* arg_)
{

  // Create new packet and validate
  zSocket::SocketBuffer MySB1;
  TEST_ISNOT_NULL(MySB1.Head());
  TEST_EQ(MySB1.Head(), MySB1.Data());
  TEST_IS_ZERO(MySB1.Length());
  TEST_IS_ZERO(MySB1.Size());
  TEST_EQ(MySB1.TotalSize(), 1500);

  // Create new packet of set size and validate
  zSocket::SocketBuffer MySB2(500);
  TEST_ISNOT_NULL(MySB2.Head());
  TEST_EQ(MySB1.Head(), MySB1.Data());
  TEST_ISNOT_NULL(MySB2.Data());
  TEST_IS_ZERO(MySB2.Length());
  TEST_IS_ZERO(MySB2.Size());
  TEST_EQ(MySB2.TotalSize(), 500);

  // Return success
  return (0);

  return (0);
}

int
zSocketTest_SocketAddrDefaults(void* arg_)
{
  // Create new socket address and validate
  zSocket::SocketAddr MySockAddr1;
  TEST_EQ(std::string("0.0.0.0"), MySockAddr1.GetIpAddrStr());
  TEST_EQ(std::string("00:00:00:00:00:00"), MySockAddr1.GetHwAddrStr());
  TEST_EQ(0, MySockAddr1.GetPort());
  TEST_EQ(std::string("0"), MySockAddr1.GetPortStr());

  // Create Socket address using string notation
  zSocket::SocketAddr MySockAddr2("1.2.3.4:56789");
  TEST_EQ(std::string("1.2.3.4"), MySockAddr2.GetIpAddrStr());
  TEST_EQ(std::string("00:00:00:00:00:00"), MySockAddr2.GetHwAddrStr());
  TEST_EQ(56789, MySockAddr2.GetPort());
  TEST_EQ(std::string("56789"), MySockAddr2.GetPortStr());

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(0x09080706);
  addr.sin_port = htons(54321);
  zSocket::SocketAddr MySockAddr3(addr);
  TEST_EQ(std::string("9.8.7.6"), MySockAddr3.GetIpAddrStr());
  TEST_EQ(54321, MySockAddr3.GetPort());
  TEST_EQ(std::string("54321"), MySockAddr3.GetPortStr());

  // Return success
  return (0);
}

int
zSocketTest_SocketDefaults(void* arg_)
{

  // Return success
  return (0);

}

int
zSocketTest_ListenerDefaults(void* arg_)
{
  SocketTestListener listener;

  // Create Socket address using string notation
  zSocket::SocketAddr MySockAddr("127.0.0.2:56789");
  TEST_EQ(MySockAddr.GetIpAddrStr(), std::string("127.0.0.2"));
  TEST_EQ(MySockAddr.GetPortStr(), std::string("56789"));

  // Create socket
  zSocket::Socket MySock(MySockAddr);
  MySock.Listen();
  MySock.Register(&listener);

  // Cleanup
  MySock.Unregister(&listener);

  // Return success
  return (0);

}

