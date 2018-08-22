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

#ifndef __ZETHSOCKET_H__
#define __ZETHSOCKET_H__

#include <linux/if_packet.h>
#include <linux/if_ether.h>

#include <string>

#include <zutils/zThread.h>
#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

class RawSocket;

//**********************************************************************
// Class: zSocket::EthAddress
//**********************************************************************

class EthAddress :
    public Address
{

  friend RawSocket;

public:

  EthAddress(const std::string &addr_ = "00:00:00:00:00:00");

  EthAddress(const Address& addr_);

  EthAddress(const struct sockaddr_ll& sa_);

  virtual
  ~EthAddress();

  virtual std::string
  GetAddress() const;

  virtual bool
  SetAddress(const std::string &addr_);

  virtual void
  Display() const;

protected:

  struct sockaddr_ll sa;

private:

};

//**********************************************************************
// Class: zSocket::EthSocket
//**********************************************************************

class RawSocket :
    public Socket
{

public:

  enum PROTO
  {
    PROTO_ERR = -1,
    PROTO_NONE = 1,
    PROTO_ALL = ETH_P_ALL,
    PROTO_LLC = ETH_P_802_2,
    PROTO_LOOP = ETH_P_LOOP,
    PROTO_IP = ETH_P_IP,
    PROTO_LAST
  };

  RawSocket(const RawSocket::PROTO proto_ = PROTO_ALL);

  virtual
  ~RawSocket();

  virtual int
  GetId() const;

  virtual const Address&
  GetAddress() const;

  virtual bool
  Getopt(Socket::OPTIONS opt_);

  virtual bool
  Setopt(Socket::OPTIONS opt_);

  virtual bool
  Bind(const Address& addr_);

  virtual SHARED_PTR(zSocket::Notification)
  Recv();

  virtual SHARED_PTR(zSocket::Notification)
  Send(const Address& to_, const Buffer& sb_);

protected:

private:

  int _fd;
  EthAddress _addr;

};

}
}

#endif /* __ZETHSOCKET_H__ */
