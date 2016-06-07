/*
 * zSocketTest.h
 *
 *  Created on: Jan 10, 2015
 *      Author: kmahoney
 */

#ifndef _ZSOCKETTEST_H_
#define _ZSOCKETTEST_H_

#include <unistd.h>
#include <stdint.h>
#include <netinet/in.h>
#include <string.h>

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
zSocketTest_InetAddressGetSet(void* arg_);
int
zSocketTest_InetAddressCompare(void* arg_);
int
zSocketTest_InetAddressIface(void* arg_);

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

  zSem::Semaphore RxSem;
  zSem::Semaphore TxSem;
  zSem::Semaphore ErrSem;

protected:

  virtual bool
  EventHandler(const zEvent::EventNotification* notification_)
  {
    ZLOG_DEBUG("Handling socket event");

    bool status = false;
    zSocket::SocketNotification *n = (zSocket::SocketNotification *) notification_;
    if (notification_ && (notification_->Type() == zEvent::Event::TYPE_SOCKET))
    {
      switch (n->Id())
      {
      case zSocket::SocketNotification::ID_PKT_RCVD:
        this->RxSem.Post();
        status = true;
        break;
      case zSocket::SocketNotification::ID_PKT_SENT:
        this->TxSem.Post();
        status = true;
        break;
      default:
        this->ErrSem.Post();
        status = false;
        break;
      }
    }
    return (status);
  }

private:

};

class TestSocket : public zSocket::Socket
{
public:
  TestSocket(const zSocket::SocketAddress &addr_) :
      _opened(false), _bound(false), _connected(false)
  {
    this->SetAddress(addr_);
  }

  virtual
  ~TestSocket()
  {
  }

  virtual bool
  Open()
  {
    bool status = false;
    if (!this->_opened)
    {
      this->_opened = true;
      status = true;
    }
    return (status);
  }

  virtual void
  Close()
  {
    this->_opened = false;
    return;
  }

  virtual bool
  Bind()
  {
    bool status = false;
    if (this->_opened && !this->_bound && !this->_connected)
    {
      this->_bound = true;
      status = true;
    }
    return (status);
  }

  virtual bool
  Connect()
  {
    bool status = false;
    if (this->_opened && !this->_bound && !this->_connected)
    {
      this->_connected = true;
      status = true;
    }
    return (status);
  }

protected:


private:

  bool _opened;
  bool _bound;
  bool _connected;

};

#endif /* _ZSOCKETTEST_H_ */
