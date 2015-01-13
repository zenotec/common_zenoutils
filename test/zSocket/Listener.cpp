#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_SocketListener(void* arg_)
{
  SocketTestListener listener;

  // Create listener socket address using string notation
  zSocket::InetAddress ListenerAddr("127.0.0.1:56789");
  TEST_EQ(ListenerAddr._getIpAddrStr(), std::string("127.0.0.1"));
  TEST_EQ(ListenerAddr._getPortStr(), std::string("56789"));

  // Create listener socket
  zSocket::Handler ListenerSock(ListenerAddr);
  ListenerSock.Listen();
  ListenerSock.Register(&listener);

  // Create sender socket address using string notation
  zSocket::InetAddress SenderAddr("127.0.0.2:56789");
  TEST_EQ(SenderAddr._getIpAddrStr(), std::string("127.0.0.2"));
  TEST_EQ(SenderAddr._getPortStr(), std::string("56789"));

  // Create sender socket
  zSocket::Handler SenderSock(SenderAddr);

  // Create packet for sending
  zSocket::Buffer sb_exp(88), *sb_obs;
  memset(sb_exp.Data(), 0xed, 88);
  sb_exp.Put(88);

  // Send packet to listener
  SenderSock.Send(ListenerAddr, &sb_exp);

  // Wait for packet to be received and validated
  sb_obs = listener.WaitForPacket(1000);
  TEST_ISNOT_NULL(sb_obs);
  TEST_TRUE(sb_exp == *sb_obs);
  delete(sb_obs);

  // Cleanup
  ListenerSock.Unregister(&listener);

  // Return success
  return (0);

}
