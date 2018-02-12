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

#include <string>

#include <zutils/zLog.h>
#include <zutils/zSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// Class: zSocket::Address
//**********************************************************************

Address::Address(const SOCKET_TYPE type_, const std::string &addr_) :
    _type(type_), _addr(addr_)
{
}

Address::Address(const Address &other_) :
    _type(other_._type), _addr(other_._addr)
{
}

Address::~Address()
{
}

Address&
Address::operator=(const Address &other_)
{
  this->_type = other_._type;
  this->_addr = other_._addr;
  return (*this);
}

bool
Address::operator ==(const Address &other_) const
{
  return ((this->_type == other_._type) && (this->_addr == other_._addr));
}

bool
Address::operator !=(const Address &other_) const
{
  return ((this->_type != other_._type) || (this->_addr != other_._addr));
}

bool
Address::operator <(const Address &other_) const
{
  return ((this->_type != other_._type) || (this->_addr < other_._addr));
}

bool
Address::operator >(const Address &other_) const
{
  return ((this->_type != other_._type) || (this->_addr > other_._addr));
}

SOCKET_TYPE
Address::GetType() const
{
  return (this->_type);
}

bool
Address::SetType(const SOCKET_TYPE type_)
{
  bool status = false;
  switch (type_)
  {
  case SOCKET_TYPE::TYPE_TEST:
    // no break
  case SOCKET_TYPE::TYPE_LOOP:
    // no break
  case SOCKET_TYPE::TYPE_UNIX:
    // no break
  case SOCKET_TYPE::TYPE_ETH:
    // no break
  case SOCKET_TYPE::TYPE_INET4:
    // no break
  case SOCKET_TYPE::TYPE_INET6:
    this->_type = type_;
    status = true;
    break;
  default:
    status = false;
    break;
  }
  return (status);
}

std::string
Address::GetAddress() const
{
  return (this->_addr);
}

bool
Address::SetAddress(const std::string &addr_)
{
  this->_addr = addr_;
  return (true);
}

void
Address::Display() const
{
  std::cout << "SocketAddress: [" << int(this->GetType()) << "]: " << this->GetAddress() << std::endl;
}

}
}
