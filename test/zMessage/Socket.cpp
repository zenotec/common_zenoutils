#include "zMessageTest.h"

using namespace Test;
using namespace zUtils;
using namespace zSocket;

int
zMessageTest_MessageSocket(void* arg_)
{

  bool status = false;

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zMessageTest_MessageSocket()");
  ZLOG_DEBUG("#############################################################");

  // Setup network socket
  LoopAddress MyAddr("lo");
  LoopSocket *MySock = new LoopSocket;
  TEST_ISNOT_NULL(MySock);
  TEST_TRUE(MySock->Address(&MyAddr));

  // Create new message socket and validate
  zMessage::MessageSocket *MsgSock = new zMessage::MessageSocket(MySock);
  TEST_ISNOT_NULL(MsgSock);

  // Create new socket hander and validate
  zEvent::EventHandler* MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_NULL(MyHandler);

  // Register socket with handler and validate
  MyHandler->RegisterEvent(MsgSock);

  // Create new message socket observer and validate
  TestSocketObserver *MyObserver = new TestSocketObserver;
  TEST_ISNOT_NULL(MyObserver);

  // Register observer with handler and validate
  MyHandler->RegisterObserver(MyObserver);
  TEST_FALSE(MyObserver->RxSem.TryWait());
  TEST_FALSE(MyObserver->TxSem.TryWait());
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Open and bind socket
  TEST_TRUE(MySock->Open());
  TEST_TRUE(MySock->Bind());

  // Send hello message to self
  zMessage::Message *helloMsg = zMessage::MessageFactory::Create(zMessage::Message::TYPE_HELLO);
  TEST_ISNOT_NULL(helloMsg);
  TEST_TRUE(helloMsg->SetId("helloMsg"));
  TEST_TRUE(helloMsg->SetDst(MyAddr.Address()));
  TEST_TRUE(helloMsg->SetSrc(MyAddr.Address()));
  ZLOG_DEBUG(helloMsg->GetJson());
  TEST_TRUE(MsgSock->Send(MyAddr, *helloMsg));
  delete (helloMsg);

  // Wait for message to be sent
  TEST_TRUE(MyObserver->TxSem.TimedWait(100));

  // No more messages should have been sent
  TEST_FALSE(MyObserver->TxSem.TryWait());

  // Verify no errors
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Wait for message to be received
  TEST_TRUE(MyObserver->RxSem.TimedWait(100));

  // No more messages should be waiting
  TEST_FALSE(MyObserver->RxSem.TryWait());

  // Verify no errors
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Acknowledge hello message
  zMessage::Message *ackMsg = zMessage::MessageFactory::Create(zMessage::Message::TYPE_ACK);
  TEST_ISNOT_NULL(ackMsg);
  TEST_TRUE(ackMsg->SetId("ackMsg"));
  TEST_TRUE(ackMsg->SetDst(MyAddr.Address()));
  TEST_TRUE(ackMsg->SetSrc(MyAddr.Address()));
  ZLOG_DEBUG(ackMsg->GetJson());
  TEST_TRUE(MsgSock->Send(MyAddr, *ackMsg));
  delete (ackMsg);

  // Wait for message to be sent
  TEST_TRUE(MyObserver->TxSem.TimedWait(100));

  // No more messages should have been sent
  TEST_FALSE(MyObserver->TxSem.TryWait());

  // Verify no errors
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Wait for message to be received
  TEST_TRUE(MyObserver->RxSem.TimedWait(100));

  // No more messages should be waiting
  TEST_FALSE(MyObserver->RxSem.TryWait());

  // Verify no errors
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Send bye message to self
  zMessage::Message *byeMsg = zMessage::MessageFactory::Create(zMessage::Message::TYPE_BYE);
  TEST_ISNOT_NULL(byeMsg);
  TEST_TRUE(byeMsg->SetId("byeMsg"));
  TEST_TRUE(byeMsg->SetDst(MyAddr.Address()));
  TEST_TRUE(byeMsg->SetSrc(MyAddr.Address()));
  ZLOG_DEBUG(byeMsg->GetJson());
  TEST_TRUE(MsgSock->Send(MyAddr, *byeMsg));
  delete (byeMsg);

  // Wait for message to be sent
  TEST_TRUE(MyObserver->TxSem.TimedWait(100));

  // No more messages should have been sent
  TEST_FALSE(MyObserver->TxSem.TryWait());

  // Verify no errors
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Wait for message to be received
  TEST_TRUE(MyObserver->RxSem.TimedWait(100));

  // No more messages should be waiting
  TEST_FALSE(MyObserver->RxSem.TryWait());

  // Verify no errors
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Clean up
  MyHandler->UnregisterEvent(MsgSock);
  MyHandler->UnregisterObserver(MyObserver);
  delete(MyHandler);
  delete(MyObserver);
  delete (MsgSock);
  MySock->Close();
  delete (MySock);

  // Return success
  return (0);

}

