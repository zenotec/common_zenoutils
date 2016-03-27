#include <sys/time.h>
#include <sys/types.h>
#include <sys/poll.h>

#include <string>
#include <list>
#include <mutex>
#include <sstream>
#include <fstream>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>
#include <zutils/zSwitch.h>

#include <zutils/zGpio.h>

namespace zUtils
{
namespace zGpio
{

//*****************************************************************************
// Handler Class
//*****************************************************************************
GpioHandler::GpioHandler() :
    _thread(this, this), _event(zEvent::Event::TYPE_GPIO)
{
}

GpioHandler::~GpioHandler()
{
  if (!this->_port_list.empty())
  {
    this->_thread.Join();
  }
  std::list<GpioPort*>::iterator it = this->_port_list.begin();
  std::list<GpioPort*>::iterator end = this->_port_list.end();
  for (; it != end; ++it)
  {
    (*it)->Close();
  }
  this->_port_list.clear();
}

bool
GpioHandler::Add(GpioPort* port_)
{
  bool status = false;
  if (port_->Open() >= 0)
  {
    this->_port_list.push_back(port_);
    this->_port_list.unique();
    status = true;
  }
  if (this->_port_list.size() == 1)
  {
    this->_thread.Run();
  }
  return (status);
}

GpioPort::STATE
GpioHandler::Get()
{
  GpioPort::STATE state = GpioPort::STATE_NONE;
  std::list<GpioPort*>::iterator it = this->_port_list.begin();
  std::list<GpioPort*>::iterator end = this->_port_list.end();
  for (; it != end; ++it)
  {
    if ((state != GpioPort::STATE_NONE) && (state != (*it)->Get()))
    {
      return (GpioPort::STATE_ERR);
    }
    state = (*it)->Get();
  }
  return (state);
}

bool
GpioHandler::Set(GpioPort::STATE state_)
{
  GpioPort::STATE state = GpioPort::STATE_NONE;
  std::list<GpioPort*>::iterator it = this->_port_list.begin();
  std::list<GpioPort*>::iterator end = this->_port_list.end();
  for (; it != end; ++it)
  {
    if (!(*it)->Set(state_) || (state_ != (*it)->Get()))
    {
      return (false);
    }
  }
  return (true);
}

void *
GpioHandler::ThreadFunction(void *arg_)
{

  int nfds = 0;
  GpioHandler *self = (GpioHandler *) arg_;
  std::list<GpioPort*>::iterator it;
  std::list<GpioPort*>::iterator end = self->_port_list.end();

  // TODO: Start critical section

  // Setup for poll loop
  struct pollfd fds[self->_port_list.size()];
  for (it = self->_port_list.begin(), nfds = 0; it != end; ++it, nfds++)
  {
    fds[nfds].fd = (*it)->Fd();
    fds[nfds].events = (POLLPRI | POLLERR);
  }

  // Poll on GPIO ports
  int ret = poll(fds, nfds, 100);
  if (ret > 0)
  {
    it = self->_port_list.begin();
    for (it = self->_port_list.begin(), nfds = 0; it != end; ++it, nfds++)
    {
      char buf[2] = { 0 };
      int fd = (*it)->Fd();
      if ((fds[nfds].revents & POLLPRI) && pread(fd, buf, sizeof(buf), 0))
      {
        zGpio::GpioNotification notification(*it);
        self->_event.Notify(&notification);
      }
    }
  }

  return (0);
}
//**********************************************************************
// Class: Notification
//**********************************************************************

GpioNotification::GpioNotification(zGpio::GpioPort* port_) :
    _port(port_)
{

}

GpioNotification::~GpioNotification()
{

}

}
}
