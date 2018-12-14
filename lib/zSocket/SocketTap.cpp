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
// Class: zSocket::Tap
//*****************************************************************************

Tap::Tap(Socket& tapper_, Socket& tappee_) :
    Adapter(tapper_), _tappee(tappee_)
{
}

Tap::~Tap()
{
}

bool
Tap::Inject(SHPTR(zEvent::Notification)& n_)
{
  zEvent::STATUS status = this->_tappee.notifyHandlers(n_);
  return (status == zEvent::STATUS_OK);
}

}
}
