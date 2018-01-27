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
#include <zutils/zEvent.h>

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
// Class: zSocket::Buffer
//**********************************************************************

class Buffer
{

public:

  Buffer(const size_t size_ = 1500);

  Buffer(const std::string &str_);

  Buffer(Buffer &other_);

  Buffer(const Buffer &other_);

  virtual
  ~Buffer();

  Buffer &
  operator=(Buffer &other_);

  Buffer &
  operator=(const Buffer &other_);

  bool
  operator==(Buffer &other_);

  bool
  operator!=(Buffer &other_);

  uint8_t *
  Head();

  bool
  Put(off_t off_);

  bool
  Push(off_t off_);

  bool
  Pull(off_t off_);

  bool
  Reset();

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
// Class: zSocket::Address
//**********************************************************************

class Address
{

public:

  Address(const SocketType type = SocketType::TYPE_NONE,
      const std::string &addr_ = std::string(""));

  Address(Address &other_);

  Address(const Address &other_);

  virtual
  ~Address();

  Address &
  operator=(Address &other_);

  Address &
  operator=(const Address &other_);

  bool
  operator ==(const Address &other_) const;
  bool
  operator !=(const Address &other_) const;
  bool
  operator <(const Address &other_) const;
  bool
  operator >(const Address &other_) const;

  const SocketType
  GetType() const;

  bool
  SetType(const SocketType type_);

  std::string
  GetAddress() const;

  bool
  SetAddress(const std::string &addr_);

  virtual void
  Display() const;

protected:

  std::string _addr;

  virtual bool
  verify(const SocketType type_, const std::string &addr_);

private:

  SocketType _type;

};

//**********************************************************************
// Class: zSocket::SocketAddressFactory
//**********************************************************************

class SocketAddressFactory
{
public:

  static Address*
  Create(const SocketType type_, const std::string& addr_ = "");

  static Address*
  Create(const Address& addr_);

  static Address*
  Create(const std::string& addr_);

private:

};

//**********************************************************************
// Typedef: zSocket::SocketAddressBufferPair
//**********************************************************************

typedef std::pair<SHARED_PTR(const Address), SHARED_PTR(Buffer)> AddressBufferPair;

//**********************************************************************
// Typedef: zSocket::SocketAddressBufferQueue
//**********************************************************************

typedef zQueue<AddressBufferPair> AddressBufferQueue;

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
  GetType() const;

  const Address&
  GetAddress() const;

  virtual bool
  Open() = 0;

  virtual void
  Close() = 0;

  virtual bool
  Getopt(Socket::OPTIONS opt_);

  virtual bool
  Setopt(Socket::OPTIONS opt_);

  bool
  Bind(const Address& addr_);

  ssize_t
  Send(AddressBufferPair& pair_);

  ssize_t
  Send(const Address& to_, Buffer& sb_);

  ssize_t
  Send(const Address& to_, const std::string& str_);

protected:

  Address* _addr;

  virtual bool
  _bind() = 0;

  // Called by derived class to process a received packet
  bool
  rxbuf(AddressBufferPair& pair_);

  // Called by derived class to get packet to send
  bool
  txbuf(AddressBufferPair& pair_, size_t timeout_ = 1000000 /* usec */);

private:

  const SocketType _type;
  AddressBufferQueue _txq;

  Socket(Socket &other_);

  Socket(const Socket &other_);

  Socket &
  operator=(Socket &other_);

  Socket &
  operator=(const Socket &other_);

};

//**********************************************************************
// Class: zSocket::Notification
//**********************************************************************

class Notification : public zEvent::Notification
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

  Notification(Socket& sock_);

  virtual
  ~Notification();

  Notification::ID
  Id() const;

  Socket&
  Sock();

  AddressBufferPair
  Pkt() const;

protected:

  void
  id(Notification::ID id_);

  void
  pkt(AddressBufferPair &pkt_);

private:

  Notification::ID _id;
  AddressBufferPair _pkt;

};

//**********************************************************************
// Class: zSocket::Manager
//**********************************************************************

class Manager : public zEvent::Handler
{
public:

  static Manager&
  Instance()
  {
    static Manager instance;
    return instance;
  }

protected:

private:

  Manager()
  {
  }

  Manager(Manager const&);

  void
  operator=(Manager const&);

};

}
}

#endif /* __ZSOCKET_H__ */
