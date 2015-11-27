//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Handler.cpp
//    Description:
//
//*****************************************************************************

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include <iostream>

#include <zutils/zCom.h>

namespace zUtils
{
namespace zCom
{

//*****************************************************************************
// Handler Class
//*****************************************************************************

Handler::Handler() :
    _thread(this, this)
{
  this->_mutex.Unlock();
}

Handler::~Handler()
{

  if (!this->_portList.empty())
  {
    this->_thread.Join(1000);
  }

  // Close all ports
  this->Close();

}

bool
Handler::Open(Port* com_)
{

  bool status = false;

  this->_mutex.Lock();

  if (com_ && com_->_open())
  {
    this->_portList.push_back(com_);
    status = true;
  }

  if (this->_portList.size() == 1)
  {
    this->_thread.Run(1000);
  }

  this->_mutex.Unlock();

  return (status);

}

void
Handler::Close(Port* com_)
{

  this->_mutex.Lock();

  if (com_)
  {
    com_->_close();
    this->_portList.remove(com_);
  }
  else
  {
    std::list<Port *>::iterator it = this->_portList.begin();
    std::list<Port *>::iterator end = this->_portList.end();
    for (; it != end; ++it)
    {
      (*it)->_close();
    }
  }

  this->_mutex.Unlock();

  return;

}

void *
Handler::ThreadFunction(void *arg_)
{

  Handler *self = (Handler *) arg_;

  if (!self->_portList.empty() && self->_mutex.TimedLock(1000))
  {
    std::list<Port *>::iterator it = self->_portList.begin();
    std::list<Port *>::iterator end = self->_portList.end();
    for (; it != end; ++it)
    {
      char c = 0;

      if ((*it)->RecvChar(&c, 10000))
      {
        self->_notify((*it), c);
      }
    }
    self->_mutex.Unlock();
  }
  usleep(1000);
  return (0);

}

bool
Handler::Register(ComReceiver* rcvr_)
{
  this->_mutex.Lock();
  this->_rcvrList.push_back(rcvr_);
  this->_mutex.Unlock();
  return (true);
}

void
Handler::Unregister(ComReceiver* rcvr_)
{
  this->_mutex.Lock();
  this->_rcvrList.remove(rcvr_);
  this->_mutex.Unlock();
}

void
Handler::_notify(Port* com_, char c_)
{
  std::list<ComReceiver*>::iterator it = this->_rcvrList.begin();
  std::list<ComReceiver*>::iterator end = this->_rcvrList.end();
  for (; it != end; ++it)
  {
    if ((*it)->RecvChar(com_, c_))
    {
      break;
    }
  }

}

}
}
