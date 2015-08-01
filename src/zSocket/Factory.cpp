/*
 * Factory.cpp
 *
 *  Created on: Aug 1, 2015
 *      Author: kmahoney
 */

#include <zutils/zLog.h>
#include <zutils/zSocket.h>

#include <zutils/zInetSocket.h>

namespace zUtils
{
namespace zSocket
{

zSocket::Address *
Factory::Create(const zSocket::Address::TYPE &type_)
{
    zSocket::Address *addr = NULL;
    switch (type_)
    {
        case Address::TYPE_INET:
            addr = new zSocket::InetAddress;
            break;
        case Address::TYPE_LOOP:
        default:
            break;
    }
    return (addr);
}

zSocket::Address *
Factory::Create(const zSocket::Address::TYPE &type_, const std::string &ifname_)
{
    zSocket::Address *addr = NULL;
    switch (type_)
    {
        case Address::TYPE_INET:
            addr = new zSocket::InetAddress(ifname_);
            break;
        case Address::TYPE_LOOP:
        default:
            break;
    }
    return (addr);
}

zSocket::Socket *
Factory::Create(const zSocket::Socket::TYPE &type_, const zSocket::Address *addr_)
{

}

}
}

