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
  return (NULL);
}

}
}

