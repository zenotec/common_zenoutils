#include "zSocketTest.h"

#include <zutils/zInet.h>

using namespace Test;
using namespace zUtils;

int
zSocketTest_InetSocketDefault(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_InetSocketDefault()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket address and validate
  zSocket::InetAddress MyAddr("lo");
  TEST_EQ(std::string("127.0.0.1:0"), MyAddr.GetAddress());

  // Set socket address
  TEST_TRUE(MyAddr.SetAddress(std::string("127.0.0.1:9876")));
  TEST_EQ(std::string("127.0.0.1:9876"), MyAddr.GetAddress());

  // Create new socket and validate
  zSocket::InetSocket *MySock = new zSocket::InetSocket;
  TEST_ISNOT_NULL(MySock);

  // Cleanup
  MySock->Close();
  delete (MySock);

  // Return success
  return (0);

}

int
zSocketTest_InetSocketSendReceiveLoop(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_InetSocketSendReceiveLoop()");
  ZLOG_DEBUG("#############################################################");

  bool status = false;

  // Create new socket address and validate
  zSocket::InetAddress *SrcAddr = new zSocket::InetAddress;
  TEST_EQ(std::string("0.0.0.0:0"), SrcAddr->GetAddress());

  // Create new socket address and validate
  zSocket::InetAddress *DstAddr = new zSocket::InetAddress;
  TEST_EQ(std::string("0.0.0.0:0"), DstAddr->GetAddress());

  // Set socket address
  TEST_TRUE(SrcAddr->SetAddress("127.0.0.1:9800"));
  TEST_EQ(std::string("127.0.0.1:9800"), SrcAddr->GetAddress());
  TEST_TRUE(SrcAddr != DstAddr);

  // Create new socket and validate
  zSocket::InetSocket *MySock = new zSocket::InetSocket;
  TEST_ISNOT_NULL(MySock);
  TEST_TRUE(MySock->SetAddress(*SrcAddr));
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
  TEST_EQ((int)MySock->Send(*SrcAddr, ExpStr), (int)ExpStr.size());

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
  TEST_EQ(txp.first.GetAddress(), rxp.first.GetAddress());
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

int
zSocketTest_InetSocketSendReceiveSock2Sock(void* arg_)
{
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_InetSocketSendReceiveSock2Sock()");
  ZLOG_DEBUG("#############################################################");

  bool status = false;

  // Create new socket address and validate
  zSocket::InetAddress *SrcAddr = new zSocket::InetAddress;
  TEST_EQ(std::string("0.0.0.0:0"), SrcAddr->GetAddress());

  // Create new socket address and validate
  zSocket::InetAddress *DstAddr = new zSocket::InetAddress;
  TEST_EQ(std::string("0.0.0.0:0"), DstAddr->GetAddress());

  // Set socket address
  TEST_TRUE(SrcAddr->SetAddress("127.0.0.1:9800"));
  TEST_EQ(std::string("127.0.0.1:9800"), SrcAddr->GetAddress());

  // Set socket address
  TEST_TRUE(DstAddr->SetAddress("127.0.0.1:9900"));
  TEST_EQ(std::string("127.0.0.1:9900"), DstAddr->GetAddress());

  // Verify addresses are different
  TEST_TRUE(SrcAddr != DstAddr);

  // Create new socket and validate
  zSocket::InetSocket *MySock1 = new zSocket::InetSocket;
  TEST_ISNOT_NULL(MySock1);
  TEST_TRUE(MySock1->SetAddress(*SrcAddr));
  TEST_TRUE(MySock1->Open());
  TEST_TRUE(MySock1->Bind());

  // Create new socket handler and validate
  zEvent::EventHandler* MyHandler1 = new zEvent::EventHandler;
  TEST_ISNOT_NULL(MyHandler1);

  // Create new socket handler and validate
  zEvent::EventHandler* MyHandler2 = new zEvent::EventHandler;
  TEST_ISNOT_NULL(MyHandler2);

  // Register socket with handler
  MyHandler1->RegisterEvent(MySock1);

  // Create new observer and validate
  TestObserver *MyObserver1 = new TestObserver;
  TEST_ISNOT_NULL(MyObserver1);

  // Register observer
  MyHandler1->RegisterObserver(MyObserver1);

  // Create new socket and validate
  zSocket::InetSocket *MySock2 = new zSocket::InetSocket;
  TEST_ISNOT_NULL(MySock2);
  TEST_TRUE(MySock2->SetAddress(*DstAddr));
  TEST_TRUE(MySock2->Open());
  TEST_TRUE(MySock2->Bind());

  // Register socket with handler
  MyHandler2->RegisterEvent(MySock2);

  // Create new observer and validate
  TestObserver *MyObserver2 = new TestObserver;
  TEST_ISNOT_NULL(MyObserver2);

  // Register observer
  MyHandler2->RegisterObserver(MyObserver2);

  // Send string and validate
  std::string expStr = "Hello Universe";
  TEST_EQ((int )MySock1->Send(*DstAddr, expStr), (int )expStr.size());

  // Wait for packet to be sent
  status = MyObserver1->TxSem.TimedWait(100000);
  TEST_TRUE(status);
  status = MyObserver2->TxSem.TryWait();
  TEST_FALSE(status);
  zSocket::SocketAddressBufferPair txp = MyObserver1->TxSem.Front();
  MyObserver1->TxSem.Pop();

  // Verify no errors
  status = MyObserver1->ErrSem.TryWait();
  TEST_FALSE(status);

  // Wait for packet to be received
  status = MyObserver2->RxSem.TimedWait(100000);
  TEST_TRUE(status);
  status = MyObserver1->RxSem.TryWait();
  TEST_FALSE(status);
  zSocket::SocketAddressBufferPair rxp = MyObserver2->RxSem.Front();
  MyObserver2->RxSem.Pop();

  // Verify no errors
  status = MyObserver2->ErrSem.TryWait();
  TEST_FALSE(status);

  // Validate messages match
  TEST_EQ(txp.first.GetAddress(), DstAddr->GetAddress());
  TEST_EQ(rxp.first.GetAddress(), SrcAddr->GetAddress());
  TEST_EQ(txp.second->Str(), rxp.second->Str());

  // Cleanup
  MyHandler1->UnregisterEvent(MySock1);
  MyHandler1->UnregisterObserver(MyObserver1);
  delete (MyObserver1);
  MySock1->Close();
  delete (MySock1);
  delete (MyHandler1);

  MyHandler1->UnregisterEvent(MySock2);
  MyHandler1->UnregisterObserver(MyObserver2);
  delete (MyObserver2);
  MySock2->Close();
  delete (MySock2);
  delete (MyHandler2);

  delete (DstAddr);
  delete (SrcAddr);

  // Return success
  return (0);

}

