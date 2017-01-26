/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <zutils/zAckMessage.h>

#include "zMessageTest.h"

using namespace Test;
using namespace zUtils;
using namespace zSocket;

int
zMessageTest_MessageLoopSocket(void* arg_)
{

  bool status = false;

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zMessageTest_MessageLoopSocket()");
  ZLOG_DEBUG("#############################################################");

  // Setup network socket
  LoopAddress MyAddr;
  LoopSocket *MySock = new LoopSocket;
  TEST_ISNOT_NULL(MySock);
  TEST_TRUE(MySock->Open());
  TEST_TRUE(MySock->Bind(MyAddr));

  // Create new message socket and validate
  zMessage::MessageSocket *MsgSock = new zMessage::MessageSocket;
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

  // Listen on socket
  TEST_TRUE(MsgSock->Listen(MySock));

  // Send hello message to self
  zMessage::Message *helloMsg = zMessage::MessageFactory::Create(zMessage::Message::TYPE_HELLO);
  TEST_ISNOT_NULL(helloMsg);
  TEST_TRUE(helloMsg->SetId("helloMsg"));
  TEST_TRUE(helloMsg->SetDst(MyAddr.Address()));
  TEST_TRUE(helloMsg->SetSrc(MyAddr.Address()));
  ZLOG_DEBUG(helloMsg->GetJson());
  TEST_TRUE(MsgSock->Send(*helloMsg));
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
  TEST_TRUE(MsgSock->Send(*ackMsg));
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
  TEST_TRUE(MsgSock->Send(*byeMsg));
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
  delete (MyHandler);
  delete (MyObserver);
  delete (MsgSock);
  MySock->Close();
  delete (MySock);

  // Return success
  return (0);

}

int
zMessageTest_MessageUnixSocket(void* arg_)
{

  bool status = false;

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zMessageTest_MessageUnixSocket()");
  ZLOG_DEBUG("#############################################################");

  // Setup network socket
  UnixAddress MyAddr1("/tmp/sock1");
  UnixSocket *MySock1 = new UnixSocket;
  TEST_ISNOT_NULL(MySock1);
  TEST_TRUE(MySock1->Open());
  TEST_TRUE(MySock1->Bind(MyAddr1));

  // Create new message socket and validate
  zMessage::MessageSocket *MsgSock1 = new zMessage::MessageSocket;
  TEST_ISNOT_NULL(MsgSock1);

  // Create new socket hander and validate
  zEvent::EventHandler* MyHandler1 = new zEvent::EventHandler;
  TEST_ISNOT_NULL(MyHandler1);

  // Register socket with handler and validate
  MyHandler1->RegisterEvent(MsgSock1);

  // Create new message socket observer and validate
  TestSocketObserver *MyObserver1 = new TestSocketObserver;
  TEST_ISNOT_NULL(MyObserver1);

  // Register observer with handler and validate
  MyHandler1->RegisterObserver(MyObserver1);
  TEST_FALSE(MyObserver1->RxSem.TryWait());
  TEST_FALSE(MyObserver1->TxSem.TryWait());
  TEST_FALSE(MyObserver1->ErrSem.TryWait());

  // Listen on socket
  TEST_TRUE(MsgSock1->Listen(MySock1));

  // Setup network socket
  UnixAddress MyAddr2("/tmp/sock2");
  UnixSocket *MySock2 = new UnixSocket;
  TEST_ISNOT_NULL(MySock2);
  TEST_TRUE(MySock2->Open());
  TEST_TRUE(MySock2->Bind(MyAddr2));

  // Create new message socket and validate
  zMessage::MessageSocket *MsgSock2 = new zMessage::MessageSocket;
  TEST_ISNOT_NULL(MsgSock2);

  // Connect socket2 to socket1
  TEST_TRUE(MsgSock2->Connect(MyAddr1, MySock2));

  // Send hello message to self
  zMessage::Message *DataMsg = zMessage::MessageFactory::Create(zMessage::Message::TYPE_DATA);
  TEST_ISNOT_NULL(DataMsg);
  TEST_TRUE(DataMsg->SetDst(MyAddr1.Address()));
  TEST_TRUE(MsgSock2->RegisterForAck(DataMsg->GetId()));
  TEST_TRUE(MsgSock2->Send(*DataMsg));

  // Wait for message to be received
  TEST_TRUE(MyObserver1->RxSem.TimedWait(100));

  // No more messages should be waiting
  TEST_FALSE(MyObserver1->RxSem.TryWait());

  // Verify no errors
  TEST_FALSE(MyObserver1->ErrSem.TryWait());

  // Acknowledge hello message
  zMessage::Message *ackMsg = zMessage::MessageFactory::Create(zMessage::Message::TYPE_ACK);
  TEST_ISNOT_NULL(ackMsg);
  TEST_TRUE(ackMsg->SetId(DataMsg->GetId()));
  TEST_TRUE(ackMsg->SetDst(MyAddr2.Address()));
  TEST_TRUE(ackMsg->SetSrc(MyAddr1.Address()));
  TEST_TRUE(MsgSock1->Send(*ackMsg));
  delete (ackMsg);

  // Wait for message to be sent
  TEST_TRUE(MyObserver1->TxSem.TimedWait(100));

  // No more messages should have been sent
  TEST_FALSE(MyObserver1->TxSem.TryWait());

  // Verify no errors
  TEST_FALSE(MyObserver1->ErrSem.TryWait());

  // Wait for ACK
  zMessage::AckMessage ack;
  TEST_TRUE(MsgSock2->WaitForAck(DataMsg->GetId(), ack, 100));
  TEST_TRUE(MsgSock2->UnregisterForAck(DataMsg->GetId()));
  delete (DataMsg);

  // Disconnect socket2 from socket1
  TEST_TRUE(MsgSock2->Disconnect(MyAddr1));

  // Clean up
  delete (MsgSock2);
  MySock2->Close();
  delete (MySock2);

  MyHandler1->UnregisterEvent(MsgSock1);
  delete (MsgSock1);
  MySock1->Close();
  delete (MySock1);
  MyHandler1->UnregisterObserver(MyObserver1);
  delete (MyObserver1);
  delete (MyHandler1);

  // Return success
  return (0);

}

