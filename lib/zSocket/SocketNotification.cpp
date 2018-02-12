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
    zEvent::Notification(sock_), _subtype(Notification::SUBTYPE_NONE),
    _sa(sock_.GetType()), _da(sock_.GetType())
{
}

Notification::~Notification()
{
}

Socket&
Notification::GetSocket() const
{
  return (static_cast<Socket&>(this->GetEvent()));
}

int
Notification::GetId() const
{
  return (this->GetSocket().GetId());
}

Notification::SUBTYPE
Notification::GetSubType() const
{
  return (this->_subtype);
}

const Address&
Notification::GetSrcAddress() const
{
  return (this->_sa);
}

const Address&
Notification::GetDstAddress() const
{
  return (this->_da);
}

const Buffer&
Notification::GetBuffer() const
{
  return (this->_sb);
}

void
Notification::SetSubType(Notification::SUBTYPE subtype_)
{
  this->_subtype = subtype_;
}

void
Notification::SetSrcAddress(const Address& sa_)
{
  this->_sa = sa_;
}

void
Notification::SetDstAddress(const Address& da_)
{
  this->_da = da_;
}

void
Notification::SetBuffer(const Buffer& sb_)
{
  this->_sb = sb_;
}

}
}
