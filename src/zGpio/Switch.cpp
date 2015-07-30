#include <zutils/zLog.h>
#include <zutils/zGpio.h>

namespace zUtils
{
namespace zGpio
{

//*****************************************************************************
// Switch Class
//*****************************************************************************

Switch::Switch(zSwitch::Switch::STATE state_)
{
  switch (state_)
  {
  case Switch::STATE_ON:
    if (this->_turnOn())
    {
      this->_state = Switch::STATE_ON;
    }
    break;
  case Switch::STATE_OFF:
    if (this->_turnOff())
    {
      this->_state = Switch::STATE_OFF;
    }
    break;
  default:
    break;
  }
}

Switch::~Switch()
{

}

bool
Switch::_turnOn()
{
  return (this->SetState(Port::STATE_ACTIVE));
}

bool
Switch::_turnOff()
{
  return (this->SetState(Port::STATE_INACTIVE));
}

}
}
