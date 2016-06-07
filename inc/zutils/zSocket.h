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

typedef std::pair<SocketAddress, std::shared_ptr<SocketBuffer> > SocketAddressBufferPair;
typedef zQueue <SocketAddressBufferPair> SocketBufferQueue;

//**********************************************************************
// zSocket::Socket Class
//**********************************************************************

class Socket : public zEvent::Event
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
  Receive(SocketAddressBufferPair &pair_);

  ssize_t
  Receive(zSocket::SocketAddress &from_, zSocket::SocketBuffer &sb_);

  ssize_t
  Receive(zSocket::SocketAddress &from_, std::string &str_);

  ssize_t
  Send(SocketAddressBufferPair &pair_);

  ssize_t
  Send(const zSocket::SocketAddress &to_, zSocket::SocketBuffer &sb_);

  ssize_t
  Send(const zSocket::SocketAddress &to_, const std::string &str_);

protected:

  zSocket::Socket::TYPE _type;

  // Called to process a received packet
  bool
  rxbuf(SocketAddressBufferPair &pair_);

  // Called to get packet to send
  bool
  txbuf(SocketAddressBufferPair &pair_, size_t timeout_ = 1000000 /* 1 sec */);

private:

  zSocket::SocketAddress _addr;

  SocketBufferQueue _rxq;
  SocketBufferQueue _txq;

};

//**********************************************************************
// zSocket::SocketNotification Class
//**********************************************************************

class SocketNotification : public zEvent::EventNotification
{
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

  SocketNotification(const SocketNotification::ID id_, Socket* sock_);

  virtual
  ~SocketNotification();

  SocketNotification::ID
  Id() const;

  zSocket::Socket*
  Sock();

protected:

private:

  SocketNotification::ID _id;
  Socket* _sock;

};

//**********************************************************************
// Class: SocketHandler
//**********************************************************************

class SocketHandler : public zEvent::EventHandler
{
public:

  SocketHandler();

  virtual
  ~SocketHandler();

  bool
  Add(Socket *sock_);

  bool
  Remove(Socket *sock_);

protected:

private:

  std::list<Socket *> _sock_list;

};

//**********************************************************************
// Class: SocketManager
//**********************************************************************

class SocketManager : public SocketHandler
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
