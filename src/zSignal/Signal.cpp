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

#include <zutils/zEvent.h>
#include <zutils/zSignal.h>

namespace zUtils
{
namespace zSignal
{

static SignalEvent signal_tbl[Signal::ID_LAST];

int
_id2sig (Signal::ID id_)
{
  int sig = -1;
  switch(id_)
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
    default:
      sig = -1;
      break;
  }
  return(sig);
}

Signal::ID
_sig2id (int sig_)
{
  Signal::ID id = Signal::ID_ERR;
  switch(sig_)
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
  return(id);
}

//**********************************************************************
// Class: SignalEvent
//**********************************************************************
SignalEvent::SignalEvent () :
    Event (Event::TYPE_SIGNAL, 0)
{
}

SignalEvent::~SignalEvent ()
{
}

//**********************************************************************
// Class: Signal
//**********************************************************************

Signal::Signal (const Signal::ID& id_) :
    _id (id_)
{
  if ((id_ <= Signal::ID_NONE) || (id_ >= Signal::ID_LAST))
  {
    std::string errmsg = "Invalid signal identifier";
    throw errmsg;
  }
  signal_tbl[id_].SetId(static_cast<uint32_t>(id_));
  this->RegisterEvent (&signal_tbl[id_]);
  int sig = _id2sig(id_);
  if (sig < 0)
  {
    std::string errmsg = "Invalid signal identifier";
    throw errmsg;
  }
  this->_prev_handler = signal(sig, &this->_signal_handler_function);
}

Signal::~Signal ()
{
  int sig = _id2sig(this->_id);
  this->UnregisterEvent (&signal_tbl[this->_id]);
  signal(sig, this->_prev_handler);
}

Signal::ID
Signal::GetId ()
const
{
  return(this->_id);
}

void
Signal::_signal_handler_function (int sig_)
{
  Signal::ID id = _sig2id(sig_);
  if ((id > Signal::ID_NONE) && (id < Signal::ID_LAST))
  {
    signal_tbl[id].Notify((void *)id);
  }

}

}
}
