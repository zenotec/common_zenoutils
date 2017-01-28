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

  // Wait for message to be received
  TEST_TRUE(MyObserver->RxSem.TimedWait(100));

  // Wait for ACK to be sent
  TEST_TRUE(MyObserver->TxSem.TimedWait(100));

  // Wait for ACK to be received
  TEST_TRUE(MyObserver->RxSem.TimedWait(100));

  // No more messages should have been sent
  TEST_FALSE(MyObserver->TxSem.TryWait());

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

  // Wait for message to be received
  TEST_TRUE(MyObserver->RxSem.TimedWait(100));

  // Wait for ACK to be sent
  TEST_TRUE(MyObserver->TxSem.TimedWait(100));

  // Wait for ACK to be received
  TEST_TRUE(MyObserver->RxSem.TimedWait(100));

  // No more messages should have been sent
  TEST_FALSE(MyObserver->TxSem.TryWait());

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
  UnixAddress ServerAddr("/tmp/server");
  UnixSocket *ServerSock = new UnixSocket;
  TEST_ISNOT_NULL(ServerSock);
  TEST_TRUE(ServerSock->Open());
  TEST_TRUE(ServerSock->Bind(ServerAddr));

  // Create new message socket and validate
  zMessage::MessageSocket *MsgServer = new zMessage::MessageSocket;
  TEST_ISNOT_NULL(MsgServer);

  // Create new socket hander and validate
  zEvent::EventHandler* ServerMsgHandler = new zEvent::EventHandler;
  TEST_ISNOT_NULL(ServerMsgHandler);

  // Register socket with handler and validate
  ServerMsgHandler->RegisterEvent(MsgServer);

  // Create new message socket observer and validate
  TestSocketObserver *ServerMsgObs = new TestSocketObserver;
  TEST_ISNOT_NULL(ServerMsgObs);

  // Register observer with handler and validate
  ServerMsgHandler->RegisterObserver(ServerMsgObs);
  TEST_FALSE(ServerMsgObs->RxSem.TryWait());
  TEST_FALSE(ServerMsgObs->TxSem.TryWait());
  TEST_FALSE(ServerMsgObs->ErrSem.TryWait());

  // Listen on socket
  TEST_TRUE(MsgServer->Listen(ServerSock));

  // Setup network socket
  UnixAddress ClientAddr("/tmp/client");
  UnixSocket *ClientSock = new UnixSocket;
  TEST_ISNOT_NULL(ClientSock);
  TEST_TRUE(ClientSock->Open());
  TEST_TRUE(ClientSock->Bind(ClientAddr));

  // Create new message socket and validate
  zMessage::MessageSocket *MsgClient = new zMessage::MessageSocket;
  TEST_ISNOT_NULL(MsgClient);

  // Connect socket2 to socket1
  TEST_TRUE(MsgClient->Connect(ServerAddr, ClientSock));

  // Wait for hello message to be received
  TEST_TRUE(ServerMsgObs->RxSem.TimedWait(100));

  // Wait for ACK to be sent
  TEST_TRUE(ServerMsgObs->TxSem.TimedWait(100));

  // Send hello message to self
  zMessage::Message *DataMsg = zMessage::MessageFactory::Create(zMessage::Message::TYPE_DATA);
  TEST_ISNOT_NULL(DataMsg);
  TEST_TRUE(DataMsg->SetDst(ServerAddr.Address()));
  TEST_TRUE(MsgClient->RegisterForAck(DataMsg->GetId()));
  TEST_TRUE(MsgClient->Send(*DataMsg));

  // Wait for message to be received
  TEST_TRUE(ServerMsgObs->RxSem.TimedWait(100));

  // No more messages should be waiting
  TEST_FALSE(ServerMsgObs->RxSem.TryWait());

  // Verify no errors
  TEST_FALSE(ServerMsgObs->ErrSem.TryWait());

  // Acknowledge hello message
  zMessage::Message *ackMsg = zMessage::MessageFactory::Create(zMessage::Message::TYPE_ACK);
  TEST_ISNOT_NULL(ackMsg);
  TEST_TRUE(ackMsg->SetId(DataMsg->GetId()));
  TEST_TRUE(ackMsg->SetDst(ClientAddr.Address()));
  TEST_TRUE(ackMsg->SetSrc(ServerAddr.Address()));
  TEST_TRUE(MsgServer->Send(*ackMsg));
  delete (ackMsg);

  // Wait for message to be sent
  TEST_TRUE(ServerMsgObs->TxSem.TimedWait(100));

  // No more messages should have been sent
  TEST_FALSE(ServerMsgObs->TxSem.TryWait());

  // Verify no errors
  TEST_FALSE(ServerMsgObs->ErrSem.TryWait());

  // Wait for ACK
  zMessage::AckMessage ack;
  TEST_TRUE(MsgClient->WaitForAck(DataMsg->GetId(), ack, 100));
  TEST_TRUE(MsgClient->UnregisterForAck(DataMsg->GetId()));
  delete (DataMsg);

  // Disconnect socket2 from socket1
  TEST_TRUE(MsgClient->Disconnect(ServerAddr));

  // Wait for bye message to be received
  TEST_TRUE(ServerMsgObs->RxSem.TimedWait(100));

  // Wait for ACK to be sent
  TEST_TRUE(ServerMsgObs->TxSem.TimedWait(100));

  // Clean up
  delete (MsgClient);
  ClientSock->Close();
  delete (ClientSock);

  ServerMsgHandler->UnregisterEvent(MsgServer);
  delete (MsgServer);
  ServerSock->Close();
  delete (ServerSock);
  ServerMsgHandler->UnregisterObserver(ServerMsgObs);
  delete (ServerMsgObs);
  delete (ServerMsgHandler);

  // Return success
  return (0);

}

