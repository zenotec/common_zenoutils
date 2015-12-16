//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zSocket.h
//    Description:
//
//*****************************************************************************

#ifndef __ZSOCKET_H__
#define __ZSOCKET_H__

#include <stdint.h>
#include <netinet/in.h>

#include <string>
#include <list>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>

namespace zUtils
{
namespace zSocket
{

class Handler;
class Factory;

//**********************************************************************
// zSocket::Buffer Class
//**********************************************************************

class Buffer
{

public:

  Buffer(Buffer &other_);

  Buffer(const Buffer &other_);

  Buffer(const size_t size_ = 1500);

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
// zSocket::Address Class
//**********************************************************************

class Address
{

public:

  enum TYPE
  {
    TYPE_ERR = -1, TYPE_NONE = 0, TYPE_LOOP = 1, TYPE_INET = 2, TYPE_LAST
  };

  Address(zSocket::Address::TYPE type = zSocket::Address::TYPE_NONE, const std::string &addr_ =
      std::string(""));

  Address(Address &other_);

  Address(const Address &other_);

  Address &
  operator=(Address &other_);

  Address &
  operator=(const Address &other_);

  virtual
  ~Address();

  bool
  operator ==(const zSocket::Address &other_) const;
  bool
  operator !=(const zSocket::Address &other_) const;
  bool
  operator <(const zSocket::Address &other_) const;
  bool
  operator >(const zSocket::Address &other_) const;

  Address::TYPE
  GetType() const;

  virtual std::string
  GetAddress() const;

  virtual bool
  SetAddress(const std::string &addr_);

protected:

  Address::TYPE _type;

private:

  std::string _addr;

};

//**********************************************************************
// zSocket::SocketEvent Class
//**********************************************************************

class SocketEvent : public zEvent::Event
{
public:

  enum EVENTID
  {
    EVENTID_ERR = -1,
    EVENTID_NONE = 0,
    EVENTID_PKT_RCVD = 1,
    EVENTID_PKT_SENT = 2,
    EVENTID_PKT_ERR = 3,
    EVENTID_LAST
  };

  SocketEvent(const SocketEvent::EVENTID id_);

  virtual
  ~SocketEvent();

protected:

private:

};

//**********************************************************************
// zSocket::Socket Class
//**********************************************************************

class Socket : public zEvent::EventHandler
{

public:

  Socket();

  virtual
  ~Socket();

  enum TYPE
  {
    TYPE_ERR = -1,
    TYPE_NONE = 0,
    TYPE_ETH = 1,
    TYPE_IPV4 = 2,
    TYPE_IPV6 = 3,
    TYPE_LAST
  };

  virtual const zSocket::Address *
  GetAddress();

  ssize_t
  Receive(zSocket::Address *from_, zSocket::Buffer *sb_);

  ssize_t
  Receive(zSocket::Address *from_, std::string &str_);

  ssize_t
  Send(const zSocket::Address *to_, zSocket::Buffer *sb_);

  ssize_t
  Send(const zSocket::Address *to_, const std::string &str_);

  ssize_t
  Broadcast(zSocket::Buffer *sb_);

  ssize_t
  Broadcast(const std::string &str_);

protected:

  zSocket::Socket::TYPE _type;

  zEvent::Event rx_event;
  zQueue<std::pair<zSocket::Address, zSocket::Buffer> > rxq;

  zEvent::Event tx_event;
  zQueue<std::pair<zSocket::Address, zSocket::Buffer> > txq;

private:

};

//**********************************************************************
// zSocket::Factory Class
//**********************************************************************

class Factory
{

public:

  static zSocket::Address *
  Create(const zSocket::Address::TYPE &type_);

  static zSocket::Address *
  Create(const zSocket::Address::TYPE &type_, const std::string &ifname_);

  static zSocket::Socket *
  Create(const zSocket::Socket::TYPE &type_, const zSocket::Address *addr_);

protected:

private:

};

}
}

#endif /* __ZSOCKET_H__ */
