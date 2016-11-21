//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

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
SignalNotification::SignalNotification(Signal* signal_) :
    _id(Signal::ID_NONE), zEvent::EventNotification(signal_)
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

void
SignalNotification::id(Signal::ID id_)
{
  this->_id = id_;
  return;
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
