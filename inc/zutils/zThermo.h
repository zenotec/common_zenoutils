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

#ifndef __ZTHERMO_H__
#define __ZTHERMO_H__

namespace zUtils
{
namespace zThermo
{

class Sensor : public zEvent::Event
{

public:

  Sensor();

  virtual
  ~Sensor();

  bool
  Temperature(double &temp_);

  void
  GetAlarm(double &min_temp_, double &max_temp_);

  bool
  SetAlarm(double &min_temp_, double &max_temp_);

protected:

private:

  virtual bool
  _read_temperature(double &temp_) = 0;

  virtual bool
  _set_alarm(double &min_temp_, double &max_temp_) = 0;

  double _min_temp;
  double _max_temp;

};

}
}

#endif /* __ZTHERMO_H__ */
