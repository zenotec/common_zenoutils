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
#include <zutils/zLoopSocket.h>
#include <zutils/zUnixSocket.h>
#include <zutils/zInetSocket.h>
#include <zutils/zEthSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// Class: zSocket::Address
//**********************************************************************

Address::Address(const SocketType type_, const std::string &addr_) :
    _type(type_), _addr(addr_)
{
}

Address::Address(Address &other_) :
    _type(other_._type), _addr(other_._addr)
{
}

Address::Address(const Address &other_) :
    _type(other_._type), _addr(other_._addr)
{
}

Address::~Address()
{
}

Address &
Address::operator=(Address &other_)
{
  this->_type = other_._type;
  this->_addr = other_._addr;
  return (*this);
}

Address &
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

const SocketType
Address::GetType() const
{
  return (this->_type);
}

bool
Address::SetType(const SocketType type_)
{
  bool status = false;
  switch (type_)
  {
  case SocketType::TYPE_LOOP:
    // no break
  case SocketType::TYPE_UNIX:
    // no break
  case SocketType::TYPE_ETH:
    // no break
  case SocketType::TYPE_INET:
    if (this->verify(type_, this->_addr))
    {
      this->_type = type_;
      status = true;
    }
    break;
  default:
    break;
  }
  return (status);
}

std::string
Address::GetAddress() const
{
  ZLOG_DEBUG("getting socket address: " + this->_addr);
  return (this->_addr);
}

bool
Address::SetAddress(const std::string &addr_)
{
  bool status = false;
  ZLOG_DEBUG("setting socket address: " + addr_);
  if (this->verify(this->_type, addr_))
  {
    this->_addr = addr_;
    status = true;
  }
  return (status);
}

void
Address::Display() const
{
  std::cout << "SocketAddress: [" << (int)this->GetType() << "]: " << this->GetAddress() << std::endl;
}

bool
Address::verify(const SocketType type_, const std::string &addr_)
{
  return (false);
}

//**********************************************************************
// Class: zSocket::SocketAddressFactory
//**********************************************************************

Address*
SocketAddressFactory::Create(const SocketType type_, const std::string& addr_)
{
  Address* addr = NULL;
  switch (type_)
  {
  case SocketType::TYPE_LOOP:
    addr = new LoopAddress;
    break;
  case SocketType::TYPE_UNIX:
    addr = new UnixAddress(addr_);
    break;
  case SocketType::TYPE_ETH:
    addr = new EthAddress(addr_);
    break;
  case SocketType::TYPE_INET:
    addr = new InetAddress(addr_);
    break;
  default:
    break;
  }
  return(addr);
}

Address*
SocketAddressFactory::Create(const Address& addr_)
{
  return(SocketAddressFactory::Create(addr_.GetType(), addr_.GetAddress()));
}

Address*
SocketAddressFactory::Create(const std::string& addr_)
{
  Address* addr = NULL;

  // First try to create an INET address
  addr = new InetAddress;
  if (addr)
  {
    if (!addr->SetAddress(addr_))
    {
      delete(addr);
      addr = NULL;
    }
  }

  // Next try to create an Ethernet address
  if (!addr)
  {
    addr = new EthAddress;
    if (addr)
    {
      if (!addr->SetAddress(addr_))
      {
        delete(addr);
        addr = NULL;
      }
    }
  }

  // Next try to create a new Unix address
  if (!addr)
  {
    addr = new UnixAddress;
    if (addr)
    {
      if (!addr->SetAddress(addr_))
      {
        delete(addr);
        addr = NULL;
      }
    }
  }

  // Next try to create a new Loop address
  if (!addr)
  {
    addr = new LoopAddress;
    if (addr)
    {
      if (!addr->SetAddress(addr_))
      {
        delete(addr);
        addr = NULL;
      }
    }
  }

  return(addr);
}

}
}
