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

  Signal();

  virtual
  ~Signal();

  Signal::ID
  Id() const;

  bool
  Id (const Signal::ID id_);

  void
  Notify();

protected:

private:

  Signal::ID _id;
  zEvent::Event _event;

  void (*_prev_handler)(int);

  static void
  _signal_handler_function (int sig_);

};

//**********************************************************************
// Class: SignalNotification
//**********************************************************************

class SignalNotification : public zEvent::EventNotification
{
public:

  SignalNotification(Signal::ID id_);

  virtual
  ~SignalNotification();

protected:

private:

  Signal::ID _id;

};

//**********************************************************************
// Class: SignalManager
//**********************************************************************

class SignalManager
{
public:

  static SignalManager&
  Instance()
  {
    static SignalManager instance;
    return instance;
  }

  void
  Notify(const Signal::ID id_)
  {
    if ((id_ > Signal::ID_NONE) && (id_ < Signal::ID_LAST))
    {
      this->_sigtbl[id_].Notify();
    }
  }

protected:

private:

  Signal _sigtbl[Signal::ID_LAST];

  SignalManager()
  {
    for (int sig = Signal::ID_NONE; sig < Signal::ID_LAST; sig++)
    {
      this->_sigtbl[sig].Id((Signal::ID)sig);
    }
  }

  SignalManager(SignalManager const&);

  void
  operator=(SignalManager const&);

};

}
}

#endif /* _SIGNAL_H_ */
