/*
 * Select.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: kmahoney
 */

#include <errno.h>
#include <string.h>
#include <sys/select.h>

#include "zutils/zSelect.h"

namespace zUtils
{

//**********************************************************************
// Select
//**********************************************************************
zSelect::zSelect() :
        _maxFd( 0 )
{
    FD_ZERO( &this->_set );
}

zSelect::~zSelect()
{
}

void zSelect::RegisterEvent( zEvent *event_ )
{
    // Update the max fd
    if( event_->GetFd() > this->_maxFd )
    {
        this->_maxFd = event_->GetFd();
    } // endif
    FD_SET( event_->GetFd(), &this->_set );
    this->_eventList.push_front( event_ );
}

void zSelect::DelEvent( zEvent *event_ )
{
    int maxFd = 0;
    // Iterate through event list
    std::list<zEvent *>::iterator iter = _eventList.begin();
    std::list<zEvent *>::iterator end = _eventList.end();
    for( ; iter != end; ++iter )
    {
        // Conditionally update the max FD
        if( (*iter)->GetFd() < maxFd )
        {
            maxFd = (*iter)->GetFd();
        } // end if
        if( (*iter)->GetFd() == event_->GetFd() )
        {
            // Remove FD from set
            FD_CLR( event_->GetFd(), &this->_set );
            // Remove from event list
            iter = this->_eventList.erase( iter );
        } // end if
    } // end for

    // Update max FD
    this->_maxFd = maxFd;
}

bool zSelect::Wait( const uint32_t usec_ )
{

    fd_set set;
    struct timeval to = { 0 };

    // Setup for select loop
    FD_ZERO( &set );
    set = this->_set;

    // Setup timeout value
    if( usec_ > 0 )
    {
        to.tv_sec = (usec_ / 1000000);
        to.tv_usec = (usec_ - (to.tv_sec * 1000000));
    } // end if

    // Select on socket
    int ret = select( this->_maxFd + 1, &set, NULL, NULL, &to );
    if( ret == 0 )
    {
        // Timed out waiting event activity
        return (false);
    } // end if

    return (true);
}

}
