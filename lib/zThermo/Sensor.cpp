/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>
#include <mutex>
#include <list>
#include <map>

#include <zutils/zSem.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>
#include <zutils/zThermo.h>

namespace zUtils
{
namespace zThermo
{

//*****************************************************************************
// Sensor Class
//*****************************************************************************

Sensor::Sensor() :
    zEvent::Event(zEvent::Event::TYPE_TEMP), _max_temp(0), _min_temp(0)
{

}

Sensor::~Sensor()
{

}

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
