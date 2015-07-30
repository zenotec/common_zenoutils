/*
 * zSwitchTest.h
 *
 *  Created on: Jan 9, 2015
 *      Author: kmahoney
 */

#ifndef _ZTHREADTEST_H_
#define _ZTHREADTEST_H_

#include <unistd.h>

#include <zutils/zSwitch.h>

#include "UnitTest.h"

int
zSwitchTest_Defaults(void* arg_);

using namespace Test;
using namespace zUtils;

class TestSwitch : public zSwitch::Switch
{
public:
  TestSwitch(zSwitch::Switch::STATE state_)
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

  ~TestSwitch()
  {
  }

protected:

private:

  virtual bool
  _turnOn()
  {
    return(true);
  }

  virtual bool
  _turnOff()
  {
    return(true);
  }

};

#endif /* _ZSWITCHTEST_H_ */
