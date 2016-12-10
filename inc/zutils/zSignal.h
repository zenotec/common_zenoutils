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

#ifndef __ZSIGNAL_H__
#define __ZSIGNAL_H__

#include <signal.h>

namespace zUtils
{
namespace zSignal
{

//**********************************************************************
// Class: Signal
//**********************************************************************

class Signal : public zEvent::Event
{

public:

  enum ID
  {
    ID_ERR = -1,
    ID_NONE = 0,
    ID_SIGCHLD = 1,
    ID_SIGTERM = 2,
    ID_SIGSEGV = 3,
    ID_SIGINT = 4,
    ID_SIGILL = 5,
    ID_SIGABRT = 6,
    ID_SIGALRM = 7,
    ID_SIGUSR1 = 8,
    ID_SIGUSR2 = 9,
    ID_LAST
  };

  Signal(const Signal::ID id_);

  virtual
  ~Signal();

  Signal::ID
  Id() const;

  bool
  Notify(siginfo_t *info_);

protected:

private:

  Signal::ID _id;
  struct sigaction _act;
  struct sigaction _oldact;

};

//**********************************************************************
// Class: SignalNotification
//**********************************************************************

class SignalNotification : public zEvent::EventNotification
{

  friend Signal;

public:

  SignalNotification(Signal* signal_);

  virtual
  ~SignalNotification();

  Signal::ID
  Id() const;

  const siginfo_t*
  SigInfo() const;

protected:

  void
  id(Signal::ID);

  void
  siginfo(const siginfo_t *info_);

private:

  Signal::ID _id;
  const siginfo_t *_info;

};

//**********************************************************************
// Class: SignalHandler
//**********************************************************************

class SignalHandler
{
public:

  SignalHandler();

  ~SignalHandler();

  bool
  RegisterObserver (Signal::ID id_, zEvent::EventObserver *obs_);

  bool
  UnregisterObserver (Signal::ID id_, zEvent::EventObserver *obs_);

  bool
  Notify(Signal::ID id_, siginfo_t *info_);

protected:

private:

  std::map<Signal::ID, zEvent::EventHandler> _sig_handlers;
  std::map<Signal::ID, Signal*> _sigs;

};

//**********************************************************************
// Class: SignalManager
//**********************************************************************

class SignalManager : public SignalHandler
{
public:

  static SignalManager&
  Instance()
  {
    static SignalManager instance;
    return instance;
  }

protected:

private:

  SignalManager()
  {
  }

  SignalManager(SignalManager const&);

  void
  operator=(SignalManager const&);


};


}
}

#endif /* __ZSIGNAL_H__ */
