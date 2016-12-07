//*****************************************************************************
//
//
//
//*****************************************************************************

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

namespace zUtils
{
namespace zSocket
{

SocketAddress::SocketAddress(const SocketType type_, const std::string &addr_) :
    _type(type_), _addr(addr_)
{
}

SocketAddress::SocketAddress(SocketAddress &other_) :
    _type(other_._type), _addr(other_._addr)
{
}

SocketAddress::SocketAddress(const SocketAddress &other_) :
    _type(other_._type), _addr(other_._addr)
{
}

SocketAddress::~SocketAddress()
{
}

SocketAddress &
SocketAddress::operator=(SocketAddress &other_)
{
  this->_type = other_._type;
  this->_addr = other_._addr;
  return (*this);
}

SocketAddress &
SocketAddress::operator=(const SocketAddress &other_)
{
  this->_type = other_._type;
  this->_addr = other_._addr;
  return (*this);
}

bool
SocketAddress::operator ==(const SocketAddress &other_) const
    {
  return ((this->_type == other_._type) && (this->_addr == other_._addr));
}

bool
SocketAddress::operator !=(const SocketAddress &other_) const
    {
  return ((this->_type != other_._type) || (this->_addr != other_._addr));
}

bool
SocketAddress::operator <(const SocketAddress &other_) const
    {
  return ((this->_type != other_._type) || (this->_addr < other_._addr));
}

bool
SocketAddress::operator >(const SocketAddress &other_) const
    {
  return ((this->_type != other_._type) || (this->_addr > other_._addr));
}

const SocketType
SocketAddress::Type() const
{
  return (this->_type);
}

bool
SocketAddress::Type(const SocketType type_)
{
  bool status = false;
  switch (type_)
  {
  case SocketType::TYPE_LOOP:

  case SocketType::TYPE_UNIX:

  case SocketType::TYPE_ETH:

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
SocketAddress::Address() const
{
  ZLOG_DEBUG("getting socket address: " + this->_addr);
  return (this->_addr);
}

bool
SocketAddress::Address(const std::string &addr_)
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

bool
SocketAddress::verify(const SocketType type_, const std::string &addr_)
{
  return ((type_ == SocketType::TYPE_NONE) || (type_ == SocketType::TYPE_TEST));
}

}
}
