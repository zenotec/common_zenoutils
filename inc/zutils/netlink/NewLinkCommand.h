/*
 * Copyright (c) 2018 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
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

#ifndef __NETLINK_NEWLINKCOMMAND_H__
#define __NETLINK_NEWLINKCOMMAND_H__

// libc includes

// libc++ includes
#include <string>

// libzutils includes

// local includes
#include "Command.h"
#include "RouteLink.h"
#include "RouteSocket.h"

namespace netlink
{

//*****************************************************************************
// Class: NewLinkCommand
//*****************************************************************************

class NewLinkCommand : public Command, public RouteLink
{

public:

  enum TYPE
  {
    TYPE_ERR = -1,
    TYPE_NONE = 0,
    TYPE_VLAN,
    TYPE_MACVLAN,
    TYPE_LAST
  };

  NewLinkCommand(const TYPE type_, const std::string& ifname_);

  virtual
  ~NewLinkCommand();

  virtual bool
  Exec();

  virtual void
  Display(const std::string& prefix_ = "") const;

protected:

private:

  RouteSocket _sock;
  TYPE _type;

};

}

#endif /* __NETLINK_NEWLINKCOMMAND_H__ */
