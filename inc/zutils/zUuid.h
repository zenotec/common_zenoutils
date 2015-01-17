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
#include <sstream>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

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
    boost::uuids::random_generator rg;
    boost::uuids::uuid uuid = rg();
    std::stringstream ss;
    ss << uuid;
    return (ss.str());
  }

protected:

private:

};

}
}

#endif /* _ZUUID_H_ */
