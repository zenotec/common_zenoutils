/*
 * zUuid.h
 *
 *  Created on: Jan 28, 2014
 *      Author: kmahoney
 */

#ifndef _ZUUID_H_
#define _ZUUID_H_

#include <stdint.h>
#include <string>

#define INPUT   1
#define USER    1

namespace zUtils
{

class zUuid
{
public:
    std::string operator()( const int len = 64 );

protected:

private:

};

}

#endif /* _ZUUID_H_ */
