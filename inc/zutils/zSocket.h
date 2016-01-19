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
class SocketFactory;

//**********************************************************************
// zSocket::SocketBuffer Class
//**********************************************************************

class SocketBuffer
{

public:

  SocketBuffer(SocketBuffer &other_);

  SocketBuffer(const SocketBuffer &other_);

  SocketBuffer(const size_t size_ = 1500);

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
// zSocket::SocketAddress Class
//**********************************************************************

class SocketAddress
{

public:

  enum TYPE
  {
    TYPE_ERR = -1,
    TYPE_NONE = 0,
    TYPE_LOOP = 1,
    TYPE_INET = 2,
    TYPE_MCAST = 3,
    TYPE_BCAST = 4,
    TYPE_LAST
  };

  SocketAddress(zSocket::SocketAddress::TYPE type = zSocket::SocketAddress::TYPE_NONE,
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
  operator ==(const zSocket::SocketAddress &other_) const;
  bool
  operator !=(const zSocket::SocketAddress &other_) const;
  bool
  operator <(const zSocket::SocketAddress &other_) const;
  bool
  operator >(const zSocket::SocketAddress &other_) const;

  SocketAddress::TYPE
  GetType() const;

  bool
  SetType(const SocketAddress::TYPE &type_);

  virtual std::string
  GetAddress() const;

  virtual bool
  SetAddress(const std::string &addr_);

protected:

  SocketAddress::TYPE _type;

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
// zSocket::SocketObserver Class
//**********************************************************************

class SocketObserver : public zEvent::EventObserver
{
public:
  SocketObserver();

  virtual
  ~SocketObserver();
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

  Socket::TYPE
  GetType();

  bool
  SetType(const Socket::TYPE type_);

  virtual const zSocket::SocketAddress &
  GetAddress();

  virtual bool
  SetAddress(const zSocket::SocketAddress &addr_);

  virtual bool
  Open();

  virtual void
  Close();

  virtual bool
  Bind();

  virtual bool
  Connect(const zSocket::SocketAddress &addr_);

  ssize_t
  Receive(zSocket::SocketAddress &from_, zSocket::SocketBuffer &sb_);

  ssize_t
  Receive(zSocket::SocketAddress &from_, std::string &str_);

  ssize_t
  Send(const zSocket::SocketAddress &to_, zSocket::SocketBuffer &sb_);

  ssize_t
  Send(const zSocket::SocketAddress &to_, const std::string &str_);

protected:

  zSocket::Socket::TYPE _type;

  zSocket::SocketEvent rx_event;
  zQueue<std::pair<zSocket::SocketAddress, zSocket::SocketBuffer> > rxq;

  zSocket::SocketEvent tx_event;
  zQueue<std::pair<zSocket::SocketAddress, zSocket::SocketBuffer> > txq;

  zSocket::SocketEvent err_event;
  zQueue<std::pair<zSocket::SocketAddress, zSocket::SocketBuffer> > errq;

private:

  zSocket::SocketAddress _addr;

};

}
}

#endif /* __ZSOCKET_H__ */
