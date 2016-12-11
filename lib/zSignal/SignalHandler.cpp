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

#include <zutils/zLog.h>
#include <zutils/zEvent.h>

#include <zutils/zSignal.h>

namespace zUtils
{
namespace zSignal
{

//**********************************************************************
// Class: SignalHandler
//**********************************************************************

SignalHandler::SignalHandler()
{
}

SignalHandler::~SignalHandler()
{
  std::map<Signal::ID, Signal*>::iterator it = this->_sigs.begin();
  std::map<Signal::ID, Signal*>::iterator end = this->_sigs.end();
  for (; it != end; ++it)
  {
    delete(it->second);
  }
  this->_sig_handlers.clear();
  this->_sigs.clear();
}

bool
SignalHandler::RegisterObserver(Signal::ID id_, zEvent::EventObserver *obs_)
{
  ZLOG_DEBUG("Registering signal observer: " + ZLOG_INT(id_));
  std::map<Signal::ID, Signal*>::iterator it = this->_sigs.find(id_);
  if (it == this->_sigs.end())
  {
    ZLOG_DEBUG("Registering signal event: " + ZLOG_INT(id_));
    this->_sigs[id_] = new Signal(id_);
    this->_sig_handlers[id_].RegisterEvent(this->_sigs[id_]);
  }
  return (this->_sig_handlers[id_].RegisterObserver(obs_));
}

bool
SignalHandler::UnregisterObserver(Signal::ID id_, zEvent::EventObserver *obs_)
{
  ZLOG_DEBUG("Unregistering signal observer: " + ZLOG_INT(id_));
  return (this->_sig_handlers[id_].UnregisterObserver(obs_));
}

bool
SignalHandler::Notify(Signal::ID id_, siginfo_t *info_)
{
  bool status = false;
  std::map<Signal::ID, Signal*>::iterator it = this->_sigs.find(id_);
  if (it != this->_sigs.end())
  {
    ZLOG_DEBUG("Notifying signal observer: " + ZLOG_INT(id_));
    status = this->_sigs[id_]->Notify(info_);
  }
  return (status);
}

}
}

