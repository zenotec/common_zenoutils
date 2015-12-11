/*
 * zThermoTest.h
 *
 *  Created on: Jan 11, 2015
 *      Author: kmahoney
 */

#ifndef _ZTHERMOTEST_H_
#define _ZTHERMOTEST_H_

#include "UnitTest.h"

#include <zutils/zThermo.h>

int
zThermoTest_Defaults(void* arg_);

using namespace Test;
using namespace zUtils;

class TestSensor : public zThermo::Sensor
{

public:
  TestSensor()
  {

  }

  virtual
  ~TestSensor()
  {

  }

protected:

  virtual bool
  _open()
  {
    return(true);
  }

  virtual bool
  _close()
  {
    return(true);
  }

private:

  virtual bool
  _read_temperature(double &temp_)
  {
    return(true);
  }

  virtual bool
  _set_alarm(double &min_temp_, double &max_temp_)
  {
    return(true);
  }

};

#endif /* _ZTHERMOTEST_H_ */
