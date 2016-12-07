//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#include <mutex>

#include <zutils/zTimer.h>
#include <zutils/zLog.h>

namespace zUtils
{
namespace zTimer
{

//**********************************************************************
// zTimer::TimerNotification Class
//**********************************************************************

TimerNotification::TimerNotification(Timer* timer_) :
    _tick(0), zEvent::EventNotification(timer_)
{
  if (timer_)
  {
    this->type(timer_->Type());
  }
}

TimerNotification::~TimerNotification()
{
}

uint64_t
TimerNotification::Tick()
{
  return(this->_tick);
}

void
TimerNotification::tick(uint64_t tick_)
{
  this->_tick = tick_;
  return;
}

}
}
