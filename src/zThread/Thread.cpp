//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

#include <string.h>

#include <mutex>
#include <list>
#include <map>

#include <zutils/zSem.h>
#include <zutils/zLog.h>
#include <zutils/zEvent.h>
#include <zutils/zSignal.h>

#include <zutils/zThread.h>

namespace zUtils
{
namespace zThread
{

//**********************************************************************
// Class: ThreadFunction
//**********************************************************************

ThreadFunction::ThreadFunction() :
    _exit(zSem::Mutex::LOCKED)
{
}

ThreadFunction::~ThreadFunction()
{
}

bool
ThreadFunction::Exit()
{
  return (this->_exit.TryLock());
}

bool
ThreadFunction::Exit(bool flag_)
{
  bool status = false;
  return (status);
}

//*****************************************************************************
// Class Thread
//*****************************************************************************
Thread::Thread(ThreadFunction *func_, ThreadArg *arg_) :
    _thread(NULL), _func(func_), _arg(arg_)
{
  zSignal::SignalManager::Instance().RegisterObserver(zSignal::Signal::ID_SIGTERM, this);
  zSignal::SignalManager::Instance().RegisterObserver(zSignal::Signal::ID_SIGINT, this);
}

Thread::~Thread()
{
  // Terminate listener thread
  this->Join();
}

bool
Thread::Start()
{
  bool status = false;
  if (this->_func)
  {
    this->_thread = new std::thread(&ThreadFunction::Run, this->_func, this->_arg);
    status = true;
  }
  return (status);
}

bool
Thread::Join()
{
  bool status = false;
  if (this->_func && this->_thread && this->_thread->joinable())
  {
    this->_thread->join();
    this->_thread = NULL;
    status = true;
  }
  return (status);
}

bool
Thread::Stop()
{
  bool status = false;
  if (this->_func)
  {
    this->_func->Exit(true);
    status = this->Join();
  }
  return (status);
}

std::string
Thread::Name()
{
  return (this->_name);
}

void
Thread::Name(const std::string &name_)
{
  this->_name = name_;
  return;
}

bool
Thread::EventHandler(const zEvent::EventNotification* notification_)
{
  bool status = false;
  const zSignal::SignalNotification *n = NULL;

  if (notification_ && (notification_->Type() == zEvent::Event::TYPE_SIGNAL))
  {
    n = static_cast<const zSignal::SignalNotification*>(notification_);
    switch (n->Id())
    {

    }
  }
  return status;
}

}
}
