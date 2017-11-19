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
#include <map>

#include <zutils/zThread.h>
#include <zutils/zInterface.h>
#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

struct hwaddr
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

  struct hwaddr
  HwAddress() const;

  bool
  HwAddress(const struct hwaddr addr_);

protected:

private:

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

class EthSocket : public Socket, public zThread::ThreadArg, zEvent::EventObserver
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

  EthSocket();

  virtual
  ~EthSocket();

  bool
  Open(const PROTO proto_);

  virtual bool
  Open();

  bool
  Close(const PROTO proto_);

  virtual void
  Close();

  bool
  Bind(const zInterface::Interface& iface_);

protected:

  std::map<PROTO, int> _sockfd;

  virtual bool
  _bind();

  virtual ssize_t
  _recv(const int fd_, zSocket::EthAddress &src_, zSocket::SocketBuffer &sb_);

  virtual ssize_t
  _send(const int fd_, const zSocket::EthAddress &dst_, zSocket::SocketBuffer &sb_);

  virtual bool
  EventHandler(zEvent::EventNotification* notification_);

private:

  zInterface::Interface* _iface;
  zEvent::EventHandler _iface_handler;

  zThread::Thread _rx_thread;
  EthSocketRecv _rx_func;
  zThread::Thread _tx_thread;
  EthSocketSend _tx_func;

};

}
}

#endif /* __ZETHSOCKET_H__ */
