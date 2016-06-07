
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
GpioHandler::GpioHandler()
{
}

GpioHandler::~GpioHandler()
{
  std::list<GpioPort*>::iterator it = this->_port_list.begin();
  std::list<GpioPort*>::iterator end = this->_port_list.end();
  for (; it != end; ++it)
  {
    this->UnregisterEvent(*it);
    (*it)->Close();
  }
  this->_port_list.clear();
}

bool
GpioHandler::Add(GpioPort* port_)
{
  bool status = false;
  if (port_ && (port_->Open() >= 0))
  {
    this->_port_list.push_back(port_);
    this->_port_list.unique();
    this->RegisterEvent(port_);
    status = true;
  }
  return (status);
}

bool
GpioHandler::Remove(GpioPort* port_)
{
  bool status = false;

  if (port_ && port_->Close())
  {
    this->_port_list.remove(port_);
    this->UnregisterEvent(port_);
    status = true;
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

}
}
