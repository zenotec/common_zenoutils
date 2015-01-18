/*
 * Handler.cpp
 *
 *  Created on: Jan 11, 2015
 *      Author: kmahoney
 */

#include <unistd.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_HandlerRegister(void* arg_)
{

  // Create new observer and validate
  TestObserver myObserver;

  // Create new socket and validate
  TestSocket mySocket;

  // Create new handler and validate
  zSocket::Handler myHandler;

  // Register observer
  TEST_TRUE(myHandler.Register(&myObserver));

  // Pause to let listener run a moment
  usleep(100000);

  // Clean up
  myHandler.Unregister(&myObserver);

  // Return success
  return (0);

}

int
zSocketTest_HandlerStartStop(void* arg_)
{

  // Create new socket address and validate
  zSocket::Address myAddress(zSocket::Address::TYPE_LOOP, "lo");
  TEST_EQ(std::string("lo"), myAddress.GetAddr());

  // Create new observer and validate
  TestObserver *myObserver = new TestObserver;
  TEST_ISNOT_NULL(myObserver);

  // Create new socket and validate
  TestSocket *mySocket = new TestSocket;
  TEST_ISNOT_NULL(mySocket);

  // Create new handler and validate
  zSocket::Handler *myHandler = new zSocket::Handler;
  TEST_ISNOT_NULL(myHandler);

  // Register observer
  TEST_TRUE(myHandler->Register(myObserver));

  // Bind socket and validate
  TEST_TRUE(myHandler->Bind(mySocket));

  // Start listener
  TEST_TRUE(myHandler->StartListener(1000));

  // Pause to let listener run a moment
  usleep(100000);

  // Stop listener
  TEST_TRUE(myHandler->StopListener(1000));

  // Clean up
  myHandler->Close(mySocket);
  delete (mySocket);
  myHandler->Unregister(myObserver);
  delete (myObserver);
  delete (myHandler);

  // Return success
  return (0);

}

int
zSocketTest_HandlerSendRecv(void* arg_)
{

  // Create new socket address and validate
  zSocket::Address myAddress(zSocket::Address::TYPE_LOOP, "lo");
  TEST_EQ(std::string("lo"), myAddress.GetAddr());

  // Create new observer and validate
  TestObserver *myObserver = new TestObserver;
  TEST_ISNOT_NULL(myObserver);

  // Create new socket and validate
  TestSocket *mySocket = new TestSocket;
  TEST_ISNOT_NULL(mySocket);

  // Create new handler and validate
  zSocket::Handler *myHandler = new zSocket::Handler;
  TEST_ISNOT_NULL(myHandler);

  // Register observer
  TEST_TRUE(myHandler->Register(myObserver));

  // Bind socket and validate
  TEST_TRUE(myHandler->Bind(mySocket));

  // Start listener
  TEST_TRUE(myHandler->StartListener(1000));

  // Send string and validate
  std::string expStr = "zSocketTest_HandlerSendRecv";
  TEST_TRUE(mySocket->SendString(myAddress, expStr));

  // Wait for socket observer to be notified
  zSocket::Buffer *sb = myObserver->WaitForPacket(1000);
  TEST_ISNOT_NULL(sb);
  delete (sb);

  // Stop listener
  TEST_TRUE(myHandler->StopListener(1000));

  // Clean up
  myHandler->Close(mySocket);
  delete (mySocket);
  myHandler->Unregister(myObserver);
  delete (myObserver);
  delete (myHandler);

  // Return success
  return (0);

}

