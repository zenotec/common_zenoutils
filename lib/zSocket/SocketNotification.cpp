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
// Class: zSocket::Notification
//*****************************************************************************

Notification::Notification(Socket& sock_) :
    zEvent::Notification(sock_),
    _subtype(Notification::SUBTYPE_NONE)
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

bool
Notification::SetSubType(const Notification::SUBTYPE subtype_)
{
  bool status = false;
  switch (subtype_)
  {
  case Notification::SUBTYPE_NONE ... Notification::SUBTYPE_LAST:
    this->_subtype = subtype_;
    status = true;
    break;
  default:
    break;
  }
  return (status);
}

SHARED_PTR(Address)
Notification::GetSrcAddress() const
{
  return (this->_sa);
}

bool
Notification::SetSrcAddress(SHARED_PTR(Address) sa_)
{
  this->_sa = sa_;
  return (true);
}

SHARED_PTR(Address)
Notification::GetDstAddress() const
{
  return (this->_da);
}

bool
Notification::SetDstAddress(SHARED_PTR(Address) da_)
{
  this->_da = da_;
  return (true);
}

SHARED_PTR(Buffer)
Notification::GetBuffer() const
{
  return (this->_sb);
}

bool
Notification::SetBuffer(SHARED_PTR(Buffer) sb_)
{
  this->_sb = sb_;
  return (true);
}

SHARED_PTR(Frame)
Notification::GetFrame() const
{
  return (this->_frame);
}

bool
Notification::SetFrame(SHARED_PTR(Frame) frame_)
{
  this->_frame = frame_;
  return (true);
}

}
}
