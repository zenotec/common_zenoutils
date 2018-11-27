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

int
_id2sig(Signal::ID id_)
{
  int sig = -1;
  switch (id_)
  {
  case Signal::ID_SIGINT:
    sig = SIGINT;
    break;
  case Signal::ID_SIGTERM:
    sig = SIGTERM;
    break;
  case Signal::ID_SIGABRT:
    sig = SIGABRT;
    break;
  case Signal::ID_SIGALRM:
    sig = SIGALRM;
    break;
  case Signal::ID_SIGCHLD:
    sig = SIGCHLD;
    break;
  case Signal::ID_SIGUSR1:
    sig = SIGUSR1;
    break;
  case Signal::ID_SIGUSR2:
    sig = SIGUSR2;
    break;
  case Signal::ID_SIGTIMER:
    sig = SIGRTMIN;
    break;
  default:
    sig = -1;
    break;
  }
  return (sig);
}

Signal::ID
_sig2id(int sig_)
{
  Signal::ID id = Signal::ID_ERR;

  // Some signals are not constants so we need to use an if
  if (sig_ == SIGRTMIN)
  {
    id = Signal::ID_SIGTIMER;
  }
  else
  {
    switch (sig_)
    {
    case SIGINT:
      id = Signal::ID_SIGINT;
      break;
    case SIGTERM:
      id = Signal::ID_SIGTERM;
      break;
    case SIGABRT:
      id = Signal::ID_SIGABRT;
      break;
    case SIGALRM:
      id = Signal::ID_SIGALRM;
      break;
    case SIGCHLD:
      id = Signal::ID_SIGCHLD;
      break;
    case SIGUSR1:
      id = Signal::ID_SIGUSR1;
      break;
    case SIGUSR2:
      id = Signal::ID_SIGUSR2;
      break;
    default:
      id = Signal::ID_ERR;
      break;
    }
  }
  return (id);
}

static void
_sigaction_func(int sig_, siginfo_t *info_, void *arg_)
{
  Signal::ID id = _sig2id(sig_);
  Manager::Instance().Notify(id, info_);
}

//**********************************************************************
// Class: Signal
//**********************************************************************

Signal::Signal(const Signal::ID id_) :
    zEvent::Event(zEvent::TYPE_SIGNAL), _id(id_), _count(0)
{
  this->RegisterEvent(this);
  memset(&this->_act, 0, sizeof(this->_act));
  memset(&this->_oldact, 0, sizeof(this->_oldact));
  this->_act.sa_sigaction = _sigaction_func;
  this->_act.sa_flags = SA_SIGINFO;
  sigemptyset(&this->_act.sa_mask);
  int sig = _id2sig(id_);
  if (sig >= 0)
  {
    sigaction(sig, &this->_act, &this->_oldact);
  }
}

Signal::~Signal()
{
  this->UnregisterEvent(this);
  int sig = _id2sig(this->_id);
  sigaction(sig, &this->_oldact, NULL);
}

Signal::ID
Signal::Id() const
{
  return (this->_id);
}

uint64_t
Signal::Count() const
{
  return (this->_count);
}

zEvent::STATUS
Signal::Notify(siginfo_t *info_)
{
  this->_count++;
  Notification* notification = new Notification(*this);
  SHARED_PTR(zEvent::Notification) n(notification);
  notification->siginfo(info_);
  return (zEvent::Event::notifyHandlers(n));
}

}
}

