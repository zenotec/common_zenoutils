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

#include <list>
#include <mutex>

#include <zutils/zQueue.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>
#include <zutils/zSerial.h>

namespace zUtils
{
namespace zSerial
{

//**********************************************************************
// Class: SerialNotification
//**********************************************************************

SerialNotification::SerialNotification(SerialPort* port_) :
    _id(SerialNotification::ID_NONE), zEvent::EventNotification(port_)
{
}

SerialNotification::~SerialNotification()
{
}

SerialNotification::ID
SerialNotification::Id() const
{
  return (this->_id);
}

void
SerialNotification::id(SerialNotification::ID id_)
{
  this->_id = id_;
  return;
}

SerialPort*
SerialNotification::Port()
{
  return (static_cast<SerialPort*>(this->GetEvent()));
}

char
SerialNotification::Data() const
{
  return (this->_c);
}

bool
SerialNotification::data(const char c_)
{
  this->_c = c_;
  return (true);
}

}
}
