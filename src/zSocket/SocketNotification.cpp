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

SocketNotification::SocketNotification(const SocketNotification::ID id_, Socket* sock_) :
    _id(id_), _sock(sock_)
{
  this->type(zEvent::Event::TYPE_SOCKET);
}

SocketNotification::~SocketNotification()
{
}

SocketNotification::ID
SocketNotification::Id() const
{
  return(this->_id);
}

zSocket::Socket*
SocketNotification::Sock()
{
  return(this->_sock);
}

}
}
