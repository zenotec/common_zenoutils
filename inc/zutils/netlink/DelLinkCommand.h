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

#ifndef __NETLINK_DELLINKCOMMAND_H__
#define __NETLINK_DELLINKCOMMAND_H__

#include <zutils/netlink/Command.h>
#include <zutils/netlink/RouteLink.h>
#include <zutils/netlink/RouteSocket.h>

namespace netlink
{

//*****************************************************************************
// Class: DelLinkCommand
//*****************************************************************************

class DelLinkCommand : public Command, public RouteLink
{

public:

  DelLinkCommand(const unsigned int ifindex_);

  DelLinkCommand(const std::string& ifname_);

  virtual
  ~DelLinkCommand();

  virtual bool
  Exec();

  virtual void
  Display(const std::string& prefix_ = "") const;

protected:

private:

  RouteSocket _sock;

};

}

#endif /* __NETLINK_DELLINKCOMMAND_H__ */
