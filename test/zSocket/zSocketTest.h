/*
 * zSocketTest.h
 *
 *  Created on: Jan 10, 2015
 *      Author: kmahoney
 */

#ifndef _ZSOCKETTEST_H_
#define _ZSOCKETTEST_H_

#include <unistd.h>

#include <zutils/zSocket.h>
#include <zutils/zQueue.h>

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
zSocketTest_HandlerDefaults(void* arg_);

int
zSocketTest_SocketBufferCompare(void* arg_);

int
zSocketTest_AddressGetSet(void* arg_);
int
zSocketTest_AddressCompare(void* arg_);

int
zSocketTest_InetAddressGet(void* arg_);
int
zSocketTest_InetAddressSet(void* arg_);
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

class TestObserver : public zSocket::Observer
{
public:

  TestObserver()
  {
    this->_events.Register(&this->_sq);
  }

  virtual
  ~TestObserver()
  {
  }

  virtual bool
  SocketRecv(zSocket::Socket *sock_, const zSocket::Address &addr_, zSocket::Buffer &sb_)
  {
    ZLOG_DEBUG("TestObserver::SocketRecv(): Receiving on socket");
    zSocket::Buffer *sb = new zSocket::Buffer(sb_);
    this->_sq.Push(std::make_pair(addr_, sb));
    return (true);
  }

  zSocket::Buffer *
  WaitForPacket(int ms_)
  {
    std::pair<zSocket::Address, zSocket::Buffer *> q;
    zSocket::Buffer *sb = 0;
    std::string logstr = "TestObserver::WaitForPacket(): Waiting for queue event";
    ZLOG_DEBUG(logstr);
    if (this->_events.Wait(ms_))
    {
      std::string logstr = "TestObserver::WaitForPacket(): Queue event received";
      ZLOG_DEBUG(logstr);
      if (!this->_sq.Empty())
      {
        q = this->_sq.Front();
        this->_sq.Pop();
        sb = q.second;
      } // end if
    } // end for
    return (sb);
  }

protected:

private:
  zQueue<std::pair<zSocket::Address, zSocket::Buffer *> > _sq;
  zEvent::EventList _events;
};

class TestSocket : public zSocket::Socket
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

  virtual bool
  _open()
  {
    return (false);
  }

  virtual void
  _close()
  {
    return;
  }

  virtual bool
  _bind()
  {
    return (false);
  }

  virtual bool
  _connect()
  {
    return (false);
  }

  virtual ssize_t
  _send(const zSocket::Address &addr_, zSocket::Buffer &sb_)
  {
    return (-1);
  }

private:

};

#endif /* _ZSOCKETTEST_H_ */
