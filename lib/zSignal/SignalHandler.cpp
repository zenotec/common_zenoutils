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

#include <string.h>
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
// Class: Handler
//**********************************************************************

Handler::Handler()
{
  for (int i = 0; i < Signal::ID_LAST; i++)
  {
    this->_sigs[i] = new Signal(Signal::ID(i));
  }
}

Handler::~Handler()
{
  for (int i = 0; i < Signal::ID_LAST; i++)
  {
    if (this->_sigs[i])
    {
      delete (this->_sigs[i]);
      this->_sigs[i] = NULL;
    }
  }
}

bool
Handler::RegisterObserver(Signal::ID id_, zEvent::Observer *obs_)
{
  bool status = false;
  if ((id_ > Signal::ID_ERR) && (id_ < Signal::ID_LAST) && this->_sigs[id_])
  {
    status = this->_sigs[id_]->RegisterObserver(obs_);
  }
  return (status);
}

bool
Handler::UnregisterObserver(Signal::ID id_, zEvent::Observer *obs_)
{
  bool status = false;
  if ((id_ > Signal::ID_ERR) && (id_ < Signal::ID_LAST) && this->_sigs[id_])
  {
    status = this->_sigs[id_]->UnregisterObserver(obs_);
  }
  return (status);
}

zEvent::STATUS
Handler::Notify(Signal::ID id_, siginfo_t *info_)
{
  zEvent::STATUS status = zEvent::STATUS_ERR;
  if ((id_ > Signal::ID_ERR) && (id_ < Signal::ID_LAST) && this->_sigs[id_])
  {
    status = this->_sigs[id_]->Notify(info_);
  }
  return (status);
}

}
}

