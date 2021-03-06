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

#include <zutils/zUdpSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

Address *
SocketFactory::Create(const Address::TYPE &type_)
{
  Address *addr = NULL;
  switch (type_)
  {
  case Address::TYPE_INET:
    addr = new InetAddress;
    break;
  case Address::TYPE_ETH:
    addr = new EthAddress;
    break;
  case Address::TYPE_LOOP:
    default:
    break;
  }
  return (addr);
}

zSocket::Address *
SocketFactory::Create(const zSocket::Address::TYPE &type_, const std::string &ifname_)
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
SocketFactory::Create(const zSocket::Socket::TYPE &type_, const zSocket::Address *addr_)
{
  return (NULL);
}

}
}

