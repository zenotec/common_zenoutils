//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zEthSocket.h
//    Description:
//
//*****************************************************************************

#ifndef __ZETHSOCKET_H__
#define __ZETHSOCKET_H__

#include <stdint.h>

#include <string>

#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// EthAddress Class
//**********************************************************************

class EthAddress
{

public:
  static const unsigned int ETHADDR_LEN;

  EthAddress(const Address &addr_);
  EthAddress(const std::string &addr_ = std::string("00:00:00:00:00:00"));
  EthAddress(const struct sockaddr_in &addr_);

  virtual
  ~EthAddress();

  bool
  operator ==(const EthAddress &other_) const;
  bool
  operator !=(const EthAddress &other_) const;
  bool
  operator <(const EthAddress &other_) const;
  bool
  operator >(const EthAddress &other_) const;

  Address
  GetAddr() const;
  std::string
  GetAddrString() const;
  struct sockaddr_in
  GetAddrSockAddr() const;

  bool
  GetAddr(Address &addr_) const;
  bool
  GetAddr(std::string &addr_) const;
  bool
  GetAddr(struct sockaddr_in &addr_) const;

  bool
  SetAddr(const Address &addr_);
  bool
  SetAddr(const std::string &addr_);
  bool
  SetAddr(const struct sockaddr_in &addr_);

protected:

private:
  uint8_t *_ethaddr;

};

}
}

#endif /* __ZETHSOCKET_H__ */
