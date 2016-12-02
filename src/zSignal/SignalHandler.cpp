//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

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

