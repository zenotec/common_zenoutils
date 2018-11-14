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

#ifndef __ZSOCKET_H__
#define __ZSOCKET_H__

// libc includes

#include <time.h>

// libc++ includes

#include <map>
#include <vector>

// libzutils includes

#include <zutils/zCompatibility.h>
#include <zutils/zEvent.h>
#include <zutils/zThread.h>

// local includes

namespace zUtils
{
namespace zSocket
{

class Socket;
class Notification;
class Tap;
class Handler;

//**********************************************************************
// Class: zSocket::Buffer
//**********************************************************************

struct skbmem;

class Buffer
{

public:

  Buffer(const size_t size_ = 0);

  Buffer(const uint8_t* data_, const size_t len_);

  Buffer(const std::string& str_);

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

  uint8_t *
  Data() const;

  uint8_t *
  Tail() const;

  uint8_t *
  End() const;

  bool
  Reserve(off_t off_);

  bool
  Put(off_t off_);

  bool
  Pull(off_t off_);

  bool
  Pop(off_t off_);

  bool
  Push(off_t off_);

  bool
  Reset();

  size_t
  Headroom() const;

  size_t
  Tailroom() const;

  size_t
  Length() const;

  size_t
  Size() const;

  size_t
  TotalSize() const;

  const struct timespec&
  Timestamp() const;

  bool
  Timestamp(const struct timespec& ts_);

  // Read from data to caller's buffer 'len_' bytes
  bool
  Read(uint8_t* data_, const size_t len_);

  // Write to data from caller's buffer 'len_' bytes
  bool
  Write(const uint8_t* p_, const size_t len_);

  std::string
  String() const;

  bool
  String(const std::string& str_);

  void
  Display() const;

protected:

private:

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

  enum TYPE
  {
    TYPE_ERR = -1,
    TYPE_NONE = 0,
    TYPE_LOOP,
    TYPE_RAW,
    TYPE_MAC,
    TYPE_IPv4,
    TYPE_IPv6,
    TYPE_UNIX,
    TYPE_LAST
  };

  Address(const Address::TYPE type_, const std::string& addr_ = std::string(""));

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

  Address::TYPE
  GetType() const;

  bool
  SetType(const Address::TYPE type_);

  virtual std::string
  GetAddress() const;

  virtual bool
  SetAddress(const std::string &addr_);

  virtual void
  Display() const;

protected:

private:

  Address::TYPE _type;
  std::string _addr;

};

//**********************************************************************
// Class: zSocket::Frame
//**********************************************************************

class Frame
{

public:

  enum TYPE
  {
    TYPE_ERR = -1,
    TYPE_NONE = 0,
    TYPE_8023,
    TYPE_80211,
    TYPE_OTHER,
    TYPE_LAST
  };

  Frame(const Frame::TYPE);

  virtual
  ~Frame();

  virtual bool
  Assemble(zSocket::Buffer& sb_, bool fcs_ = false) = 0;

  virtual bool
  Disassemble(zSocket::Buffer& sb_, bool fcs_ = false) = 0;

  virtual bool
  Peek(const zSocket::Buffer& sb_, bool fcs_ = false) = 0;

  Frame::TYPE
  GetType() const;

  bool
  SetType(const Frame::TYPE type_);

  std::string
  GetDestination() const;

  bool
  SetDestination(const std::string& dst_);

  std::string
  GetSource() const;

  bool
  SetSource(const std::string& src_);

  size_t
  GetPayload(uint8_t* buf_, const size_t len_) const;

  size_t
  GetPayloadLength() const;

  bool
  PutPayload(const uint8_t* buf_, const size_t len_);

  virtual void
  Display(const std::string& prefix_ = "") const;

protected:

private:

  Frame::TYPE _type;
  std::string _src;
  std::string _dst;
  std::vector<uint8_t> _payload;


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

  enum SOCKET_TYPE
  {
    TYPE_ERR = -1,
    TYPE_NONE = 0,
    TYPE_TEST = 1,
    TYPE_LOOP = 2,
    TYPE_UNIX = 3,
    TYPE_RAW = 4,
    TYPE_INET4 = 5,
    TYPE_INET6 = 6,
    TYPE_LAST
  };

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
    OPTIONS_PROMISC,
    OPTIONS_LAST
  };

  Socket(const Socket::SOCKET_TYPE type_);

  virtual
  ~Socket();

  virtual int
  GetFd() const;

  const Socket::SOCKET_TYPE
  GetType() const;

  virtual const Address&
  GetAddress() const;

  virtual bool
  SetAddress(const Address& addr_);

  virtual bool
  Getopt(Socket::OPTIONS opt_);

  virtual bool
  Setopt(Socket::OPTIONS opt_);

  virtual bool
  Bind(const Address& addr_);

  virtual SHARED_PTR(zSocket::Notification)
  Recv();

  virtual bool
  Send(const Address& to_, const Buffer& sb_);

  virtual bool
  Send(Frame& frame_);

  virtual bool
  Send(const Address& to_, const std::string& str_);

protected:

  zQueue::Queue<SHARED_PTR(zSocket::Notification)> txq; // transmit queue
  zQueue::Queue<SHARED_PTR(zSocket::Notification)> rxq; // receive queue

private:

  const Socket::SOCKET_TYPE _type;
  Address _addr;

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
    SUBTYPE_PKT_DROP = 3,
    SUBTYPE_PKT_ERR = 4,
    SUBTYPE_LAST
  };

  Notification(Socket& sock_);

  virtual
  ~Notification();

  Socket&
  GetSocket() const;

  Notification::SUBTYPE
  GetSubType() const;

  bool
  SetSubType(const Notification::SUBTYPE subtype_);

  SHARED_PTR(Address)
  GetSrcAddress() const;

  bool
  SetSrcAddress(SHARED_PTR(Address) sa_);

  SHARED_PTR(Address)
  GetDstAddress() const;

  bool
  SetDstAddress(SHARED_PTR(Address) da_);

  SHARED_PTR(Buffer)
  GetBuffer() const;

  bool
  SetBuffer(SHARED_PTR(Buffer) sb_);

  SHARED_PTR(Frame)
  GetFrame() const;

  bool
  SetFrame(const SHARED_PTR(Frame) frame_);

  void
  Display(const std::string& prefix_ = "");

protected:

private:

  Notification::SUBTYPE _subtype;
  SHARED_PTR(Address) _sa;
  SHARED_PTR(Address) _da;
  SHARED_PTR(Buffer) _sb;
  SHARED_PTR(Frame) _frame;

};

//**********************************************************************
// Class: zSocket::Adapter
//**********************************************************************

class Adapter :
    public Socket
{

public:

  Adapter(Socket& socket_);

  virtual
  ~Adapter();

  virtual int
  GetFd() const;

  virtual const zSocket::Address&
  GetAddress() const;

  virtual bool
  Getopt(Socket::OPTIONS opt_);

  virtual bool
  Setopt(Socket::OPTIONS opt_);

  virtual bool
  Bind(const zSocket::Address& addr_);

  virtual SHARED_PTR(zSocket::Notification)
  Recv();

  virtual bool
  Send(const zSocket::Address& to_, const zSocket::Buffer& sb_);

  virtual bool
  Send(Frame& frame_);

  virtual bool
  Send(const Address& to_, const std::string& str_);

protected:

  Socket& socket;

private:

};

//**********************************************************************
// Class: zSocket::Tap
//**********************************************************************

class Tap :
    public Adapter
{

public:

  Tap(Socket& tapper_, Socket& tappee_);

  virtual
  ~Tap();

  bool
  Inject(SHARED_PTR(zSocket::Notification) n_);

  virtual SHARED_PTR(zSocket::Notification)
  Recv() = 0;

  virtual bool
  Send(const zSocket::Address& to_, const zSocket::Buffer& sb_) = 0;

protected:

private:

  Socket& _tappee;

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
  ObserveEvent(SHARED_PTR(zEvent::Notification) n_)
  {
    bool status = false;
    if (n_.get() && (n_->GetType() == zEvent::Event::TYPE_SOCKET))
    {
      status = this->ObserveEvent(STATIC_CAST(Notification)(n_));
    }
    return (status);
  }

  virtual bool
  ObserveEvent(SHARED_PTR(zSocket::Notification) n_) = 0;

private:

};

//**********************************************************************
// Class: zSocket::Handler
//**********************************************************************

class Handler :
    public zEvent::Handler,
    public zThread::ThreadFunction,
    public zThread::ThreadArg
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
  std::map<int, Socket*> _smap;
  zThread::Thread _thread; // Thread to poll socket notification queues

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
