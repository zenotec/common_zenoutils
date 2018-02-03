/*
 * Copyright (c) 2014-2018 ZenoTec LLC (http://www.zenotec.net)
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

#include <zutils/zLog.h>
#include <zutils/zSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

//*****************************************************************************
// Class: zSocket::Adapter
//*****************************************************************************

Adapter::Adapter(Socket& socket_) :
    zEvent::Adapter(socket_), _socket(socket_)
{
}

Adapter::~Adapter()
{
}

SHARED_PTR(zEvent::Notification)
Adapter::AdaptEvent(SHARED_PTR(zEvent::Notification) noti_)
{
  if (noti_ && (noti_->GetType() == zEvent::Event::TYPE_SOCKET))
  {
    noti_ = this->AdaptEvent(STATIC_CAST(Notification)(noti_));
  }
  return (noti_);
}

}
}
