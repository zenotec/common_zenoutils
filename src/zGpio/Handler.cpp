#include <sys/time.h>
#include <sys/types.h>
#include <sys/poll.h>

#include <sstream>
#include <fstream>

#include <zutils/zLog.h>
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
  std::list<Port*>::iterator it = this->_portList.begin();
  std::list<Port*>::iterator end = this->_portList.end();
  for (; it != end; ++it)
  {
    (*it)->_close();
  }
  this->_portList.clear();
}

bool
Handler::AddPort(Port* port_)
{
  bool status = false;
  if (port_->_open())
  {
    this->_portList.push_back(port_);
    this->_portList.unique();
    status = true;
  }
  if (this->_portList.size() == 1)
  {
    this->_thread.Run();
  }
  return (status);
}

Port*
Handler::GetPort(uint32_t id_)
{
  std::list<Port*>::iterator it = this->_portList.begin();
  std::list<Port*>::iterator end = this->_portList.end();
  for (; it != end; ++it)
  {
    if ((*it)->GetId() == id_)
    {
      return (*it);
    }
  }

  return (0);
}

Port::STATE
Handler::GetState()
{
  Port::STATE state = Port::STATE_NONE;
  std::list<Port*>::iterator it = this->_portList.begin();
  std::list<Port*>::iterator end = this->_portList.end();
  for (; it != end; ++it)
  {
    if ((state != Port::STATE_NONE) && (state != (*it)->GetState()))
    {
      return (Port::STATE_ERR);
    }
    state = (*it)->GetState();
  }
  return (state);
}

bool
Handler::SetState(Port::STATE state_)
{
  Port::STATE state = Port::STATE_NONE;
  std::list<Port*>::iterator it = this->_portList.begin();
  std::list<Port*>::iterator end = this->_portList.end();
  for (; it != end; ++it)
  {
    if (!(*it)->SetState(state_) || (state_ != (*it)->GetState()))
    {
      return (false);
    }
  }
  return (true);
}

bool
Handler::Register(Observer *obs_)
{
  if (!obs_)
  {
    return (false);
  }
  this->_obsList.push_back(obs_);
  return (true);
}

bool
Handler::Unregister(Observer *obs_)
{
  if (!obs_)
  {
    return (false);
  }
  this->_obsList.remove(obs_);
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

  // Setup for poll loop
  struct pollfd fds[self->_portList.size()];
  for (it = self->_portList.begin(), nfds = 0; it != end; ++it, nfds++)
  {
    fds[nfds].fd = (*it)->_state_file;
    fds[nfds].events = (POLLPRI | POLLERR);
  }

  // Poll on GPIO ports
  int ret = poll(fds, nfds, 100);
  if (ret > 0)
  {
    it = self->_portList.begin();
    for (it = self->_portList.begin(), nfds = 0; it != end; ++it, nfds++)
    {
      char buf[2] = { 0 };
      int fd = (*it)->_state_file;
      if ((fds[nfds].revents & POLLPRI) && pread(fd, buf, sizeof(buf), 0))
      {
        self->_notify(*(*it));
      }
    }
  }

  return (0);
}

void
Handler::_notify(zGpio::Port &port_)
{
  std::list<Observer*>::iterator it = this->_obsList.begin();
  std::list<Observer*>::iterator end = this->_obsList.end();
  for (; it != end; ++it)
  {
    (*it)->PortHandler(port_);
  }
}

}
}
