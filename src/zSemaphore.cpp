/*
 * Select.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: kmahoney
 */

#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sstream>

#include "zutils/zSemaphore.h"

namespace zUtils
{

//**********************************************************************
// Semaphore
//**********************************************************************

zSemaphore::zSemaphore()
{

    // Initialize lock to 'locked'
    int ret = sem_init( &this->_sem, 0, 0 );
    if( ret != 0 )
    {
        std::stringstream errmsg;
        errmsg << "Semaphore: Error initializing lock: " << ret;
        throw errmsg.str();
    } // end if

    // Create event file descriptor
    int id = eventfd( 0, EFD_NONBLOCK );
    if( id < 0 )
    {
        std::stringstream errmsg;
        errmsg << "Semaphore: Error creating event: " << id;
        throw errmsg.str();
    } // end if

    // Set event FD
    this->_setFd( id );

    // Unlock
    this->_unlock();

}

zSemaphore::~zSemaphore()
{
    int ret = sem_destroy( &this->_sem );
    if( ret != 0 )
    {
        std::string errmsg;
        errmsg = "Semaphore: Cannot destroy lock";
        throw errmsg;
    } // end if
    close( this->GetFd() );
}

void zSemaphore::Post( const uint64_t &cnt_ )
{
    // Wait for lock
    this->_lock();

    // Increment semaphore count
    int n = write( this->GetFd(), &cnt_, sizeof(cnt_) );
    if( n != sizeof(cnt_) )
    {
        std::string errmsg;
        errmsg = "Semaphore: Error reading event FD";
        this->_unlock();
        throw errmsg;
    } // end if
    this->_notify( cnt_ );
    // Unlock
    this->_unlock();
}

uint64_t zSemaphore::Acknowledge( const uint64_t &cnt_ )
{
    int n = 0;
    uint64_t cnt, pending = 0;

    // Wait for lock
    this->_lock();

    // Read event FD to get semaphore count
    n = read( this->GetFd(), &cnt, sizeof(uint64_t) );
    if( n != sizeof(uint64_t) )
    {
        std::string errmsg;
        errmsg = "Semaphore: Error reading event FD";
        this->_unlock();
        throw errmsg;
    } // end if

    // Acknowledge events
    this->_acknowledge( cnt_ );

    // Get updated pending count
    pending = this->GetPending();

    // Write back updated semaphore count
    n = write( this->GetFd(), &pending, sizeof(uint64_t) );
    if( n != sizeof(uint64_t) )
    {
        std::string errmsg;
        errmsg = "Semaphore: Error reading event FD";
        this->_unlock();
        throw errmsg;
    } // end if

    // Unlock
    this->_unlock();

    return (cnt);
}

void zSemaphore::_lock()
{
    sem_wait( &this->_sem );
}

void zSemaphore::_unlock()
{
    sem_post( &this->_sem );
}

}
