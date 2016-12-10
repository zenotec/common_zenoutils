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

//*****************************************************************************
// Handler Class
//*****************************************************************************
GpioHandler::GpioHandler()
{
}

GpioHandler::~GpioHandler()
{
  std::list<GpioPort*>::iterator it = this->_port_list.begin();
  std::list<GpioPort*>::iterator end = this->_port_list.end();
  for (; it != end; ++it)
  {
    this->UnregisterEvent(*it);
    (*it)->Close();
  }
  this->_port_list.clear();
}

bool
GpioHandler::Add(GpioPort* port_)
{
  bool status = false;
  if (port_ && (port_->Open() >= 0))
  {
    this->_port_list.push_back(port_);
    this->_port_list.unique();
    this->RegisterEvent(port_);
    status = true;
  }
  return (status);
}

bool
GpioHandler::Remove(GpioPort* port_)
{
  bool status = false;

  if (port_ && port_->Close())
  {
    this->_port_list.remove(port_);
    this->UnregisterEvent(port_);
    status = true;
  }
  return (status);
}

GpioPort::STATE
GpioHandler::Get()
{
  GpioPort::STATE state = GpioPort::STATE_NONE;
  std::list<GpioPort*>::iterator it = this->_port_list.begin();
  std::list<GpioPort*>::iterator end = this->_port_list.end();
  for (; it != end; ++it)
  {
    if ((state != GpioPort::STATE_NONE) && (state != (*it)->Get()))
    {
      return (GpioPort::STATE_ERR);
    }
    state = (*it)->Get();
  }
  return (state);
}

bool
GpioHandler::Set(GpioPort::STATE state_)
{
  GpioPort::STATE state = GpioPort::STATE_NONE;
  std::list<GpioPort*>::iterator it = this->_port_list.begin();
  std::list<GpioPort*>::iterator end = this->_port_list.end();
  for (; it != end; ++it)
  {
    if (!(*it)->Set(state_) || (state_ != (*it)->Get()))
    {
      return (false);
    }
  }
  return (true);
}

}
}
