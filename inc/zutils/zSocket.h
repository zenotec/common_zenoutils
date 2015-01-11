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

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// SocketBuffer Class
//**********************************************************************

class SocketBuffer
{

public:
  SocketBuffer(const size_t size_ = 1500);
  ~SocketBuffer();

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

protected:

private:

  SocketBuffer(SocketBuffer &other_);
  SocketBuffer(const SocketBuffer &other_);
  uint8_t *_head;
  size_t _data;
  size_t _tail;
  size_t _end;

};

//**********************************************************************
// SocketAddr Class
//**********************************************************************

class SocketAddr
{
public:
  SocketAddr(const std::string &addr_ = "0.0.0.0:0");
  SocketAddr(const struct sockaddr_in &addr_);
  ~SocketAddr();

  bool
  operator ==(const SocketAddr &other_) const;
  bool
  operator !=(const SocketAddr &other_) const;
  bool
  operator <(const SocketAddr &other_) const;
  bool
  operator >(const SocketAddr &other_) const;

  bool
  SetAddr(const std::string &addr_);
  bool
  SetAddr(const struct sockaddr_in &addr_);

  struct sockaddr_in
  GetAddr();

  bool
  SetHwAddr(const std::string &addr_);

  const uint8_t *
  GetHwAddr() const;
  std::string
  GetHwAddrStr() const;

  in_addr_t
  GetIpAddr() const;
  std::string
  GetIpAddrStr() const;

  in_port_t
  GetPort();
  std::string
  GetPortStr();

protected:

private:
  uint8_t _hwaddr[6];
  struct sockaddr_in _ipaddr;

};

//**********************************************************************
// SocketListener Class
//**********************************************************************

class SocketListener
{
public:

  virtual bool
  SocketRecv(const SocketAddr &addr_, SocketBuffer *pkt_) = 0;

protected:

private:

};

//**********************************************************************
// Socket Class
//**********************************************************************

class Socket : private zUtils::zThreadFunction
{

public:
  Socket(SocketAddr &addr_);
  virtual
  ~Socket();

  SocketAddr
  GetAddr() const;

  void
  Register(SocketListener *listener_);
  void
  Unregister(SocketListener *listener_);

  void
  Listen();

  bool
  Send(SocketAddr addr_, SocketBuffer *pkt_);

protected:
  virtual void *
  ThreadFunction(void *arg_);

private:

  void
  _notifyHandler(SocketAddr &addr_, SocketBuffer *buf_);
  std::list<SocketListener *> _handlers;

  zSem::Mutex _lock;
  SocketAddr _addr;

  void
  _create();
  void
  _destroy();
  void
  _bind(SocketAddr &addr_);
  void
  _listen();
  size_t
  _recv(SocketAddr &addr_, SocketBuffer *buf_);
  size_t
  _send(SocketAddr &addr_, SocketBuffer *buf_);

  int _sock;
  zThread *_thread;

};

}
}

#endif /* __ZSOCKET_H__ */
