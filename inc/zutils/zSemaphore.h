/*
 * Semaphore.h
 *
 *  Created on: Jan 28, 2014
 *      Author: kmahoney
 */

#ifndef _ZSEMAPHORE_H_
#define _ZSEMAPHORE_H_

#include <semaphore.h>
#include <sys/select.h>
#include <sys/eventfd.h>

#include "zutils/zEvent.h"

namespace zUtils
{

class zSemaphore : public zEvent
{
public:
    zSemaphore();
    ~zSemaphore();

    void Post( const uint64_t &cnt_ = 1 );
    uint64_t Acknowledge( const uint64_t &cnt_ = 1 );

protected:

private:
    void _lock();
    void _unlock();
    sem_t _sem;
};

}

#endif /* _ZSEMAPHORE_H_ */
