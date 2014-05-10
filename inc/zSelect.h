/*
 * Select.h
 *
 *  Created on: Jan 28, 2014
 *      Author: kmahoney
 */

#ifndef SELECT_H_
#define SELECT_H_

#include <list>

#include "zEvent.h"

namespace zUtils
{

class zSelect
{
public:
    zSelect();
    ~zSelect();

    void RegisterEvent( zEvent *event_ );
    void DelEvent( zEvent *event_ );
    bool Wait( const uint32_t usec_ );

protected:

private:
    std::list<zEvent *> _eventList;
    fd_set _set;
    int _maxFd;

};

}

#endif /* SELECT_H_ */
