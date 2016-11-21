//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Handler.cpp
//    Description:
//
//*****************************************************************************

#include <stdint.h>
#include <netinet/in.h>
#include <string.h>

#include <string>
#include <list>
#include <mutex>
#include <memory>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>

#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

//*****************************************************************************
// zSocket::SocketNotification Class
//*****************************************************************************

SocketNotification::SocketNotification(Socket* sock_) :
    _id(SocketNotification::ID_NONE), zEvent::EventNotification(sock_)
{
}

SocketNotification::~SocketNotification()
{
}

SocketNotification::ID
SocketNotification::Id() const
{
  return(this->_id);
}

void
SocketNotification::id(SocketNotification::ID id_)
{
  this->_id = id_;
  return;
}

zSocket::Socket*
SocketNotification::Sock()
{
  return(static_cast<zSocket::Socket*>(this->GetEvent()));
}

SocketAddressBufferPair
SocketNotification::Pkt() const
{
  return(this->_pkt);
}

void
SocketNotification::pkt(SocketAddressBufferPair &pkt_)
{
  this->_pkt = pkt_;
  return;
}

}
}
