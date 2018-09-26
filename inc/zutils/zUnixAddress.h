/*
 * Copyright (c) 2018 ZenoTec LLC (http://www.zenotec.net)
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

#ifndef __ZUNIXADDRESS_H__
#define __ZUNIXADDRESS_H__

#include <sys/un.h>

#include <string>

#include <zutils/zThread.h>
#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// Class: zSocket::UnixAddress
//**********************************************************************

class UnixAddress :
    public Address
{

public:

  UnixAddress(const std::string &addr_ = "");

  UnixAddress(const Address& addr_);

  UnixAddress(const struct sockaddr_un& sa_);

  virtual
  ~UnixAddress();

  virtual std::string
  GetAddress() const;

  virtual bool
  SetAddress(const std::string &addr_);

  struct sockaddr_un
  GetSA() const;

  bool
  SetSA(const struct sockaddr_un& sa_);

  virtual void
  Display() const;

protected:

private:

  struct sockaddr_un _sa;

};

}
}

#endif /* __ZUNIXADDRESS_H__ */
