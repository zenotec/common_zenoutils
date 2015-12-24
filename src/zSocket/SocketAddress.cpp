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

SocketAddress &
SocketAddress::operator=(SocketAddress &other_)
{
  this->_type = other_._type;
  this->SetAddress(other_.GetAddress());
  return (*this);
}

SocketAddress &
SocketAddress::operator=(const SocketAddress &other_)
{
  this->_type = other_._type;
  this->SetAddress(other_.GetAddress());
  return (*this);
}

SocketAddress::~SocketAddress()
{
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

std::string
SocketAddress::GetAddress() const
{
  return (this->_addr);
}

bool
SocketAddress::SetAddress(const std::string &addr_)
{
  this->_addr = addr_;
  return (true);
}

}
}
