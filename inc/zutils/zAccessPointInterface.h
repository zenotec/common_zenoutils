/*
 * Copyright (c) 2017 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
 *
 * Copyright (c) 2014-2017 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ZACCESSPOINTINTERFACE_H__
#define __ZACCESSPOINTINTERFACE_H__

#include <zutils/zInterface.h>
using namespace zUtils::zInterface;

#include <zutils/zWireless.h>

namespace zUtils
{
namespace zWireless
{

// ****************************************************************************
// Class: AccessPointInterface
// ****************************************************************************

class AccessPointInterface : public Interface
{
public:

  AccessPointInterface(const std::string &name_, const unsigned int phyindex_ = 0);

  virtual
  ~AccessPointInterface();

  virtual void
  Display(const std::string &prefix_ = std::string(""));

protected:

private:

};

}
}
#endif /* __ZACCESSPOINTINTERFACE_H__ */
