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

//**********************************************************************
// Class: Notification
//**********************************************************************

GpioNotification::GpioNotification(zGpio::GpioPort::STATE state_, zGpio::GpioPort* port_) :
    _state(state_), _port(port_)
{
  this->type(zEvent::Event::TYPE_GPIO);
}

GpioNotification::~GpioNotification()
{
  this->type(zEvent::Event::TYPE_ERR);
  this->_state = GpioPort::STATE_ERR;
}

zGpio::GpioPort::STATE
GpioNotification::State()
{
  return(this->_state);
}

zGpio::GpioPort*
GpioNotification::Port()
{
  return(this->_port);
}

}
}
