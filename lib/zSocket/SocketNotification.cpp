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

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

//*****************************************************************************
// zSocket::SocketNotification Class
//*****************************************************************************

Notification::Notification(Socket& sock_) :
    zEvent::Notification(sock_), _id(Notification::ID_NONE)
{
}

Notification::~Notification()
{
}

Notification::ID
Notification::Id() const
{
  return (this->_id);
}

void
Notification::id(Notification::ID id_)
{
  this->_id = id_;
  return;
}

zSocket::Socket&
Notification::Sock()
{
  return (static_cast<zSocket::Socket&>(this->GetEvent()));
}

AddressBufferPair
Notification::Pkt() const
{
  return (this->_pkt);
}

void
Notification::pkt(AddressBufferPair &pkt_)
{
  this->_pkt = pkt_;
  return;
}

}
}
