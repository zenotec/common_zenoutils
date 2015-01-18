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
Handler::Handler()
{
}

Handler::~Handler()
{
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
  return (status);
}

bool
Handler::RemovePort(Port* port_)
{
  bool status = false;
  if (port_->_close())
  {
    this->_portList.remove(port_);
    status = true;
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
    if ((state != Port::STATE_NONE) && (state != (*it)->_getState()))
    {
      return (Port::STATE_ERR);
    }
    state = (*it)->_getState();
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
    if (!(*it)->_setState(state_) || (state_ != (*it)->_getState()))
    {
      return (false);
    }
  }
  return (true);
}

}
}
