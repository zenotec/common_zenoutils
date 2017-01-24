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

#ifndef _ZSOCKETTEST_H_
#define _ZSOCKETTEST_H_

#include "UnitTest.h"

int
zSocketTest_BufferDefaults(void* arg_);
int
zSocketTest_AddressDefaults(void* arg_);
int
zSocketTest_ObserverDefaults(void* arg_);
int
zSocketTest_SocketDefaults(void* arg_);

int
zSocketTest_BufferCompare(void* arg_);
int
zSocketTest_BufferString(void* arg_);

int
zSocketTest_AddressGetSet(void* arg_);
int
zSocketTest_AddressCompare(void* arg_);

int
zSocketTest_LoopAddressGetSet(void* arg_);
int
zSocketTest_LoopAddressCompare(void* arg_);

int
zSocketTest_LoopSocketDefault(void* arg_);
int
zSocketTest_LoopSocketSendReceive(void* arg_);

int
zSocketTest_UnixAddressGetSet(void* arg_);
int
zSocketTest_UnixAddressCompare(void* arg_);

int
zSocketTest_UnixSocketDefault(void* arg_);
int
zSocketTest_UnixSocketSendReceive(void* arg_);

int
zSocketTest_InetAddressGetSet(void* arg_);
int
zSocketTest_InetAddressCompare(void* arg_);

int
zSocketTest_InetSocketDefault(void* arg_);
int
zSocketTest_InetSocketSendReceiveLoop(void* arg_);
int
zSocketTest_InetSocketSendReceiveSock2Sock(void* arg_);

int
zSocketTest_InetSocketObserver(void* arg_);

using namespace Test;
using namespace zUtils;
using namespace zSocket;

class TestAddress : public zSocket::SocketAddress
{
public:

  TestAddress() :
    zSocket::SocketAddress(SocketType::TYPE_TEST)
  {
  }

  virtual
  ~TestAddress()
  {
  }

protected:

  virtual bool
  verify(const SocketType type_, const std::string &addr_)
  {
    return (type_ == SocketType::TYPE_TEST);
  }

};

class TestObserver : public zEvent::EventObserver
{
public:
  TestObserver()
  {
  }

  virtual
  ~TestObserver()
  {
  }

  zQueue<SocketAddressBufferPair> RxSem;
  zQueue<SocketAddressBufferPair> TxSem;
  zQueue<SocketAddressBufferPair> ErrSem;

protected:

  virtual bool
  EventHandler(zEvent::EventNotification* notification_)
  {
    ZLOG_DEBUG("Handling socket event");

    bool status = false;
    if (notification_ && (notification_->Type() == zEvent::Event::TYPE_SOCKET))
    {
      SocketNotification *n = (SocketNotification *) notification_;
      switch (n->Id())
      {
      case SocketNotification::ID_PKT_RCVD:
        this->RxSem.Push(n->Pkt());
        status = true;
        break;
      case SocketNotification::ID_PKT_SENT:
        this->TxSem.Push(n->Pkt());
        this->TxSem.Post();
        status = true;
        break;
      default:
        this->ErrSem.Push(n->Pkt());
        status = false;
        break;
      }
    }
    return (status);
  }

private:

};

class TestSocket : public LoopSocket
{
public:
  TestSocket()
  {
  }

  virtual
  ~TestSocket()
  {
  }

protected:

private:

};

#endif /* _ZSOCKETTEST_H_ */
