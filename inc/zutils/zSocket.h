//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zSocket.h
//    Description:
//
//*****************************************************************************

#ifndef __ZSOCKET_H__
#define __ZSOCKET_H__

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
// zSocket::SocketBuffer Class
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
// zSocket::SocketAddress Class
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

  virtual bool
  verify(const SocketType type_, const std::string &addr_);

private:

  SocketType _type;
  std::string _addr;

};

typedef std::pair<std::shared_ptr<const SocketAddress>, std::shared_ptr<SocketBuffer> > SocketAddressBufferPair;
typedef zQueue<SocketAddressBufferPair> SocketBufferQueue;

//**********************************************************************
// zSocket::Socket Class
//**********************************************************************

class Socket : public zEvent::Event
{

public:

  Socket(SocketType type_ = SocketType::TYPE_NONE);

  virtual
  ~Socket();

  const SocketType
  Type() const;

  const SocketAddress*
  Address() const;

  bool
  Address(const SocketAddress *addr_);

  virtual bool
  Open() = 0;

  virtual void
  Close() = 0;

  virtual bool
  Bind() = 0;

  virtual bool
  Connect(const SocketAddress* addr_) = 0;

  ssize_t
  Send(SocketAddressBufferPair &pair_);

  ssize_t
  Send(const SocketAddress &to_, SocketBuffer &sb_);

  ssize_t
  Send(const SocketAddress &to_, const std::string &str_);

protected:

  // Called by derived class to process a received packet
  bool
  rxbuf(SocketAddressBufferPair &pair_);

  // Called by derived class to get packet to send
  bool
  txbuf(SocketAddressBufferPair &pair_, size_t timeout_ = 1000000 /* usec */);

private:

  const SocketType _type;
  const SocketAddress* _src;
  const SocketAddress* _dst;
  SocketBufferQueue _txq;

  Socket(Socket &other_);

  Socket(const Socket &other_);

  Socket &
  operator=(Socket &other_);

  Socket &
  operator=(const Socket &other_);

};

//**********************************************************************
// zSocket::SocketNotification Class
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
// Class: SocketManager
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
