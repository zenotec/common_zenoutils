/*
 * Handler.cpp
 *
 *  Created on: Jan 11, 2015
 *      Author: kmahoney
 */


#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_HandlerRegister(void* arg_)
{

  // Create new buffer and validate
  zSocket::Buffer myBuffer;
  TEST_ISNOT_NULL(myBuffer.Head());
  TEST_EQ(myBuffer.Head(), myBuffer.Data());
  TEST_IS_ZERO(myBuffer.Length());
  TEST_IS_ZERO(myBuffer.Size());
  TEST_EQ(myBuffer.TotalSize(), 1500);

  // Create new socket address and validate
  TestAddress myAddress;
  TEST_EQ(std::string(""), myAddress.GetAddr());

  // Create new observer and validate
  TestObserver myObserver;

  // Create new socket and validate
  TestSocket mySocket;

  // Create new handler and validate
  zSocket::Handler myHandler;

  // Register observer
  myHandler.Register(&myObserver);

  // Return success
  return (0);

}


