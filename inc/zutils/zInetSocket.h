//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zInetSocket.h
//    Description:
//
//*****************************************************************************

#ifndef __ZINETSOCKET_H__
#define __ZINETSOCKET_H__

#include <stdint.h>

#include <string>

#include <zutils/zThread.h>
#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// InetAddress Class
//**********************************************************************

class InetAddress: public Address
{

public:

    InetAddress();
    InetAddress(const std::string &ifname_);

    virtual
    ~InetAddress();

    bool
    operator ==(const InetAddress &other_) const;
    bool
    operator !=(const InetAddress &other_) const;
    bool
    operator <(const InetAddress &other_) const;
    bool
    operator >(const InetAddress &other_) const;

    virtual std::string
    GetAddress() const;

    virtual bool
    SetAddress(const std::string &addr_);

    std::string
    GetIpAddr() const;
    std::string
    GetPort() const;
    std::string
    GetNetmask() const;
    std::string
    GetBroadcast() const;

    bool
    SetIpAddr(const std::string &ipaddr_);
    bool
    SetPort(const std::string &port_);
    bool
    SetNetmask(const std::string &netmask_);
    bool
    SetBroadcast(const std::string &bcast_);

protected:

private:

    std::string _ipaddr;
    std::string _port;
    std::string _netmask;
    std::string _bcast;

};

//**********************************************************************
// InetSocket Class
//**********************************************************************

class InetSocket: public Socket, private zThread::Function
{

public:

    InetSocket(const InetAddress &addr_);

    virtual
    ~InetSocket();

    virtual const zSocket::Address *
    GetAddress();

protected:

    virtual void *
    ThreadFunction(void *arg_);

    virtual bool
    _open();

    virtual void
    _close();

    virtual bool
    _bind();

    virtual bool
    _connect();

    virtual ssize_t
    _send(const zSocket::Address *addr_, zSocket::Buffer *sb_);

    virtual ssize_t
    _broadcast(zSocket::Buffer *sb_);

private:

    virtual ssize_t
    _recv(zSocket::Address *addr_, zSocket::Buffer *sb_);

    zThread::Thread _thread;

    int _sock;
    const InetAddress _inetaddr;

};

}
}

#endif /* __ZINETSOCKET_H__ */
