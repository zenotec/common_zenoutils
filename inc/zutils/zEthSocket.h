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
#include <linux/if_ether.h>

#include <string>

#include <zutils/zThread.h>
#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

struct eth_addr
{
  uint8_t addr[ETH_ALEN];
};

//**********************************************************************
// EthAddress Class
//**********************************************************************

class EthAddress : public SocketAddress
{

public:

  EthAddress(const std::string &addr_= std::string("00:00:00:00:00:00"));

  EthAddress(SocketAddress &addr_);

  EthAddress(const SocketAddress &addr_);

  virtual
  ~EthAddress();

  std::string
  IfName() const;

  int
  IfIndex() const;

  struct eth_addr
  Mac() const;

protected:

private:

  std::string _name;
  struct eth_addr _mac;

  virtual bool
  verify(const SocketType type_, const std::string &addr_);

};

//**********************************************************************
// zSocket::EthSocketRecv Class
//**********************************************************************

class EthSocketRecv : public zThread::ThreadFunction
{

public:

  EthSocketRecv()
  {
  }

  virtual
  ~EthSocketRecv()
  {
  }

  virtual void
  Run(zThread::ThreadArg *arg_);

protected:

private:

};

//**********************************************************************
// zSocket::EthSocketSend Class
//**********************************************************************

class EthSocketSend : public zThread::ThreadFunction
{

public:

  EthSocketSend()
  {
  }

  virtual
  ~EthSocketSend()
  {
  }

  virtual void
  Run(zThread::ThreadArg *arg_);

protected:

private:

};

//**********************************************************************
// zSocket::EthSocket Class
//**********************************************************************

class EthSocket : public Socket, public zThread::ThreadArg
{

  friend EthSocketRecv;
  friend EthSocketSend;

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

  EthSocket(const PROTO proto_ = PROTO_ALL);

  virtual
  ~EthSocket();

  virtual bool
  Open();

  virtual void
  Close();

protected:

  int _sock;

  virtual bool
  _bind();

  virtual ssize_t
  _recv(zSocket::EthAddress &src_, zSocket::SocketBuffer &sb_);

  virtual ssize_t
  _send(const zSocket::EthAddress &dst_, zSocket::SocketBuffer &sb_);

private:

  PROTO _proto;

  zThread::Thread _rx_thread;
  EthSocketRecv _rx_func;
  zThread::Thread _tx_thread;
  EthSocketSend _tx_func;

};

}
}

#endif /* __ZETHSOCKET_H__ */
