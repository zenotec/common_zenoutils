//*****************************************************************************
//
//
//
//*****************************************************************************

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>

#include "zutils/zLog.h"
#include "zutils/zSocket.h"

namespace zUtils
{
namespace zSocket
{

Socket::Socket(SocketAddr &addr_) :
    _addr(addr_), _sock(0), _thread(0)
{
  this->_create();
  this->_bind(addr_);
  this->_lock.Unlock();

}

Socket::~Socket()
{
  this->_lock.Lock();
  this->_destroy();
}

SocketAddr
Socket::GetAddr() const
{
  return (this->_addr);
}

void
Socket::Listen()
{
  this->_listen();
}

void
Socket::Register(SocketListener *listener_)
{
  this->_lock.Lock();
  this->_listenerTable.push_back(listener_);
  this->_lock.Unlock();
}

void
Socket::Unregister(SocketListener *listener_)
{
  this->_lock.Lock();
  this->_listenerTable.remove(listener_);
  this->_lock.Unlock();
}

bool
Socket::Send(SocketAddr addr_, SocketBuffer *buf_)
{
  std::string logstr;
  logstr += "Sending on socket:\t";
  logstr += "To: " + addr_.GetIpAddrStr() + ":" + addr_.GetPortStr() + ";\t";
  logstr += "From: " + this->_addr.GetIpAddrStr() + ":" + this->_addr.GetPortStr() + ";\t";
  logstr += "Size: " + zLog::IntStr(buf_->Size()) + ";";
  ZLOG_INFO(logstr);
  return (this->_send(addr_, buf_) == buf_->Length());
}

//*****************************************************************************
// Private methods
//*****************************************************************************

void
Socket::_create()
{
  // Create a inet socket
  if (!this->_sock)
  {
    this->_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (this->_sock < 0)
    {
      ZLOG_CRIT("Cannot create socket: " + std::string(strerror(errno)));
      throw;
    } // end if
  } // end if
}

void
Socket::_destroy()
{
  // Close socket
  if (this->_sock)
  {
    close(this->_sock);
    this->_sock = 0;
  } // end if
  if (this->_thread)
  {
    delete (this->_thread);
    this->_thread = 0;
  } // end if
}

void
Socket::_bind(SocketAddr &addr_)
{
  struct sockaddr_in addr = addr_.GetAddr();
  int ret = bind(this->_sock, (struct sockaddr*) &addr, sizeof(addr));
  if (ret < 0)
  {
    ZLOG_CRIT("Cannot bind socket: " + std::string(strerror(errno)));
    throw;
  } // end if
}

size_t
Socket::_recv(SocketAddr &addr_, SocketBuffer *buf_)
{
  struct sockaddr_in src = { 0 };
  socklen_t len = sizeof(src);
  int n = recvfrom(this->_sock, buf_->Head(), buf_->TotalSize(), 0, (struct sockaddr *) &src, &len);
  if (n > 0)
  {
    addr_.SetAddr(src);
    buf_->Put(n);
  } // end if
  return (n);
}

size_t
Socket::_send(SocketAddr &addr_, SocketBuffer *buf_)
{
  struct sockaddr_in src = addr_.GetAddr();
  socklen_t slen = sizeof(src);
  int n = sendto(this->_sock, buf_->Head(), buf_->Size(), 0, (struct sockaddr *) &src, slen);
  return (n);
}

void
Socket::_listen()
{
  // Create socket listening thread
  this->_thread = new zUtils::zThread(this, this);
}

void *
Socket::Function(void *arg_)
{
  Socket *self = (Socket *) arg_;

  fd_set rxFds;
  struct timeval to;

  // Setup for select loop
  FD_ZERO(&rxFds);
  FD_SET(self->_sock, &rxFds);
  to.tv_sec = 0;
  to.tv_usec = 100000;

  // Select on socket
  int ret = select(self->_sock + 1, &rxFds, NULL, NULL, &to);

  if (ret > 0)
  {
    // Received message
    SocketAddr addr;
    SocketBuffer *sb = new SocketBuffer; // Listener is responsible for deleting
    int n = self->_recv(addr, sb);
    if (n > 0)
    {
      std::string logstr;
      logstr += "Receiving on socket:\t";
      logstr += "To: " + self->_addr.GetIpAddrStr() + ":" + self->_addr.GetPortStr() + ";\t";
      logstr += "From: " + addr.GetIpAddrStr() + ":" + addr.GetPortStr() + ";\t";
      logstr += "Size: " + zLog::IntStr(n) + ";";
      ZLOG_INFO(logstr);
      self->_notifyListeners(addr, sb);
    } // end if
    else
    {
      std::string logstr;
      logstr += "Error receiving packet: \n";
      logstr += "Socket: " + self->_addr.GetIpAddrStr() + ":" + self->_addr.GetPortStr();
      logstr += "  Status: " + zLog::IntStr(n);
      ZLOG_WARN(logstr);
      delete (sb);
    } //  end else
  } // end if

  if (ret < 0)
  {
    ZLOG_ERR("Cannot select on socket fd: " + std::string(strerror(errno)));
  } // end if

  return (0);

}

void
Socket::_notifyListeners(SocketAddr &addr_, SocketBuffer *buf_)
{

  this->_lock.Lock();
  if (!this->_listenerTable.empty())
  {
    SocketListener *listener = this->_listenerTable.front();
    this->_listenerTable.pop_front();
    this->_listenerTable.push_back(listener);
    ZLOG_INFO("Notifying socket listener");
    listener->Recv(addr_, buf_);
  } // end if
  else
  {
    // Since no listeners are register, delete socket buffer
    delete (buf_);
  } // end else
  this->_lock.Unlock();

  return;
}

}
}
