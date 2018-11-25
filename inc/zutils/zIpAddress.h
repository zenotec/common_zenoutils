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

#ifndef __ZIPADDRESS_H__
#define __ZIPADDRESS_H__

#include <netinet/in.h>

#include <string>

#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// Class: zSocket::Ipv4Address
//**********************************************************************

class Ipv4Address : public Address
{

public:

  Ipv4Address (const in_addr_t ip_, const in_port_t port_);

  Ipv4Address(const std::string &addr_ = std::string("0.0.0.0:0"));

  Ipv4Address(const Address& addr_);

  Ipv4Address(const struct sockaddr_in& sa_);

  virtual
  ~Ipv4Address();

  virtual std::string
  GetAddress() const;

  virtual bool
  SetAddress(const std::string &addr_);

  uint32_t
  GetIp() const;

  std::string
  GetIpString() const;

  bool
  SetIp(const uint32_t ip_);

  bool
  SetIp(const std::string& ip_);

  uint16_t
  GetPort() const;

  std::string
  GetPortString() const;

  bool
  SetPort(const uint16_t port_);

  bool
  SetPort(const std::string& port_);

  struct sockaddr_in
  GetSA() const;

  bool
  SetSA(const struct sockaddr_in& sa_);

  virtual void
  Display() const;

protected:

private:

  struct sockaddr_in _sa;

};

}
}

#endif /* __ZIPADDRESS_H__ */
