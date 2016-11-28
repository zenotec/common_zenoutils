#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>

#include <sstream>

#include <string>
#include <list>
#include <mutex>
#include <memory>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>
#include <zutils/zSocket.h>
#include <zutils/zLoopSocket.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_LoopSocketDefault(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_LoopSocketDefault()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket address and validate
  zSocket::LoopAddress MyAddr(std::string("127.0.0.1:0"));
  TEST_EQ(std::string("127.0.0.1:0"), MyAddr.Address());

  // Set socket address
  TEST_TRUE(MyAddr.Address(std::string("127.0.0.1:9876")));
  TEST_EQ(std::string("127.0.0.1:9876"), MyAddr.Address());

  // Create new socket and validate
  zSocket::LoopSocket *MySock = new zSocket::LoopSocket;
  TEST_ISNOT_NULL(MySock);

  // Cleanup
  MySock->Close();
  delete (MySock);

  // Return success
  return (0);

}

int
zSocketTest_LoopSocketSendReceive(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_LoopSocketSendReceiveLoop()");
  ZLOG_DEBUG("#############################################################");

  bool status = false;

  // Create new socket address and validate
  zSocket::LoopAddress *SrcAddr = new zSocket::LoopAddress;
  TEST_EQ(SocketType::TYPE_LOOP, SrcAddr->Type());
  TEST_EQ(std::string(""), SrcAddr->Address());
  TEST_TRUE(SrcAddr->Address("lo"));
  TEST_EQ(std::string("lo"), SrcAddr->Address());

  // Create new socket address and validate
  zSocket::LoopAddress *DstAddr = new zSocket::LoopAddress;
  TEST_EQ(SocketType::TYPE_LOOP, DstAddr->Type());
  TEST_EQ(std::string(""), DstAddr->Address());
  TEST_TRUE(DstAddr->Address("lo"));
  TEST_EQ(std::string("lo"), DstAddr->Address());

  // Create new socket and validate
  zSocket::LoopSocket *MySock = new zSocket::LoopSocket;
  TEST_ISNOT_NULL(MySock);
  TEST_TRUE(MySock->Address(SrcAddr));
  TEST_TRUE(MySock->Open());
  TEST_TRUE(MySock->Bind());

  // Create new socket handler and validate
  zEvent::EventHandler* MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_NULL(MyHandler);

  // Add socket to handler
  MyHandler->RegisterEvent(MySock);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);

  // Register observer with socket handler
  MyHandler->RegisterObserver(MyObserver);

  // Send string and validate
  std::string ExpStr = "Hello Universe";
  TEST_EQ((int)MySock->Send(*DstAddr, ExpStr), (int)ExpStr.size());

  // Wait for packet to be sent
  status = MyObserver->TxSem.TimedWait(100000);
  TEST_TRUE(status);
  zSocket::SocketAddressBufferPair txp = MyObserver->TxSem.Front();
  MyObserver->TxSem.Pop();

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Wait for packet to be received
  status = MyObserver->RxSem.TimedWait(100000);
  TEST_TRUE(status);
  zSocket::SocketAddressBufferPair rxp = MyObserver->RxSem.Front();
  MyObserver->RxSem.Pop();

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Validate messages match
  TEST_EQ(txp.first->Address(), rxp.first->Address());
  TEST_EQ(txp.second->Str(), rxp.second->Str());

  // Unregister observer with socket handler
  MyHandler->UnregisterEvent(MySock);
  MyHandler->UnregisterObserver(MyObserver);

  // Cleanup
  delete (MyHandler);
  delete (MyObserver);
  delete (MySock);
  delete (DstAddr);
  delete (SrcAddr);

  // Return success
  return (0);

}
