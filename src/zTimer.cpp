/*
 * Select.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: kmahoney
 */

#include <string.h>
#include <errno.h>

#include <string>

#include "zTimer.h"

namespace zUtils
{

//**********************************************************************
// Timer
//**********************************************************************

zTimer::zTimer() :
        _timerThreadId( 0 ), _exit( false )
{
    // Set the FD
    this->_setFd( this->_sem.GetFd() );
}

zTimer::~zTimer()
{
    this->Stop();
}

void zTimer::Start( uint64_t interval )
{
    this->_interval = interval;
    int ret = pthread_create( &this->_timerThreadId, NULL, zTimer::_timerThread, this );
    if( ret < 0 )
    {
        std::string errmsg( "Error: Cannot create timer thread: " );
        errmsg += std::string( strerror( errno ) );
        throw errmsg;
    } // end if
}

uint64_t zTimer::Acknowledge( const uint64_t &cnt_ )
{
    this->_sem.Acknowledge( cnt_ );
    return (this->_acknowledge( cnt_ ));
}

void zTimer::Stop()
{
    if( this->_timerThreadId )
    {
        this->_exit = true;
        pthread_join( this->_timerThreadId, 0 );
    } // end if
}

void *zTimer::_timerThread( void *arg_ )
{
    zTimer *timer = (zTimer *) arg_;
    while( timer->_exit == false )
    {
        usleep( timer->_interval );
        if( timer->_exit == false )
        {
            timer->_sem.Post();
            timer->_notify();
        } // end if
    } // end while

    // Reset exit flag
    timer->_exit = false;

    // Cleanup thread resources
    pthread_exit( 0 );

    // Return
    return (0);

}

}
