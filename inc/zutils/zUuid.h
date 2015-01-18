/*
 * zUuid.h
 *
 *  Created on: Jan 28, 2014
 *      Author: kmahoney
 */

#ifndef _ZUUID_H_
#define _ZUUID_H_

#include <stdint.h>

#include <uuid/uuid.h>

#include <string>
#include <sstream>

namespace zUtils
{
namespace zUuid
{

class Uuid
{
public:
  std::string
  operator()(const unsigned int len = 64)
  {
    uuid_t uuid;
    uuid_generate(uuid);
    char c_str[256] = { 0 };
    uuid_unparse(uuid, c_str);
    return (std::string(c_str));
  }

protected:

private:

};

}
}

#endif /* _ZUUID_H_ */
