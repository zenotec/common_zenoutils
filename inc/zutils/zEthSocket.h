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

class EthAddress : public Address
{
public:
  static const unsigned int ETHADDR_LEN;

  EthAddress(const std::string &addr_ = std::string("00:00:00:00:00:00"));
  EthAddress(const uint8_t *addr_);

  virtual
  ~EthAddress();

  const uint8_t *
  GetHwAddr() const;

  virtual bool
  SetAddr(const std::string &addr_);
  bool
  SetAddr(const uint8_t *addr_);

protected:
private:
  uint8_t *_ethaddr;
};

}
}

#endif /* __ZETHSOCKET_H__ */
