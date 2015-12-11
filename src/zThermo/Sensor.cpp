/*
 * Sensor.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: kmahoney
 */

#include <zutils/zThermo.h>

namespace zUtils
{
namespace zThermo
{

//*****************************************************************************
// Sensor Class
//*****************************************************************************

bool
Sensor::Temperature(double &temp_)
{
  bool status = false;
  if (this->_read_temperature(temp_))
  {
    status = true;
  }
  return (status);
}

void
Sensor::GetAlarm(double &min_temp_, double &max_temp_)
{
  min_temp_ = this->_min_temp;
  max_temp_ = this->_max_temp;
}

bool
Sensor::SetAlarm(double &min_temp_, double &max_temp_)
{
  bool status = false;
  if (this->_set_alarm(min_temp_, max_temp_))
  {
    this->_min_temp = min_temp_;
    this->_max_temp = max_temp_;
    status = true;
  }
  return (status);
}

}
}
