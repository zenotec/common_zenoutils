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

#ifndef __ZSOCKET_H__
#define __ZSOCKET_H__

#include <zutils/zCompatibility.h>
#include <zutils/zQueue.h>

namespace zUtils
{
namespace zSocket
{

typedef enum SocketType
{
  TYPE_ERR = -1,
  TYPE_NONE = 0,
  TYPE_TEST = 1,
  TYPE_LOOP = 2,
  TYPE_UNIX = 3,
  TYPE_ETH = 4,
  TYPE_INET = 5,
  TYPE_INET6 = 6,
  TYPE_LAST
} SocketType;

//**********************************************************************
// Class: zSocket::SocketBuffer
//**********************************************************************

class SocketBuffer
{

public:

  SocketBuffer(const size_t size_ = 1500);

  SocketBuffer(const std::string &str_);

  SocketBuffer(SocketBuffer &other_);

  SocketBuffer(const SocketBuffer &other_);

  virtual
  ~SocketBuffer();

  SocketBuffer &
  operator=(SocketBuffer &other_);

  SocketBuffer &
  operator=(const SocketBuffer &other_);

  bool
  operator==(SocketBuffer &other_);

  bool
  operator!=(SocketBuffer &other_);

  uint8_t *
  Head();

  bool
  Put(off_t off_);

  bool
  Push(off_t off_);

  bool
  Pull(off_t off_);

  uint8_t *
  Data(off_t off_ = 0);

  uint8_t *
  Tail();

  uint8_t *
  End();

  size_t
  Length();

  size_t
  Size();

  size_t
  TotalSize();

  std::string
  Str();

  bool
  Str(const std::string &str_);

protected:

private:

  uint8_t *_head;
  size_t _data;
  size_t _tail;
  size_t _end;

};

//**********************************************************************
// Class: zSocket::SocketAddress
//**********************************************************************

class SocketAddress
{

public:

  SocketAddress(const SocketType type = SocketType::TYPE_NONE,
      const std::string &addr_ = std::string(""));

  SocketAddress(SocketAddress &other_);

  SocketAddress(const SocketAddress &other_);

  virtual
  ~SocketAddress();

  SocketAddress &
  operator=(SocketAddress &other_);

  SocketAddress &
  operator=(const SocketAddress &other_);

  bool
  operator ==(const SocketAddress &other_) const;
  bool
  operator !=(const SocketAddress &other_) const;
  bool
  operator <(const SocketAddress &other_) const;
  bool
  operator >(const SocketAddress &other_) const;

  const SocketType
  Type() const;

  bool
  Type(const SocketType type_);

  std::string
  Address() const;

  bool
  Address(const std::string &addr_);

protected:

  std::string _addr;

  virtual bool
  verify(const SocketType type_, const std::string &addr_) = 0;

private:

  SocketType _type;

};

//**********************************************************************
// Class: zSocket::SocketAddressFactory
//**********************************************************************

class SocketAddressFactory
{
public:

  static SocketAddress*
  Create(const SocketType type_, const std::string& addr_ = "");

  static SocketAddress*
  Create(const SocketAddress& addr_);

  static SocketAddress*
  Create(const std::string& addr_);

private:

};

//**********************************************************************
// Typedef: zSocket::SocketAddressBufferPair
//**********************************************************************

typedef std::pair<SHARED_PTR(const SocketAddress), SHARED_PTR(SocketBuffer)> SocketAddressBufferPair;

//**********************************************************************
// Typedef: zSocket::SocketAddressBufferQueue
//**********************************************************************

typedef zQueue<SocketAddressBufferPair> SocketAddressBufferQueue;

//**********************************************************************
// Class: zSocket::Socket
//**********************************************************************

class Socket : public zEvent::Event
{

public:

  enum OPTIONS
  {
    OPTIONS_ERR = -1,
    OPTIONS_NONE = 0,
    OPTIONS_ALLOW_BCAST = 1,
    OPTIONS_NONBLOCK = 2,
    OPTIONS_TOS_LP = 3,
    OPTIONS_TOS_NP = 4,
    OPTIONS_TOS_HP = 5,
    OPTIONS_TOS_UHP = 6,
    OPTIONS_LAST
  };

  Socket(SocketType type_ = SocketType::TYPE_NONE);

  virtual
  ~Socket();

  const SocketType
  Type() const;

  const SocketAddress&
  Address() const;

  virtual bool
  Open() = 0;

  virtual void
  Close() = 0;

  virtual bool
  Getopt(Socket::OPTIONS opt_);

  virtual bool
  Setopt(Socket::OPTIONS opt_);

  bool
  Bind(const SocketAddress& addr_);

  ssize_t
  Send(SocketAddressBufferPair& pair_);

  ssize_t
  Send(const SocketAddress& to_, SocketBuffer& sb_);

  ssize_t
  Send(const SocketAddress& to_, const std::string& str_);

protected:

  SocketAddress* _addr;

  virtual bool
  _bind() = 0;

  // Called by derived class to process a received packet
  bool
  rxbuf(SocketAddressBufferPair& pair_);

  // Called by derived class to get packet to send
  bool
  txbuf(SocketAddressBufferPair& pair_, size_t timeout_ = 1000000 /* usec */);

private:

  const SocketType _type;
  SocketAddressBufferQueue _txq;

  Socket(Socket &other_);

  Socket(const Socket &other_);

  Socket &
  operator=(Socket &other_);

  Socket &
  operator=(const Socket &other_);

};

//**********************************************************************
// Class: zSocket::SocketNotification
//**********************************************************************

class SocketNotification : public zEvent::EventNotification
{

  friend Socket;

public:

  enum ID
  {
    ID_ERR = -1,
    ID_NONE = 0,
    ID_PKT_RCVD = 1,
    ID_PKT_SENT = 2,
    ID_PKT_ERR = 3,
    ID_LAST
  };

  SocketNotification(Socket* sock_);

  virtual
  ~SocketNotification();

  SocketNotification::ID
  Id() const;

  Socket*
  Sock();

  SocketAddressBufferPair
  Pkt() const;

protected:

  void
  id(SocketNotification::ID id_);

  void
  pkt(SocketAddressBufferPair &pkt_);

private:

  SocketNotification::ID _id;
  SocketAddressBufferPair _pkt;

};

//**********************************************************************
// Class: zSocket::SocketManager
//**********************************************************************

class SocketManager : public zEvent::EventHandler
{
public:

  static SocketManager&
  Instance()
  {
    static SocketManager instance;
    return instance;
  }

protected:

private:

  SocketManager()
  {
  }

  SocketManager(SocketManager const&);

  void
  operator=(SocketManager const&);

};

}
}

#endif /* __ZSOCKET_H__ */
