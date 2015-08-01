//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zSocket.h
//    Description:
//
//*****************************************************************************

#ifndef __ZSOCKET_H__
#define __ZSOCKET_H__

#include <stdint.h>
#include <netinet/in.h>

#include <string>
#include <list>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zQueue.h>

namespace zUtils
{
namespace zSocket
{

class Handler;
class Factory;

//**********************************************************************
// zSocket::Buffer Class
//**********************************************************************

class Buffer
{

public:

    Buffer(Buffer &other_);

    Buffer(const size_t size_ = 1500);

    virtual
    ~Buffer();

    Buffer &
    operator=(Buffer &other_);

    bool
    operator==(Buffer &other_);

    bool
    operator!=(Buffer &other_);

    uint8_t *
    Head();

    bool
    Put(off_t off_);

    bool
    Push(off_t off_);

    bool
    Pull(off_t off_);

    uint8_t *
    Data(off_t off_ = 0);

    uint8_t *
    Tail();

    uint8_t *
    End();

    size_t
    Length();

    size_t
    Size();

    size_t
    TotalSize();

    std::string
    Str();

    bool
    Str(const std::string &str_);

protected:

private:

    Buffer(const Buffer &other_);

    Buffer &
    operator=(const Buffer &other_);

    uint8_t *_head;
    size_t _data;
    size_t _tail;
    size_t _end;

};

//**********************************************************************
// zSocket::Address Class
//**********************************************************************

class Address
{

public:

    enum TYPE
    {
        TYPE_ERR = -1, TYPE_NONE = 0, TYPE_LOOP = 1, TYPE_INET = 2, TYPE_LAST
    };

    Address(zSocket::Address::TYPE type = zSocket::Address::TYPE_NONE, const std::string &addr_ =
            std::string(""));

    virtual
    ~Address();

    bool
    operator ==(const zSocket::Address &other_) const;
    bool
    operator !=(const zSocket::Address &other_) const;
    bool
    operator <(const zSocket::Address &other_) const;
    bool
    operator >(const zSocket::Address &other_) const;

    Address::TYPE
    GetType() const;

    virtual std::string
    GetAddress() const;

    virtual bool
    SetAddress(const std::string &addr_);

protected:

    Address::TYPE _type;

private:

    std::string _addr;

};

//**********************************************************************
// zSocket::Socket Class
//**********************************************************************

class Socket: public zQueue<std::pair<zSocket::Address *, zSocket::Buffer *> >
{
    friend class Handler;

public:

    enum TYPE
    {
        TYPE_ERR = -1, TYPE_NONE = 0, TYPE_ETH = 1, TYPE_IPV4 = 2, TYPE_IPV6 = 3, TYPE_LAST
    };

    virtual const zSocket::Address *
    GetAddress() = 0;

    ssize_t
    Receive(zSocket::Address *from_, zSocket::Buffer *sb_);
    ssize_t
    Receive(zSocket::Address *from_, std::string &str_);

    ssize_t
    Send(const zSocket::Address *to_, zSocket::Buffer *sb_);
    ssize_t
    Send(const zSocket::Address *to_, const std::string &str_);

    ssize_t
    Broadcast(zSocket::Buffer *sb_);
    ssize_t
    Broadcast(const std::string &str_);

protected:

    virtual bool
    _open() = 0;

    virtual void
    _close() = 0;

    virtual bool
    _bind() = 0;

    virtual bool
    _connect() = 0;

    virtual ssize_t
    _send(const zSocket::Address *addr_, zSocket::Buffer *sb_) = 0;

    virtual ssize_t
    _broadcast(zSocket::Buffer *sb_) = 0;

protected:

    zSocket::Socket::TYPE _type;

private:

};

//**********************************************************************
// zSocket::Factory Class
//**********************************************************************

class Factory
{

public:

    static zSocket::Address *
    Create(const zSocket::Address::TYPE &type_);

    static zSocket::Address *
    Create(const zSocket::Address::TYPE &type_, const std::string &ifname_);

    static zSocket::Socket *
    Create(const zSocket::Socket::TYPE &type_, const zSocket::Address *addr_);

protected:

private:

};

//**********************************************************************
// zSocket::Observer Class
//**********************************************************************

class Observer
{
public:
    virtual bool
    SocketRecv(zSocket::Socket *sock_, const zSocket::Address *addr_, zSocket::Buffer *sb_) = 0;
};

//**********************************************************************
// zSocket::Handler Class
//**********************************************************************

class Handler: private zThread::Function
{

public:

    Handler();

    virtual
    ~Handler();

    bool
    Register(zSocket::Observer *obs_);

    void
    Unregister(zSocket::Observer *obs_);

    bool
    Bind(zSocket::Socket *sock_);

    bool
    Connect(zSocket::Socket *sock_);

    ssize_t
    Send(const zSocket::Address *to_, zSocket::Buffer *sb_);

    ssize_t
    Send(const zSocket::Address *to_, const std::string &str_);

    ssize_t
    Broadcast(zSocket::Buffer *sb_);

    ssize_t
    Broadcast(const std::string &str_);

    void
    Close();

    bool
    StartListener(uint32_t msecs_ = 0);

    bool
    StopListener(uint32_t msecs_ = 0);

protected:

    virtual void *
    ThreadFunction(void *arg_);

    zSocket::Socket *_sock;

private:

    void
    _notify(zSocket::Socket *sock_, zSocket::Address *addr_, zSocket::Buffer *buf_);

    std::list<zSocket::Observer *> _obsList;
    zEvent::EventList _waitList;

    zSem::Mutex _lock;
    zThread::Thread _thread;

};

}
}

#endif /* __ZSOCKET_H__ */
