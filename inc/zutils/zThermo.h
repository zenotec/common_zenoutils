/*
 * zThermo.h
 *
 *  Created on: Dec 8, 2015
 *      Author: kmahoney
 */

#ifndef _ZTHERMO_H_
#define _ZTHERMO_H_

#include <zutils/zSem.h>
#include <zutils/zEvent.h>
#include <zutils/zThread.h>

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

#endif /* _ZTHERMO_H_ */
