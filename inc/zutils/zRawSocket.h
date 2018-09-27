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

#ifndef __ZSOCKET_RAWSOCKET_H__
#define __ZSOCKET_RAWSOCKET_H__

#include <linux/if_packet.h>
#include <linux/if_ether.h>

#include <string>

#include <zutils/zThread.h>
#include <zutils/zSocket.h>
#include <zutils/zRawAddress.h>
#include <zutils/zMacAddress.h>

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// Class: zSocket::RawSocket
//**********************************************************************

class RawSocket :
    public Socket
{

public:

  enum PROTO
  {
    PROTO_ERR = -1,
    PROTO_NONE = 0,
    PROTO_ETH = ETH_P_802_3,
    PROTO_ALL = ETH_P_ALL,
    PROTO_LLC = ETH_P_802_2,
    PROTO_LOOP = ETH_P_LOOP,
    PROTO_IP = ETH_P_IP,
    PROTO_LAST
  };

  enum PACKETTYPE

  {
    PACKETTYPE_ERR = -1,
    PACKETTYPE_NONE = 0,
    PACKETTYPE_HOST = PACKET_HOST,
    PACKETTYPE_BCAST = PACKET_BROADCAST,
    PACKETTYPE_MCAST = PACKET_MULTICAST,
    PACKETTYPE_OTHERHOST = PACKET_OTHERHOST,
    PACKETTYPE_OUTGOING = PACKET_OUTGOING,
    PACKETTYPE_LOOP = PACKET_LOOPBACK,
    PACKETTYPE_USER = PACKET_USER,
    PACKETTYPE_KERNEL = PACKET_KERNEL,
    PACKETTYPE_LAST
  };

  RawSocket(const RawSocket::PROTO proto_ = PROTO_ALL,
      const RawSocket::PACKETTYPE ptype_ = PACKETTYPE_HOST);

  virtual
  ~RawSocket();

  virtual int
  GetId() const;

  virtual bool
  Getopt(Socket::OPTIONS opt_);

  virtual bool
  Setopt(Socket::OPTIONS opt_);

  // Address is name of interface
  virtual bool
  Bind(const Address& addr_);

  virtual SHARED_PTR(zSocket::Notification)
  Recv();

  virtual SHARED_PTR(zSocket::Notification)
  Send(const Address& to_, const Buffer& sb_);

protected:

private:

  int _fd;
  RawAddress _addr;
  RawSocket::PROTO _proto;
  RawSocket::PACKETTYPE _ptype;

};

}
}

#endif /* __ZSOCKET_RAWSOCKET_H__ */
