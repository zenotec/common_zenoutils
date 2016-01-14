//*****************************************************************************
//
//
//
//*****************************************************************************

#include <zutils/zLog.h>
#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

SocketAddress::SocketAddress(SocketAddress::TYPE type_, const std::string &addr_) :
    _type(type_), _addr(addr_)
{
}

SocketAddress::SocketAddress(SocketAddress &other_) :
    _type(other_._type)
{
  this->SetAddress(other_.GetAddress());
}

SocketAddress::SocketAddress(const SocketAddress &other_) :
  _type(other_._type)
{
  this->SetAddress(other_.GetAddress());
}

SocketAddress::~SocketAddress()
{
}

SocketAddress &
SocketAddress::operator=(SocketAddress &other_)
{
  this->SetType(other_.GetType());
  this->SetAddress(other_.GetAddress());
  return (*this);
}

SocketAddress &
SocketAddress::operator=(const SocketAddress &other_)
{
  this->SetType(other_.GetType());
  this->SetAddress(other_.GetAddress());
  return (*this);
}

bool
SocketAddress::operator ==(const SocketAddress &other_) const
{
  return ((this->GetType() == other_.GetType()) && (this->GetAddress() == other_.GetAddress()));
}

bool
SocketAddress::operator !=(const SocketAddress &other_) const
{
  return ((this->GetType() != other_.GetType()) || (this->GetAddress() != other_.GetAddress()));
}

bool
SocketAddress::operator <(const SocketAddress &other_) const
    {
  return ((this->GetType() != other_.GetType()) || (this->GetAddress() < other_.GetAddress()));
}

bool
SocketAddress::operator >(const SocketAddress &other_) const
    {
  return ((this->GetType() != other_.GetType()) || (this->GetAddress() > other_.GetAddress()));
}

SocketAddress::TYPE
SocketAddress::GetType() const
{
  return (this->_type);
}

bool
SocketAddress::SetType(const SocketAddress::TYPE &type_)
{
  bool status = false;
  switch (type_)
  {
  case SocketAddress::TYPE_LOOP:
  case SocketAddress::TYPE_INET:
  case SocketAddress::TYPE_MCAST:
  case SocketAddress::TYPE_BCAST:
    this->_type = type_;
    status = true;
    break;
  default:
    status = false;
  }
  return(status);
}

std::string
SocketAddress::GetAddress() const
{
  ZLOG_DEBUG("getting socket address: " + this->_addr);
  return (this->_addr);
}

bool
SocketAddress::SetAddress(const std::string &addr_)
{
  ZLOG_DEBUG("setting socket address: " + addr_);
  this->_addr = addr_;
  return (true);
}

}
}
