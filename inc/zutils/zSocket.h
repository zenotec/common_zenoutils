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

#include <time.h>

#include <map>

#include <zutils/zCompatibility.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>
#include <zutils/zThread.h>

namespace zUtils
{
namespace zSocket
{

class Buffer;
class Address;
class Socket;
class Notification;
class Tap;
class Handler;


enum SOCKET_TYPE
{
  TYPE_ERR = -1,
  TYPE_NONE = 0,
  TYPE_TEST = 1,
  TYPE_LOOP = 2,
  TYPE_UNIX = 3,
  TYPE_ETH = 4,
  TYPE_INET4 = 5,
  TYPE_INET6 = 6,
  TYPE_LAST
};

//**********************************************************************
// Class: zSocket::Buffer
//**********************************************************************

struct skbmem;

class Buffer
{

public:

  Buffer(const size_t size_ = (8 * 1024));

  Buffer(const std::string &str_);

  Buffer(const Buffer &other_);

  virtual
  ~Buffer();

  Buffer &
  operator=(const Buffer &other_);

  bool
  operator==(const Buffer &other_) const;

  bool
  operator!=(const Buffer &other_) const;

  uint8_t *
  Head() const;

  bool
  Put(off_t off_);

  bool
  Push(off_t off_);

  bool
  Pull(off_t off_);

  bool
  Reset();

  uint8_t *
  Data() const;

  uint8_t *
  Tail() const;

  uint8_t *
  End() const;

  size_t
  Length() const;

  size_t
  Size() const;

  size_t
  TotalSize() const;

  std::string
  Str() const;

  bool
  Str(const std::string &str_);

  void
  Display() const;

protected:

private:

  struct timespec _ts;
  SHARED_PTR(struct skbmem) _skbmem;
  uint8_t *_head;
  size_t _data;
  size_t _tail;
  size_t _end;

  void
  _init(const size_t size_);

  void
  _copy (const Buffer& other_);

};

//**********************************************************************
// Class: zSocket::Address
//**********************************************************************

class Address
{

public:

  Address(const SOCKET_TYPE type, const std::string& addr_ = std::string(""));

  Address(const Address &other_);

  virtual
  ~Address();

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

  SOCKET_TYPE
  GetType() const;

  bool
  SetType(const SOCKET_TYPE type_);

  virtual std::string
  GetAddress() const;

  virtual bool
  SetAddress(const std::string &addr_);

  virtual void
  Display() const;

protected:

private:

  SOCKET_TYPE _type;
  std::string _addr;

};

//**********************************************************************
// Class: zSocket::Socket
//**********************************************************************

class Socket :
    public zEvent::Event
{

  friend Tap;
  friend Handler;

public:

  enum OPTIONS
  {
    OPTIONS_ERR = -1,
    OPTIONS_NONE = 0,
    OPTIONS_ALLOW_BCAST, // Allow sending of broadcast packets
    OPTIONS_NONBLOCK, // Do not block
    OPTIONS_ALLOW_REUSE, // Allow multiple socket to use address
    OPTIONS_TOS_LP, // Low priority
    OPTIONS_TOS_NP, // Normal priority
    OPTIONS_TOS_HP, // High priority
    OPTIONS_TOS_UHP, // Ultra-high priority
    OPTIONS_LAST
  };

  Socket(const SOCKET_TYPE type_);

  virtual
  ~Socket();

  virtual int
  GetId() const;

  const SOCKET_TYPE
  GetType() const;

  virtual const Address&
  GetAddress() const = 0;

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

  virtual SHARED_PTR(zSocket::Notification)
  Send(const Address& to_, const std::string& str_);

protected:

private:

  const SOCKET_TYPE _type;

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

class Notification :
    public zEvent::Notification
{

  friend Socket;

public:

  enum SUBTYPE
  {
    SUBTYPE_ERR = -1,
    SUBTYPE_NONE = 0,
    SUBTYPE_PKT_RCVD = 1,
    SUBTYPE_PKT_SENT = 2,
    SUBTYPE_PKT_ERR = 3,
    SUBTYPE_LAST
  };

  Notification(Socket& sock_);

  virtual
  ~Notification();

  Socket&
  GetSocket() const;

  int
  GetId() const;

  Notification::SUBTYPE
  GetSubType() const;

  void
  SetSubType(Notification::SUBTYPE subtype_);

  const Address&
  GetSrcAddress() const;

  void
  SetSrcAddress(const Address& sa_);

  const Address&
  GetDstAddress() const;

  void
  SetDstAddress(const Address& da_);

  const Buffer&
  GetBuffer() const;

  void
  SetBuffer(const Buffer& sb_);

protected:

private:

  Notification::SUBTYPE _subtype;
  Address _sa;
  Address _da;
  Buffer _sb;

};

//**********************************************************************
// Class: zSocket::Tap
//**********************************************************************

class Tap
{

public:

  Tap(Socket& sock_);

  virtual
  ~Tap();

  bool
  Recv(const Address& to_, Buffer& sb_);

  bool
  Send(const Address& to_, Buffer& sb_);

protected:

private:

  Socket& _sock;

};

//**********************************************************************
// Class: zSocket::Observer
//**********************************************************************

class Observer :
    public zEvent::Observer
{

public:

  Observer()
  {
  }

  virtual
  ~Observer()
  {
  }

protected:

  virtual bool
  ObserveEvent(SHARED_PTR(zEvent::Notification)noti_)
  {
    bool status = false;
    if (noti_ && (noti_->GetType() == zEvent::Event::TYPE_SOCKET))
    {
      status = this->ObserveEvent(STATIC_CAST(Notification)(noti_));
    }
    return (status);
  }

  virtual bool
  ObserveEvent(SHARED_PTR(zSocket::Notification)noti_) = 0;

private:

};

//**********************************************************************
// Class: zSocket::Adapter
//**********************************************************************

class Adapter :
    public zEvent::Adapter
{

public:

  Adapter(Socket& socket_);

  virtual
  ~Adapter();

protected:

  virtual SHARED_PTR(zEvent::Notification)
  AdaptEvent(SHARED_PTR(zEvent::Notification) noti_);

  virtual SHARED_PTR(zSocket::Notification)
  AdaptEvent(SHARED_PTR(zSocket::Notification) noti_) = 0;

private:

  Socket& _socket;

};

//**********************************************************************
// Class: zSocket::Handler
//**********************************************************************

class Handler :
    public zEvent::Handler,
    public zThread::ThreadFunction
{
public:

  Handler();

  virtual
  ~Handler();

  bool
  RegisterSocket(Socket* socket_);

  bool
  UnregisterSocket(Socket* socket_);

protected:

  virtual void
  Run(zThread::ThreadArg *arg_);

private:

  zSem::Mutex _lock;
  std::map<int, Socket*> _sock_list;
  zThread::Thread _thread;

};

//**********************************************************************
// Class: zSocket::Manager
//**********************************************************************

class Manager : public Handler
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
