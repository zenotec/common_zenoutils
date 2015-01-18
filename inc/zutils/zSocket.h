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

namespace zUtils
{
namespace zSocket
{

class Handler;

//**********************************************************************
// zSocket::Buffer Class
//**********************************************************************

class Buffer
{

public:
  Buffer(Buffer &other_);
    Buffer(const size_t size_ = 1500);
    ~Buffer();

  Buffer &
  operator=(Buffer &other_);
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

  Buffer(const Buffer &other_);
  Buffer &
  operator=(const Buffer &other_);

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
    TYPE_ERR = -1,
    TYPE_NONE = 0,
    TYPE_LOOP = 1,
    TYPE_INET = 2,
    TYPE_LAST
  };

  Address(zSocket::Address::TYPE type = zSocket::Address::TYPE_NONE,
      const std::string &addr_ = std::string(""));
    ~Address();

  bool
  operator ==(const zSocket::Address &other_) const;
  bool
  operator !=(const zSocket::Address &other_) const;

  Address::TYPE
  GetType() const;
  bool
  SetType(const zSocket::Address::TYPE &type_);

  std::string
  GetAddr() const;
  bool
  SetAddr(const std::string &addr_);

protected:

private:

  Address::TYPE _type;
  std::string _addr;

};

//**********************************************************************
// zSocket::Socket Class
//**********************************************************************

class Socket : public zQueue<std::pair<zSocket::Address, zSocket::Buffer *> >
{
  friend class Handler;

public:

  ssize_t
  RecvBuffer(zSocket::Address &addr_, zSocket::Buffer &sb_);
  ssize_t
  RecvString(zSocket::Address &addr_, std::string &str_);

  ssize_t
  SendBuffer(const zSocket::Address &addr_, zSocket::Buffer &sb_);
  ssize_t
  SendString(const zSocket::Address &addr_, const std::string &str_);

protected:

  virtual bool
  _open() = 0;

  virtual void
  _close() = 0;

  virtual bool
  _bind() = 0;

  virtual bool
  _connect() = 0;

  virtual ssize_t
  _send(const zSocket::Address &addr_, zSocket::Buffer &sb_) = 0;

private:

};

//**********************************************************************
// zSocket::Observer Class
//**********************************************************************

class Observer
{
public:
  virtual bool
  SocketRecv(zSocket::Socket *sock_, const zSocket::Address &addr_, zSocket::Buffer &sb_) = 0;
};

//**********************************************************************
// zSocket::Handler Class
//**********************************************************************

class Handler : private zThread::Function
{

public:

  Handler();
  virtual
  ~Handler();

  bool
  Register(zSocket::Observer *obs_);
  void
  Unregister(zSocket::Observer *obs_);

  bool
  Bind(zSocket::Socket *sock_);
  bool
  Connect(zSocket::Socket *sock_);

  void
  Close(zSocket::Socket *sock_ = NULL);

  bool
  StartListener(uint32_t msecs_ = 0);
  bool
  StopListener(uint32_t msecs_ = 0);

protected:

  virtual void *
  ThreadFunction(void *arg_);

private:

  void
  _notify(zSocket::Socket *sock_, zSocket::Address &addr_, zSocket::Buffer &buf_);

  std::list<zSocket::Observer *> _obsList;
  zEvent::EventList _waitList;
  std::list<zSocket::Socket *> _sockList;

  zSem::Mutex _lock;
  zThread::Thread _thread;

};

}
}

#endif /* __ZSOCKET_H__ */
