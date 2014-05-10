/*
 * Timer.h
 *
 *  Created on: Jan 28, 2014
 *      Author: kmahoney
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include <pthread.h>

#include "zutils/zSemaphore.h"

namespace zUtils
{

class zTimer: public zEvent
{
public:
    zTimer();
    ~zTimer();
    void Start( uint64_t interval );
    uint64_t Acknowledge( const uint64_t &cnt_ = 1);
    void Stop();

protected:

private:
    static void *_timerThread( void *arg_ );
    pthread_t _timerThreadId;
    bool _exit;
    uint64_t _interval;
    zSemaphore _sem;

};

}

#endif /* TIMER_H_ */
