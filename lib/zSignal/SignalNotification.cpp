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

#include <signal.h>

#include <mutex>
#include <list>
#include <map>

#include <zutils/zEvent.h>

#include <zutils/zSignal.h>

namespace zUtils
{
namespace zSignal
{

//**********************************************************************
// Class: SignalNotification
//**********************************************************************
SignalNotification::SignalNotification(Signal& signal_) :
    zEvent::Notification(signal_), _id(signal_.Id()), _info(NULL),
    _cnt(signal_.Count())
{
}

SignalNotification::~SignalNotification()
{
}

Signal::ID
SignalNotification::Id() const
{
  return (this->_id);
}

const siginfo_t*
SignalNotification::SigInfo() const
{
  return (this->_info);
}

void
SignalNotification::siginfo(const siginfo_t *info_)
{
  this->_info = info_;
  return;
}

}
}
