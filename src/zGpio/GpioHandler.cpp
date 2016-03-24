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
#include <zutils/zConf.h>
#include <zutils/zSwitch.h>

#include <zutils/zGpio.h>

namespace zUtils
{
namespace zGpio
{

//*****************************************************************************
// Handler Class
//*****************************************************************************
Handler::Handler() :
    _thread(this, this)
{
}

Handler::~Handler()
{
  if (!this->_portList.empty())
  {
    this->_thread.Join();
  }
//  std::list<Port*>::iterator it = this->_portList.begin();
//  std::list<Port*>::iterator end = this->_portList.end();
//  for (; it != end; ++it)
//  {
//    (*it)->_close();
//  }
  this->_portList.clear();
}

bool
Handler::Add(Port* port_)
{
  bool status = false;
//  if (port_->_open())
//  {
//    this->_portList.push_back(port_);
//    this->_portList.unique();
//    status = true;
//  }
//  if (this->_portList.size() == 1)
//  {
//    this->_thread.Run();
//  }
  return (status);
}

Port::STATE
Handler::Get()
{
  Port::STATE state = Port::STATE_NONE;
  std::list<Port*>::iterator it = this->_portList.begin();
  std::list<Port*>::iterator end = this->_portList.end();
  for (; it != end; ++it)
  {
    if ((state != Port::STATE_NONE) && (state != (*it)->Get()))
    {
      return (Port::STATE_ERR);
    }
    state = (*it)->Get();
  }
  return (state);
}

bool
Handler::Set(Port::STATE state_)
{
  Port::STATE state = Port::STATE_NONE;
  std::list<Port*>::iterator it = this->_portList.begin();
  std::list<Port*>::iterator end = this->_portList.end();
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
Handler::ThreadFunction(void *arg_)
{

  int nfds = 0;
  Handler *self = (Handler *) arg_;
  std::list<Port*>::iterator it;
  std::list<Port*>::iterator end = self->_portList.end();

  // TODO: Start critical section

//  // Setup for poll loop
//  struct pollfd fds[self->_portList.size()];
//  for (it = self->_portList.begin(), nfds = 0; it != end; ++it, nfds++)
//  {
//    fds[nfds].fd = (*it)->_state_file;
//    fds[nfds].events = (POLLPRI | POLLERR);
//  }
//
//  // Poll on GPIO ports
//  int ret = poll(fds, nfds, 100);
//  if (ret > 0)
//  {
//    it = self->_portList.begin();
//    for (it = self->_portList.begin(), nfds = 0; it != end; ++it, nfds++)
//    {
//      char buf[2] = { 0 };
//      int fd = (*it)->_state_file;
//      if ((fds[nfds].revents & POLLPRI) && pread(fd, buf, sizeof(buf), 0))
//      {
//        self->_notify(*(*it));
//      }
//    }
//  }

  return (0);
}

}
}
