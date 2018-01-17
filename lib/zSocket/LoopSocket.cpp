/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

#include <zutils/zLoopSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// LoopAddress Class
//**********************************************************************

LoopAddress::LoopAddress() :
    SocketAddress(SocketType::TYPE_LOOP)
{
}

LoopAddress::~LoopAddress()
{
}

bool
LoopAddress::verify(const SocketType type_, const std::string &addr_)
{
  return ((type_ == SocketType::TYPE_LOOP) && addr_.empty());
}

//**********************************************************************
// zSocket::LoopSocket Class
//**********************************************************************

LoopSocket::LoopSocket() :
    Socket(SocketType::TYPE_LOOP), _thread(this, NULL),
        _opened(false), _bound(false), _connected(false)
{
  return;
}

LoopSocket::~LoopSocket()
{
  this->Close();
  return;
}

bool
LoopSocket::Open()
{
  bool status = false;

  if (!this->_opened)
  {
    this->_opened = true;
    status = true;
  }
  return (status);
}

void
LoopSocket::Close()
{
  // Close socket
  if (this->_thread.Stop())
  {
    this->_opened = false;
    ZLOG_DEBUG("Socket Closed");
  } // end if

  return;
}

bool
LoopSocket::_bind()
{
  bool status = false;

  if (this->Address().Type() != SocketType::TYPE_LOOP)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  if (this->_opened && !this->_bound && !this->_connected)
  {
    // Start listener threads
    if (this->_thread.Start())
    {
      this->_bound = true;
      status = true;
    }
    else
    {
      ZLOG_ERR("Error starting listening threads");
    }
  }

  return (status);
}

//bool
//LoopSocket::Connect(const SocketAddress& addr_)
//{
//  bool status = false;
//
//  if (this->Address().Type() != SocketType::TYPE_LOOP)
//  {
//    ZLOG_CRIT(std::string("Invalid socket address"));
//    return (false);
//  }
//
//  if (addr_.Type() != SocketType::TYPE_LOOP)
//  {
//    ZLOG_ERR(std::string("Invalid socket address type"));
//    return (false);
//  }
//
//  if (this->_opened && !this->_bound && !this->_connected)
//  {
//    // Start listener threads
//    if (this->_thread.Start())
//    {
//      this->_connected = true;
//      status = true;
//    }
//    else
//    {
//      ZLOG_ERR("Error starting listening threads");
//    }
//  }
//
//  return (status);
//}

void
LoopSocket::Run(zThread::ThreadArg *arg_)
{

  SocketAddressBufferPair p;

  while (!this->Exit())
  {

    if (this->txbuf(p, 100))
    {
      ZLOG_DEBUG("Sending packet: " + p.first->Address() +
          "(" + zLog::IntStr(p.second->Size()) + ")");
      if (!this->rxbuf(p))
      {
        ZLOG_ERR("Error sending packet");
      }
    }

  }

  return;

}

}
}
