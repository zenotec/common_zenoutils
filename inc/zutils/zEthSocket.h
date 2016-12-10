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

  EthAddress(const SocketAddress &addr_);
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

  SocketAddress
  GetAddr() const;
  std::string
  GetAddrString() const;
  struct sockaddr_in
  GetAddrSockAddr() const;

  bool
  GetAddr(SocketAddress &addr_) const;
  bool
  GetAddr(std::string &addr_) const;
  bool
  GetAddr(struct sockaddr_in &addr_) const;

  bool
  SetAddr(const SocketAddress &addr_);
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
