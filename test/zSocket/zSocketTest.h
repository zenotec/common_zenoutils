/*
 * zSocketTest.h
 *
 *  Created on: Jan 10, 2015
 *      Author: kmahoney
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
  EventHandler(const zEvent::EventNotification* notification_)
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
