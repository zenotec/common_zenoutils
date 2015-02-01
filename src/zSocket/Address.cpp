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

Address::Address(Address::TYPE type_, const std::string &addr_) :
    _type(Address::TYPE_NONE), _addr(addr_)
{
  this->SetType(type_);
}

Address::~Address()
{
}

bool
Address::operator ==(const Address &other_) const
{
  return ((this->GetType() == other_.GetType()) && (this->GetAddress() == other_.GetAddress()));
}

bool
Address::operator !=(const Address &other_) const
{
  return ((this->GetType() != other_.GetType()) || (this->GetAddress() != other_.GetAddress()));
}

bool
Address::operator <(const Address &other_) const
{
  return ((this->GetType() != other_.GetType()) || (this->GetAddress() < other_.GetAddress()));
}

bool
Address::operator >(const Address &other_) const
{
  return ((this->GetType() != other_.GetType()) || (this->GetAddress() > other_.GetAddress()));
}

Address::TYPE
Address::GetType() const
{
  return (this->_type);
}

bool
Address::SetType(const Address::TYPE &type_)
{
  bool status = true;
  switch (type_)
  {
  case Address::TYPE_LOOP:
    case Address::TYPE_INET:
    this->_type = type_;
    break;
  default:
    this->_type = Address::TYPE_ERR;
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

std::string
Address::GetBroadcast() const
{
  return (this->_bcaddr);
}

bool
Address::SetBroadcast(const std::string &addr_)
{
  this->_bcaddr = addr_;
  return (true);
}

}
}
