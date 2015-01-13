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
  TEST_EQ(expSockAddr.sin_addr.s_addr, obsSockAddr.sin_addr.s_addr);
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

int
zSocketTest_InetSocketDefault(void* arg_)
{

  // Create new socket address and validate
  zSocket::Address myAddr(zSocket::Address::TYPE_INET);
  TEST_EQ(std::string(""), myAddr.GetAddr());

  // Set socket address
  TEST_TRUE(myAddr.SetAddr(std::string("127.0.0.1:9876")));
  TEST_EQ(std::string("127.0.0.1:9876"), myAddr.GetAddr());

  // Create new socket handler and validate
  zSocket::Handler *myHandler = new zSocket::Handler;
  TEST_ISNOT_NULL(myHandler);

  // Create new socket and validate
  zSocket::InetSocket *mySock = new zSocket::InetSocket(myAddr);
  TEST_ISNOT_NULL(mySock);
  TEST_TRUE(myHandler->Open(mySock));
  TEST_TRUE(mySock->Bind());

  // Cleanup
  myHandler->Close(mySock);
  delete (mySock);
  delete (myHandler);

  // Return success
  return (0);

}

int
zSocketTest_InetSocketSendReceiveLoop(void* arg_)
{

  // Create new socket address and validate
  zSocket::Address srcAddr(zSocket::Address::TYPE_INET);
  TEST_EQ(std::string(""), srcAddr.GetAddr());

  // Create new socket address and validate
  zSocket::Address dstAddr(zSocket::Address::TYPE_INET);
  TEST_EQ(std::string(""), dstAddr.GetAddr());

  // Set socket address
  TEST_TRUE(srcAddr.SetAddr(std::string("127.0.0.1:9876")));
  TEST_EQ(std::string("127.0.0.1:9876"), srcAddr.GetAddr());
  TEST_TRUE(srcAddr != dstAddr);

  // Create new socket handler and validate
  zSocket::Handler *myHandler = new zSocket::Handler;
  TEST_ISNOT_NULL(myHandler);

  // Create new socket and validate
  zSocket::InetSocket *mySock = new zSocket::InetSocket(srcAddr);
  TEST_ISNOT_NULL(mySock);
  TEST_TRUE(myHandler->Open(mySock));
  TEST_TRUE(mySock->Bind());

  // Send string and validate
  std::string expStr = "Hello Universe";
  TEST_EQ(mySock->SendString(srcAddr, expStr), expStr.size());

  // Do not need a context switch as the underlying code puts the buffer into the queue
  //   and therefore the buffer is immediately available

  // Receive string back and validate
  std::string obsStr;
  TEST_EQ(mySock->RecvString(dstAddr, obsStr), expStr.size());
  TEST_TRUE(srcAddr == dstAddr);
  TEST_EQ(expStr, obsStr);

  // Cleanup
  myHandler->Close(mySock);
  delete (mySock);
  delete (myHandler);

  // Return success
  return (0);

}

int
zSocketTest_InetSocketSendReceiveSock2Sock(void* arg_)
{

  // Create new socket address and validate
  zSocket::Address srcAddr(zSocket::Address::TYPE_INET);
  TEST_EQ(std::string(""), srcAddr.GetAddr());

  // Create new socket address and validate
  zSocket::Address dstAddr(zSocket::Address::TYPE_INET);
  TEST_EQ(std::string(""), dstAddr.GetAddr());

  // Set socket address
  TEST_TRUE(srcAddr.SetAddr(std::string("127.0.0.1:9877")));
  TEST_EQ(std::string("127.0.0.1:9877"), srcAddr.GetAddr());

  // Set socket address
  TEST_TRUE(dstAddr.SetAddr(std::string("127.0.0.2:9877")));
  TEST_EQ(std::string("127.0.0.2:9877"), dstAddr.GetAddr());

  // Create new socket handler and validate
  zSocket::Handler *myHandler = new zSocket::Handler;
  TEST_ISNOT_NULL(myHandler);

  // Create new socket and validate
  zSocket::InetSocket *mySock1 = new zSocket::InetSocket(srcAddr);
  TEST_ISNOT_NULL(mySock1);
  TEST_TRUE(myHandler->Open(mySock1));
  TEST_TRUE(mySock1->Bind());

  // Create new socket and validate
  zSocket::InetSocket *mySock2 = new zSocket::InetSocket(dstAddr);
  TEST_ISNOT_NULL(mySock2);
  TEST_TRUE(myHandler->Open(mySock2));
  TEST_TRUE(mySock2->Bind());

  // Send string and validate
  std::string expStr = "Hello Universe";
  TEST_EQ(mySock1->SendString(dstAddr, expStr), expStr.size());

  // Sleep a moment to allow context switch
  usleep(100000);

  // Receive string back and validate
  zSocket::Address obsAddr;
  std::string obsStr;
  TEST_EQ(mySock2->RecvString(obsAddr, obsStr), expStr.size());
  TEST_EQ(obsAddr.GetAddr(), srcAddr.GetAddr());
  TEST_EQ(obsAddr.GetType(), srcAddr.GetType());
  TEST_TRUE(obsAddr == srcAddr);
  TEST_EQ(expStr, obsStr);

  // Cleanup
  myHandler->Close(mySock1);
  myHandler->Close(mySock2);
  delete (mySock1);
  delete (mySock2);
  delete (myHandler);

  // Return success
  return (0);

}

int
zSocketTest_InetSocketObserver(void* arg_)
{

  // Create new socket address and validate
  zSocket::Address srcAddr(zSocket::Address::TYPE_INET);
  TEST_EQ(std::string(""), srcAddr.GetAddr());

  // Create new socket address and validate
  zSocket::Address dstAddr(zSocket::Address::TYPE_INET);
  TEST_EQ(std::string(""), dstAddr.GetAddr());

  // Set socket address
  TEST_TRUE(srcAddr.SetAddr(std::string("127.0.0.1:9877")));
  TEST_EQ(std::string("127.0.0.1:9877"), srcAddr.GetAddr());
  TEST_TRUE(srcAddr != dstAddr);

  // Create new socket handler and validate
  zSocket::Handler *myHandler = new zSocket::Handler;
  TEST_ISNOT_NULL(myHandler);

  // Create new observer and validate
  TestObserver myObserver;

  // Register handler
  TEST_TRUE(myHandler->Register(&myObserver));

  // Start listening thread
  TEST_TRUE(myHandler->StartListener(1000000));

  // Create new socket and validate
  zSocket::InetSocket *mySock = new zSocket::InetSocket(srcAddr);
  TEST_ISNOT_NULL(mySock);
  TEST_TRUE(myHandler->Open(mySock));
  TEST_TRUE(mySock->Bind());

  // Send string and validate
  std::string expStr = "Hello Universe";
  TEST_EQ(mySock->SendString(srcAddr, expStr), expStr.size());

  // Sleep a moment to allow context switch
  usleep(100000);

  // Wait for socket observer to be notified
  zSocket::Buffer *sb = myObserver.WaitForPacket(1000);
  TEST_ISNOT_NULL(sb);

  // Cleanup
  delete (sb);
  TEST_TRUE(myHandler->StopListener());
  myHandler->Unregister(&myObserver);
  myHandler->Close(mySock);
  delete (mySock);
  delete (myHandler);

  // Return success
  return (0);

}

