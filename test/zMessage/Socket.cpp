#include "zMessageTest.h"

#include <zutils/zInet.h>

using namespace Test;
using namespace zUtils;

int
zMessageTest_MessageSocket(void* arg_)
{

  bool status = false;

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zMessageTest_MessageSocket()");
  ZLOG_DEBUG("#############################################################");

  // Setup network socket
  zSocket::InetAddress MyAddr("lo");
  TEST_TRUE(MyAddr.SetPort("8888"));
  zSocket::InetSocket *MySock = new zSocket::InetSocket;
  TEST_ISNOT_NULL(MySock);
  TEST_TRUE(MySock->SetAddress(MyAddr));

  // Create new message socket and validate
  zMessage::MessageSocket *MyMsgSock = new zMessage::MessageSocket(MySock);
  TEST_ISNOT_NULL(MyMsgSock);

  // Create new message socket observer and validate
  TestSocketObserver *MyObserver = new TestSocketObserver;
  TEST_ISNOT_NULL(MyObserver);

  // Register observer and validate
  MyMsgSock->RegisterObserver(MyObserver);
  TEST_FALSE(MyObserver->RxSem.TryWait());
  TEST_FALSE(MyObserver->TxSem.TryWait());
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Open and bind socket
  status = MySock->Open();
  TEST_TRUE(status);
  status = MySock->Bind();
  TEST_TRUE(status);

  // Send hello message to self
  zMessage::Message *helloMsg = zMessage::Factory::Create(zMessage::Message::TYPE_HELLO);
  TEST_ISNOT_NULL(helloMsg);
  TEST_TRUE(helloMsg->SetId("helloMsg"));
  TEST_TRUE(helloMsg->SetDst(MyAddr.GetAddress()));
  TEST_TRUE(helloMsg->SetSrc(MyAddr.GetAddress()));
  ZLOG_DEBUG(helloMsg->GetJson());
  TEST_TRUE(MyMsgSock->Send(*helloMsg));
  delete (helloMsg);

  // Wait for message to be sent
  TEST_TRUE(MyObserver->TxSem.TimedWait(10000));

  // No more messages should have been sent
  TEST_FALSE(MyObserver->TxSem.TryWait());

  // Verify no errors
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Wait for message to be received
  TEST_TRUE(MyObserver->RxSem.TimedWait(10000));

  // No more messages should be waiting
  TEST_FALSE(MyObserver->RxSem.TryWait());

  // Verify no errors
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Acknowledge hello message
  zMessage::Message *ackMsg = zMessage::Factory::Create(zMessage::Message::TYPE_ACK);
  TEST_ISNOT_NULL(ackMsg);
  TEST_TRUE(ackMsg->SetId("ackMsg"));
  TEST_TRUE(ackMsg->SetDst(MyAddr.GetAddress()));
  TEST_TRUE(ackMsg->SetSrc(MyAddr.GetAddress()));
  ZLOG_DEBUG(ackMsg->GetJson());
  TEST_TRUE(MyMsgSock->Send(*ackMsg));
  delete (ackMsg);

  // Wait for message to be sent
  TEST_TRUE(MyObserver->TxSem.TimedWait(10000));

  // No more messages should have been sent
  TEST_FALSE(MyObserver->TxSem.TryWait());

  // Verify no errors
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Wait for message to be received
  TEST_TRUE(MyObserver->RxSem.TimedWait(10000));

  // No more messages should be waiting
  TEST_FALSE(MyObserver->RxSem.TryWait());

  // Verify no errors
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Send bye message to self
  zMessage::Message *byeMsg = zMessage::Factory::Create(zMessage::Message::TYPE_BYE);
  TEST_ISNOT_NULL(byeMsg);
  TEST_TRUE(byeMsg->SetId("byeMsg"));
  TEST_TRUE(byeMsg->SetDst(MyAddr.GetAddress()));
  TEST_TRUE(byeMsg->SetSrc(MyAddr.GetAddress()));
  ZLOG_DEBUG(byeMsg->GetJson());
  TEST_TRUE(MyMsgSock->Send(*byeMsg));
  delete (byeMsg);

  // Wait for message to be sent
  TEST_TRUE(MyObserver->TxSem.TimedWait(10000));

  // No more messages should have been sent
  TEST_FALSE(MyObserver->TxSem.TryWait());

  // Verify no errors
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Wait for message to be received
  TEST_TRUE(MyObserver->RxSem.TimedWait(10000));

  // No more messages should be waiting
  TEST_FALSE(MyObserver->RxSem.TryWait());

  // Verify no errors
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Clean up
  MyMsgSock->UnregisterObserver(MyObserver);
  delete(MyObserver);
  delete (MyMsgSock);
  MySock->Close();
  delete (MySock);

  // Return success
  return (0);

}

