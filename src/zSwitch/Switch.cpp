//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zSwitch.cpp
//    Description:
//
//*****************************************************************************

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "zutils/zSwitch.h"
#include "zutils/zLog.h"

namespace zUtils
{
namespace zSwitch
{

//*****************************************************************************
// Class Switch
//*****************************************************************************

bool
Switch::On()
{
  if (!this->IsOn() && this->_turnOn())
  {
    this->_state = Switch::STATE_ON;
  }
  return (this->IsOn());
}

bool
Switch::Off()
{
  if (!this->IsOff() && this->_turnOff())
  {
    this->_state = Switch::STATE_OFF;
  }
  return (this->IsOff());
}

bool
Switch::Toggle()
{
  // If in any other state than off, first toggle off
  if (!this->IsOff() && this->_turnOff())
  {
    this->_state = Switch::STATE_OFF;
    return (true);
  }

  // Only if it is in off state, toggle on
  else if (this->IsOff() && this->_turnOn())
  {
    this->_state = Switch::STATE_ON;
    return (true);
  }

  // Everything else should cause error
  else
  {
    return (false);
  }

}

bool
Switch::IsOn()
{
  return (this->_state == Switch::STATE_ON);
}

bool
Switch::IsOff()
{
  return (this->_state == Switch::STATE_OFF);
}

}
}
