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

int
zSocketTest_HandlerRegister(void* arg_);
int
zSocketTest_HandlerStartStop(void* arg_);
int
zSocketTest_HandlerSendRecv(void* arg_);
int
zSocketTest_HandlerBroadcast(void* arg_);

using namespace Test;
using namespace zUtils;

class TestObserver: public zSocket::Observer
{
public:

    TestObserver()
    {
        this->_events.Register(&this->_sq);
    }

    virtual ~TestObserver()
    {
    }

    virtual bool SocketRecv(zSocket::Socket *sock_, const zSocket::Address *addr_,
            zSocket::Buffer *sb_)
    {
        ZLOG_DEBUG("TestObserver::SocketRecv(): Receiving on socket");
        zSocket::Buffer *sb = new zSocket::Buffer(*sb_);
        this->_sq.Push(std::make_pair(*addr_, sb));
        return (true);
    }

    zSocket::Buffer *
    WaitForPacket(int ms_)
    {
        std::pair<zSocket::Address, zSocket::Buffer *> q;
        zSocket::Buffer *sb = 0;
        std::string logstr = "TestObserver::WaitForPacket(): Waiting for queue event";
        ZLOG_DEBUG(logstr);
        if (this->_events.Wait(ms_ * 1000))
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

class TestSocket: public zSocket::Socket
{
public:
    TestSocket(const zSocket::Address *addr_) :
            _addr(*addr_), _opened(false), _bound(false), _connected(false)
    {
    }

    virtual ~TestSocket()
    {
    }

    virtual const zSocket::Address *
    GetAddress()
    {
        return (&this->_addr);
    }

protected:

    virtual bool _open()
    {
        bool status = false;
        if (!this->_opened)
        {
            this->_opened = true;
            status = true;
        }
        return (status);
    }

    virtual void _close()
    {
        this->_opened = false;
        return;
    }

    virtual bool _bind()
    {
        bool status = false;
        if (this->_opened && !this->_bound && !this->_connected)
        {
            this->_bound = true;
            status = true;
        }
        return (status);
    }

    virtual bool _connect()
    {
        bool status = false;
        if (this->_opened && !this->_bound && !this->_connected)
        {
            this->_connected = true;
            status = true;
        }
        return (status);
    }

    virtual ssize_t _send(const zSocket::Address *addr_, zSocket::Buffer *sb_)
    {
        zSocket::Address *addr = new zSocket::Address(this->_addr);
        zSocket::Buffer *sb = new zSocket::Buffer(*sb_);
        this->Push(std::make_pair(addr, sb));
        return (sb->Size());
    }

    virtual ssize_t _broadcast(zSocket::Buffer *sb_)
    {
        zSocket::Address *addr = new zSocket::Address(this->_addr);
        zSocket::Buffer *sb = new zSocket::Buffer(*sb_);
        this->Push(std::make_pair(addr, sb));
        return (sb->Size());
    }

private:

    const zSocket::Address _addr;
    bool _opened;
    bool _bound;
    bool _connected;

};

#endif /* _ZSOCKETTEST_H_ */
