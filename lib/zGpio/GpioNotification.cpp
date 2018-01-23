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

#include <sys/time.h>
#include <sys/types.h>
#include <sys/poll.h>

#include <string>
#include <list>
#include <mutex>
#include <sstream>
#include <fstream>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>
#include <zutils/zSwitch.h>

#include <zutils/zGpio.h>

namespace zUtils
{
namespace zGpio
{

//**********************************************************************
// Class: Notification
//**********************************************************************

GpioNotification::GpioNotification(zGpio::GpioPort::STATE state_, zGpio::GpioPort* port_) :
    _state(state_), _port(port_)
{
  this->setType(zEvent::Event::TYPE_GPIO);
}

GpioNotification::~GpioNotification()
{
  this->setType(zEvent::Event::TYPE_ERR);
  this->_state = GpioPort::STATE_ERR;
}

zGpio::GpioPort::STATE
GpioNotification::State()
{
  return (this->_state);
}

zGpio::GpioPort*
GpioNotification::Port()
{
  return (this->_port);
}

}
}
