/*
 * Copyright (c) 2017 Cable Television Laboratories, Inc. ("CableLabs")
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

#ifndef __NETLINK_SETLINKCOMMAND_H__
#define __NETLINK_SETLINKCOMMAND_H__

#include "Command.h"
#include "RouteLink.h"
#include "RouteSocket.h"

namespace netlink
{

//*****************************************************************************
// Class: SetLinkCommand
//*****************************************************************************

class SetLinkCommand : public Command, public RouteLink
{

public:

  SetLinkCommand(const unsigned int ifindex_);

  SetLinkCommand(const std::string& ifname_);

  virtual
  ~SetLinkCommand();

  virtual bool
  Exec();

  virtual void
  Display(const std::string& prefix_ = "") const;

protected:

private:

  RouteSocket _sock;
  RouteLink _orig;

};

}

#endif /* __NETLINK_SETLINKCOMMAND_H__ */
