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

//**********************************************************************
// Class: SocketHandler
//**********************************************************************

SocketHandler::SocketHandler()
{
}

SocketHandler::~SocketHandler()
{
  std::list<Socket*>::iterator it = this->_sock_list.begin();
  std::list<Socket*>::iterator end = this->_sock_list.end();
  for (; it != end; ++it)
  {
    this->UnregisterEvent(*it);
    (*it)->Close();
  }
  this->_sock_list.clear();
}

bool
SocketHandler::Add(Socket *sock_)
{
  bool status = false;
  if (sock_ && (sock_->Open()))
  {
    this->RegisterEvent(sock_);
    this->_sock_list.push_back(sock_);
    this->_sock_list.unique();
    status = true;
  }
  return (status);
}

bool
SocketHandler::Remove(Socket *sock_)
{
  bool status = false;
  if (sock_)
  {
    this->UnregisterEvent(sock_);
    this->_sock_list.remove(sock_);
    sock_->Close();
    status = true;
  }
  return (status);
}


}
}
