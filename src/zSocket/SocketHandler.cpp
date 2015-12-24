//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Handler.cpp
//    Description:
//
//*****************************************************************************

#include <zutils/zLog.h>
#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

Handler::Handler() :
    _sock(0), _thread(this, this)
{
  this->_lock.Unlock();
}

Handler::~Handler()
{
  this->Close();
}

bool
Handler::Register(Observer *obs_)
{
  if (!obs_)
  {
    return (false);
  }
  ZLOG_DEBUG("zSocket::Handler::Register: Registering socket observer");
  this->_lock.Lock();
  this->_obsList.push_back(obs_);
  this->_lock.Unlock();
  return (true);
}

void
Handler::Unregister(Observer *obs_)
{
  ZLOG_DEBUG("zSocket::Handler::Unregister: Unregistering socket observer");
  this->_lock.Lock();
  this->_obsList.remove(obs_);
  this->_lock.Unlock();
}

bool
Handler::Bind(Socket *sock_)
{
  bool status = false;
  if (!sock_ || this->_sock)
  {
    return (false);
  }
  if (sock_->_open() && sock_->_bind())
  {
    ZLOG_DEBUG("zSocket::Handler::Open: Binding socket");
    this->_sock = sock_;
    this->_sockList.push_front(sock_);
    this->_waitList.RegisterEvent(sock_);
    status = true;
  }
  return (status);
}

bool
Handler::Connect(Socket *sock_)
{
  bool status = false;
  if (!sock_ || this->_sock)
  {
    return (false);
  }
  if (sock_->_open() && sock_->_connect())
  {
    ZLOG_DEBUG("zSocket::Handler::Open: Connecting to socket");
    this->_sock = sock_;
    this->_sockList.push_front(sock_);
    this->_waitList.RegisterEvent(sock_);
    status = true;
  }
  return (status);
}

bool
Handler::Listen(Socket *sock_)
{
  bool status = false;
  if (!sock_)
  {
    return (false);
  }
  if (sock_->_open() && sock_->_bind())
  {
    ZLOG_DEBUG("zSocket::Handler::Open: Connecting to socket");
    this->_sockList.push_front(sock_);
    this->_waitList.RegisterEvent(sock_);
    status = true;
  }
  return (status);
}

void
Handler::Close(Socket *sock_)
{
  std::list<zSocket::Socket *>::iterator it = this->_sockList.begin();
  std::list<zSocket::Socket *>::iterator end = this->_sockList.end();
  for (; it != end;)
  {
    if (sock_ && (sock_ == (*it)))
    {
      ZLOG_DEBUG("zSocket::Handler::Close: Closing socket");
      sock_->_close();
      this->_sockList.remove(sock_);
      this->_waitList.UnregisterEvent(sock_);
      break;
    }
    else if (!sock_)
    {
      ZLOG_DEBUG("zSocket::Handler::Close: Closing socket");
      (*it)->_close();
      this->_waitList.UnregisterEvent(*it);
      it = this->_sockList.erase(it);
    }
    else
    {
      ++it;
    }
  }
  return;
}

ssize_t
Handler::Send(const zSocket::SocketAddress *to_, zSocket::SocketBuffer *sb_)
{
  ssize_t size = -1;
  if (this->_sock)
  {
    size = this->_sock->Send(to_, sb_);
  }
  return (size);
}

ssize_t
Handler::Send(const zSocket::SocketAddress *to_, const std::string &str_)
{
  ssize_t size = -1;
  if (this->_sock)
  {
    size = this->_sock->Send(to_, str_);
  }
  return (size);
}

ssize_t
Handler::Broadcast(zSocket::SocketBuffer *sb_)
{
  ssize_t size = -1;
  if (this->_sock)
  {
    size = this->_sock->Broadcast(sb_);
  }
  return (size);
}

ssize_t
Handler::Broadcast(const std::string &str_)
{
  ssize_t size = -1;
  if (this->_sock)
  {
    size = this->_sock->Broadcast(str_);
  }
  return (size);
}

bool
Handler::StartListener()
{
  ZLOG_DEBUG("zSocket::Handler::StartListener: Start listening on sockets");
  return (this->_thread.Run());
}

bool
Handler::StopListener()
{
  ZLOG_DEBUG("zSocket::Handler::StopListener: Stop listening on sockets");
  return (this->_thread.Join());
}

//*****************************************************************************
// Private methods
//*****************************************************************************

void *
Handler::ThreadFunction(void *arg_)
{
  Handler *self = (Handler *) arg_;
  std::string logstr;

  logstr = "zSocket::Handler::ThreadFunction(): Waiting for socket event....";
  ZLOG_DEBUG(logstr);

  if (self->_waitList.TimedWait(100000))
  {
    ZLOG_DEBUG("zSocket::Handler::ThreadFunction(): Received socket event....");

    zSocket::Socket *sock = static_cast<zSocket::Socket *>(self->_waitList.GetEvent());
    SocketAddress *addr = new SocketAddress;
    SocketBuffer *sb = new SocketBuffer;
    int n = sock->Receive(addr, sb);
    if (n > 0)
    {
      self->_notify(sock, addr, sb);
    }
    delete (addr);
    delete (sb);
  }
  return (0);
}

void
Handler::_notify(zSocket::Socket *sock_, zSocket::SocketAddress *addr_, zSocket::SocketBuffer *buf_)
{
  bool status = false;

  // Protect observer list
  this->_lock.Lock();

  std::list<zSocket::Observer *>::iterator it = this->_obsList.begin();
  std::list<zSocket::Observer *>::iterator end = this->_obsList.end();
  for (; it != end; ++it)
  {
    ZLOG_INFO("zSocket::Handler::_notify(): Notifying socket listener");
    (*it)->SocketRecv(sock_, addr_, buf_);
  }

  this->_lock.Unlock();

  return;
}

}
}
