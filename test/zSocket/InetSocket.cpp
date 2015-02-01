#include "zSocketTest.h"

#include <zutils/zInetSocket.h>

using namespace Test;
using namespace zUtils;

int
zSocketTest_InetSocketDefault(void* arg_)
{

  // Create new socket address and validate
  zSocket::Address myAddr(zSocket::Address::TYPE_INET);
  TEST_EQ(std::string(""), myAddr.GetAddress());

  // Set socket address
  TEST_TRUE(myAddr.SetAddress(std::string("127.0.0.1:9876")));
  TEST_EQ(std::string("127.0.0.1:9876"), myAddr.GetAddress());

  // Create new socket handler and validate
  zSocket::Handler *myHandler = new zSocket::Handler;
  TEST_ISNOT_NULL(myHandler);

  // Create new socket and validate
  zSocket::InetSocket *mySock = new zSocket::InetSocket(myAddr);
  TEST_ISNOT_NULL(mySock);
  TEST_TRUE(myHandler->Bind(mySock));

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

  unsigned int n = 0;

  // Create new socket address and validate
  zSocket::Address srcAddr(zSocket::Address::TYPE_INET);
  TEST_EQ(std::string(""), srcAddr.GetAddress());

  // Create new socket address and validate
  zSocket::Address dstAddr(zSocket::Address::TYPE_INET);
  TEST_EQ(std::string(""), dstAddr.GetAddress());

  // Set socket address
  TEST_TRUE(srcAddr.SetAddress(std::string("127.0.0.1:9876")));
  TEST_EQ(std::string("127.0.0.1:9876"), srcAddr.GetAddress());
  TEST_TRUE(srcAddr != dstAddr);

  // Create new socket handler and validate
  zSocket::Handler *myHandler = new zSocket::Handler;
  TEST_ISNOT_NULL(myHandler);

  // Create new socket and validate
  zSocket::InetSocket *mySock = new zSocket::InetSocket(srcAddr);
  TEST_ISNOT_NULL(mySock);
  TEST_TRUE(myHandler->Bind(mySock));

  // Send string and validate
  std::string expStr = "Hello Universe";
  TEST_EQ((int )mySock->Send(srcAddr, expStr), (int )expStr.size());

  // Do not need a context switch as the underlying code puts the buffer into the queue
  //   and therefore the buffer is immediately available

  // Receive string back and validate
  std::string obsStr;
  TEST_EQ((int )mySock->Receive(dstAddr, obsStr), (int )expStr.size());
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
  TEST_EQ(std::string(""), srcAddr.GetAddress());

  // Create new socket address and validate
  zSocket::Address dstAddr(zSocket::Address::TYPE_INET);
  TEST_EQ(std::string(""), dstAddr.GetAddress());

  // Set socket address
  TEST_TRUE(srcAddr.SetAddress(std::string("127.0.0.1:9877")));
  TEST_EQ(std::string("127.0.0.1:9877"), srcAddr.GetAddress());

  // Set socket address
  TEST_TRUE(dstAddr.SetAddress(std::string("127.0.0.2:9877")));
  TEST_EQ(std::string("127.0.0.2:9877"), dstAddr.GetAddress());

  // Create new socket handler and validate
  zSocket::Handler *myHandler = new zSocket::Handler;
  TEST_ISNOT_NULL(myHandler);

  // Create new socket and validate
  zSocket::InetSocket *mySock1 = new zSocket::InetSocket(srcAddr);
  TEST_ISNOT_NULL(mySock1);
  TEST_TRUE(myHandler->Bind(mySock1));

  // Create new socket and validate
  zSocket::InetSocket *mySock2 = new zSocket::InetSocket(dstAddr);
  TEST_ISNOT_NULL(mySock2);
  TEST_TRUE(myHandler->Bind(mySock2));

  // Send string and validate
  std::string expStr = "Hello Universe";
  TEST_EQ((int )mySock1->Send(dstAddr, expStr), (int )expStr.size());

  // Sleep a moment to allow context switch
  usleep(100000);

  // Receive string back and validate
  zSocket::Address obsAddr;
  std::string obsStr;
  TEST_EQ((int )mySock2->Receive(obsAddr, obsStr), (int )expStr.size());
  TEST_EQ(obsAddr.GetAddress(), srcAddr.GetAddress());
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
  TEST_EQ(std::string(""), srcAddr.GetAddress());

  // Create new socket address and validate
  zSocket::Address dstAddr(zSocket::Address::TYPE_INET);
  TEST_EQ(std::string(""), dstAddr.GetAddress());

  // Set socket address
  TEST_TRUE(srcAddr.SetAddress(std::string("127.0.0.1:9877")));
  TEST_EQ(std::string("127.0.0.1:9877"), srcAddr.GetAddress());
  TEST_TRUE(srcAddr != dstAddr);

  // Create new socket handler and validate
  zSocket::Handler *myHandler = new zSocket::Handler;
  TEST_ISNOT_NULL(myHandler);

  // Create new observer and validate
  TestObserver myObserver;

  // Register handler
  TEST_TRUE(myHandler->Register(&myObserver));

  // Start listening thread
  TEST_TRUE(myHandler->StartListener(1000));

  // Create new socket and validate
  zSocket::InetSocket *mySock = new zSocket::InetSocket(srcAddr);
  TEST_ISNOT_NULL(mySock);
  TEST_TRUE(myHandler->Bind(mySock));

  // Send string and validate
  std::string expStr = "Hello Universe";
  TEST_EQ((int )mySock->Send(srcAddr, expStr), (int )expStr.size());

  // Sleep a moment to allow context switch
  usleep(100000);

  // Wait for socket observer to be notified
  zSocket::Buffer *sb = myObserver.WaitForPacket(1000);
  TEST_ISNOT_NULL(sb);

  // Cleanup
  delete (sb);
  TEST_TRUE(myHandler->StopListener(1000));
  myHandler->Unregister(&myObserver);
  myHandler->Close(mySock);
  delete (mySock);
  delete (myHandler);

  // Return success
  return (0);

}

