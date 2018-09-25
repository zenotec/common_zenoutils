/*
 * Copyright (c) 2014-2018 ZenoTec LLC (http://www.zenotec.net)
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

#ifndef __ZRAWADDRESS_H__
#define __ZRAWADDRESS_H__

#include <stdint.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>

#include <string>

#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// Class: zSocket::RawAddress
//**********************************************************************

class RawAddress :
    public Address
{

public:

  RawAddress(const std::string &addr_ = "");

  RawAddress(const Address& addr_);

  RawAddress(const struct sockaddr_ll& sa_);

  virtual
  ~RawAddress();

  virtual std::string
  GetAddress() const;

  virtual bool
  SetAddress(const std::string &addr_);

  struct sockaddr_ll
  GetSA() const;

  bool
  SetSA(const struct sockaddr_ll& sa_);

  virtual void
  Display() const;

protected:

private:

  struct sockaddr_ll _sa;

};

}
}

#endif /* __ZRAWADDRESS_H__ */
