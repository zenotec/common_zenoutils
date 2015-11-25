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
  //    std::cout << "(" << this << ")Handler::Handler: Entry" << std::endl;
  //    std::cout.flush();
  //    std::cout << "(" << this << ")Handler::Handler: Exit" << std::endl;
  //    std::cout.flush();
  this->_mutex.Unlock();
}

Handler::~Handler()
{
//    std::cout << "(" << this << ")Handler::~Handler: Entry" << std::endl;
//    std::cout.flush();

  if (!this->_portList.empty())
  {
    this->_thread.Join(1000);
  }

  // Close all ports
  this->Close();

//    std::cout << "(" << this << ")Handler::~Handler: Exit" << std::endl;
//    std::cout.flush();
}

bool
Handler::Open(Port* com_)
{

//    std::cout << "(" << this << ")Handler::Open: Entry" << std::endl;
//    std::cout << "(" << this << ")Handler::Open(" << com_ << ")" << std::endl;
//    std::cout.flush();

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

  //    std::cout << "(" << this << ")Handler::Open: Exit" << std::endl;
  //    std::cout.flush();

  this->_mutex.Unlock();

  return (status);

}

void
Handler::Close(Port* com_)
{
//    std::cout << "(" << this << ")Handler::Close: Entry" << std::endl;
//    std::cout << "(" << this << ")Handler::Close(" << com_ << ")" << std::endl;
//    std::cout.flush();

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
//    std::cout << "(" << this << ")Handler::Close: Exit" << std::endl;
//    std::cout.flush();
  return;

}

void *
Handler::ThreadFunction( void *arg_ )
{

//    std::cout << "(" << this << ")Listening: Start: " << this->_exit << std::endl;
//    std::cout.flush();
  Handler *self = (Handler *) arg_;
  uint32_t usecs_ = 1000;

  if (!self->_portList.empty() && self->_mutex.TryLock())
  {
    std::list<Port *>::iterator it = self->_portList.begin();
    std::list<Port *>::iterator end = self->_portList.end();
    for (; it != end; ++it)
    {
      char c = 0;
//            std::cout << "(" << this << ")Listening: Port(" << *it << ")" << std::endl;
//            std::cout.flush();
      if ((*it)->RecvChar(&c, usecs_))
      {
        self->_notify((*it), c);
      }
    }
  }
  else
  {
    // If no ports have been opened or cannot get lock, just sleep the specified time
    usleep(usecs_);
  }
//    std::cout << "(" << this << ")Listening: Stop: " << this->_exit << std::endl;
//    std::cout.flush();
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
