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
// Buffer Class
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
// Address Class
//**********************************************************************

class Address
{
public:

  enum TYPE
  {
    TYPE_ERR = -1, TYPE_NONE = 0, TYPE_INET = 1, TYPE_LAST
  };

  Address(Address::TYPE type = Address::TYPE_NONE, const std::string &addr_ = std::string(""));
  ~Address();

  bool
  operator ==(const Address &other_) const;
  bool
  operator !=(const Address &other_) const;

  Address::TYPE
  GetType() const;
  bool
  SetType(const Address::TYPE &type_);

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
// Observer Class
//**********************************************************************

class Observer
{
public:
  virtual bool
  SocketRecv(const Address &addr_, Buffer *buf_) = 0;

protected:
private:
};

//**********************************************************************
// Socket Class
//**********************************************************************

class Socket : public zQueue<std::pair<Address, Buffer *> >
{
  friend class Handler;

public:

  ssize_t
  RecvBuffer(Address &addr_, Buffer &sb_);
  ssize_t
  RecvString(Address &addr_, std::string &str_);

  ssize_t
  SendBuffer(const Address &addr_, Buffer &sb_);
  ssize_t
  SendString(const Address &addr_, const std::string &str_);

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
  _send(const Address &addr_, Buffer &sb_) = 0;

private:

};

//**********************************************************************
// Handler Class
//**********************************************************************

class Handler : private zThread::Function
{

public:

  Handler();
  virtual
  ~Handler();

  bool
  Register(Observer *obs_);
  void
  Unregister(Observer *obs_);

  bool
  Bind(Socket *sock_);
  bool
  Connect(Socket *sock_);

  void
  Close(Socket *sock_ = NULL);

  bool
  StartListener(uint32_t usecs_);
  bool
  StopListener();

protected:

  virtual void *
  ThreadFunction(void *arg_);

private:

  void
  _notify(Socket *sock_, Address &addr_, Buffer *buf_);

  std::list<Observer *> _obsList;
  zEvent::EventList _waitList;
  std::list<Socket *> _sockList;

  zSem::Mutex _lock;
  zThread::Thread _thread;

};

}
}

#endif /* __ZSOCKET_H__ */
