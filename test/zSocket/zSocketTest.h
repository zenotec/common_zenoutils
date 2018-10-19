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

#include <zutils/zLoopSocket.h>

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
zSocketTest_BufferString(void* arg_);
int
zSocketTest_BufferReadWrite(void* arg_);
int
zSocketTest_BufferCompare(void* arg_);
int
zSocketTest_BufferCopy(void* arg_);

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
zSocketTest_MacAddressGetSet(void* arg_);
int
zSocketTest_MacAddressCompare(void* arg_);

int
zSocketTest_RawSocketDefault(void* arg_);
int
zSocketTest_RawSocketSendReceiveLoop(void* arg_);
int
zSocketTest_RawSocketSendReceiveSock2Sock(void* arg_);

int
zSocketTest_Ipv4AddressGetSet(void* arg_);
int
zSocketTest_Ipv4AddressCompare(void* arg_);

int
zSocketTest_UdpSocketDefault(void* arg_);
int
zSocketTest_UdpSocketSendReceiveLoop(void* arg_);
int
zSocketTest_UdpSocketSendReceiveSock2Sock(void* arg_);

int
zSocketTest_UdpSocketObserver(void* arg_);

using namespace Test;
using namespace zUtils;
using namespace zSocket;

class TestAddress : public zSocket::Address
{

public:

  TestAddress() :
    zSocket::Address(Address::TYPE_NONE)
  {
  }

  virtual
  ~TestAddress()
  {
  }

protected:

};

class TestObserver : public zEvent::Observer
{

public:

  TestObserver()
  {
  }

  virtual
  ~TestObserver()
  {
  }

  zQueue::Queue<SHARED_PTR(zSocket::Notification)> RxSem;
  zQueue::Queue<SHARED_PTR(zSocket::Notification)> TxSem;
  zQueue::Queue<SHARED_PTR(zSocket::Notification)> ErrSem;

protected:

  virtual bool
  ObserveEvent(SHARED_PTR(zEvent::Notification) noti_)
  {
    ZLOG_DEBUG("Handling socket event");

    bool status = false;
    if (noti_ && (noti_->GetType() == zEvent::Event::TYPE_SOCKET))
    {
      SHARED_PTR(Notification) n = STATIC_CAST(Notification)(noti_);
      switch (n->GetSubType())
      {
      case Notification::SUBTYPE_PKT_RCVD:
        this->RxSem.Push(n);
        status = true;
        break;
      case Notification::SUBTYPE_PKT_SENT:
        this->TxSem.Push(n);
        this->TxSem.Post();
        status = true;
        break;
      default:
        this->ErrSem.Push(n);
        status = false;
        break;
      }
    }
    return (status);
  }

private:

};

class TestSocket :
    public LoopSocket
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
