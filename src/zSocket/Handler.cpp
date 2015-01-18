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
    _thread(this, this)
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
  ZLOG_DEBUG("Handler::Register: Registering observer");
  this->_lock.Lock();
  this->_obsList.push_back(obs_);
  this->_lock.Unlock();
  return (true);
}

void
Handler::Unregister(Observer *obs_)
{
  ZLOG_DEBUG("Handler::Unregister: Unregistering observer");
  this->_lock.Lock();
  this->_obsList.remove(obs_);
  this->_lock.Unlock();
}

bool
Handler::Bind(Socket *sock_)
{
  bool status = false;
  if (!sock_)
  {
    return (false);
  }
  if (sock_->_open() && sock_->_bind())
  {
    ZLOG_DEBUG("Handler::Open: Opening socket");
    this->_sockList.push_back(sock_);
    this->_waitList.Register(sock_);
    status = true;
  }
  return (status);
}

bool
Handler::Connect(Socket *sock_)
{
  bool status = false;
  if (!sock_)
  {
    return (false);
  }
  if (sock_->_open() && sock_->_connect())
  {
    ZLOG_DEBUG("Handler::Open: Opening socket");
    this->_sockList.push_back(sock_);
    this->_waitList.Register(sock_);
    status = true;
  }
  return (status);
}

void
Handler::Close(Socket *sock_)
{
  if (sock_)
  {
    ZLOG_DEBUG("Handler::Close: Closing socket");
    sock_->_close();
    this->_waitList.Unregister(sock_);
    this->_sockList.remove(sock_);
  }
  return;
}

bool
Handler::StartListener(uint32_t msecs_)
{
  ZLOG_DEBUG("Handler::StartListener: Start listening on sockets");
  return (this->_thread.Run(msecs_));
}

bool
Handler::StopListener(uint32_t msecs_)
{
  ZLOG_DEBUG("Handler::StopListener: Stop listening on sockets");
  return (this->_thread.Join(msecs_));
}

//*****************************************************************************
// Private methods
//*****************************************************************************

void *
Handler::ThreadFunction(void *arg_)
{
  Handler *self = (Handler *) arg_;
  std::string logstr;

  logstr = "Handler::ThreadFunction(): Waiting for socket event....";
  ZLOG_DEBUG(logstr);

  if (self->_waitList.Wait(1000))
  {
    logstr = "Handler::ThreadFunction(): Received socket event....";
    ZLOG_DEBUG(logstr);

    std::list<Socket *>::iterator it = self->_sockList.begin();
    std::list<Socket *>::iterator end = self->_sockList.end();
    for (; it != end; ++it)
    {
      Address addr;
      Buffer *sb = new Buffer;
      int n = (*it)->RecvBuffer(addr, *sb);
      if (n > 0)
      {
        self->_notify((*it), addr, *sb);
      }
      delete (sb);
    }
  }
  return (0);
}

void
Handler::_notify(zSocket::Socket *sock_, zSocket::Address &addr_, zSocket::Buffer &buf_)
{
  bool status = false;

  // Protect observer list
  this->_lock.Lock();

  std::list<zSocket::Observer *>::iterator it = this->_obsList.begin();
  std::list<zSocket::Observer *>::iterator end = this->_obsList.end();
  for (; it != end; ++it)
  {
    ZLOG_INFO("Handler::_notify(): Notifying socket listener");
    (*it)->SocketRecv(sock_, addr_, buf_);
  }

  this->_lock.Unlock();

  return;
}

}
}
