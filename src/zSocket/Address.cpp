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
    _type(type_), _addr(addr_)
{
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

}
}