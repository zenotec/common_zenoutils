//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zThread.h
//    Description:
//
//*****************************************************************************

#ifndef __ZTHREAD_H__
#define __ZTHREAD_H__

#include <string>
#include <thread>

#include <zutils/zEvent.h>

namespace zUtils
{
namespace zThread
{

//**********************************************************************
// Class: ThreadArg
//**********************************************************************

class ThreadArg
{
public:

protected:

private:

};

//**********************************************************************
// Class: ThreadFunction
//**********************************************************************

class ThreadFunction
{
public:

  ThreadFunction();

  virtual
  ~ThreadFunction();

  virtual void
  Run(zThread::ThreadArg *arg_) = 0;

  bool
  Exit();

  bool
  Exit(bool flag_);

protected:

private:

  zSem::Mutex _exit;

};

//**********************************************************************
// Class: Thread
//**********************************************************************

class Thread : public zEvent::EventObserver
{
public:

  Thread(ThreadFunction *func_, ThreadArg *arg_);

  virtual
  ~Thread();

  unsigned long
  Id();

  std::string
  Name();

  void
  Name(const std::string &name_);

  bool
  Start();

  bool
  Join();

  bool
  Stop();

protected:

  virtual bool
  EventHandler(const zEvent::EventNotification* notification_);

private:

  std::thread *_thread;

  zEvent::EventHandler _sighandler;

  std::string _name;
  ThreadFunction *_func;
  ThreadArg *_arg;

};

}
}

#endif /* __ZTHREAD_H__ */
