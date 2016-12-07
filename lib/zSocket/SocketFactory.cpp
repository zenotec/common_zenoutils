/*
 * Factory.cpp
 *
 *  Created on: Aug 1, 2015
 *      Author: kmahoney
 */

#include <zutils/zLog.h>
#include <zutils/zSocket.h>

#include <zutils/zInet.h>

namespace zUtils
{
namespace zSocket
{

zSocket::SocketAddress *
SocketFactory::Create(const zSocket::SocketAddress::TYPE &type_)
{
    zSocket::SocketAddress *addr = NULL;
    switch (type_)
    {
        case SocketAddress::TYPE_INET:
            addr = new zSocket::InetAddress;
            break;
        case SocketAddress::TYPE_LOOP:
        default:
            break;
    }
    return (addr);
}

zSocket::SocketAddress *
SocketFactory::Create(const zSocket::SocketAddress::TYPE &type_, const std::string &ifname_)
{
    zSocket::SocketAddress *addr = NULL;
    switch (type_)
    {
        case SocketAddress::TYPE_INET:
            addr = new zSocket::InetAddress(ifname_);
            break;
        case SocketAddress::TYPE_LOOP:
        default:
            break;
    }
    return (addr);
}

zSocket::Socket *
SocketFactory::Create(const zSocket::Socket::TYPE &type_, const zSocket::SocketAddress *addr_)
{
  return(NULL);
}

}
}

