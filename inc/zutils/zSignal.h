//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

#ifndef _ZSIGNAL_H_
#define _ZSIGNAL_H_


namespace zUtils
{
namespace zSignal
{

//**********************************************************************
// Class: SignalEvent
//**********************************************************************

class SignalEvent : public zEvent::Event
{
public:

  SignalEvent();

  virtual
  ~SignalEvent();

protected:

private:

};

//**********************************************************************
// Class: Signal
//**********************************************************************

class Signal : public zEvent::EventHandler
{

public:

  enum ID
  {
    ID_ERR = -1,
    ID_NONE = 0,
    ID_SIGCHLD,
    ID_SIGTERM,
    ID_SIGSEGV,
    ID_SIGINT,
    ID_SIGILL,
    ID_SIGABRT,
    ID_SIGALRM,
    ID_SIGKILL,
    ID_SIGFPE,
    ID_SIGUSR1,
    ID_SIGUSR2,
    ID_LAST
  };

  Signal(const Signal::ID &id_);

  virtual
  ~Signal();

  Signal::ID
  GetId() const;

protected:

private:

  Signal::ID _id;

  void (*_prev_handler)(int);

  static void
  _signal_handler_function (int sig_);

};

}
}

#endif /* _SIGNAL_H_ */
