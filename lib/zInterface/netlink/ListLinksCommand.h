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

#ifndef __NETLINK_LISTLINKSCOMMAND_H__
#define __NETLINK_LISTLINKSCOMMAND_H__

// libc includes

// libc++ includes
#include <string>
#include <map>

// libzutils includes

// local includes
#include "Command.h"

#include "RouteSocket.h"
#include "RouteLink.h"

namespace netlink
{

//*****************************************************************************
// Class: ListLinksCommand
//*****************************************************************************

class ListLinksCommand : public Command
{

public:

  std::map<int, RouteLink> Link;

  ListLinksCommand();

  virtual
  ~ListLinksCommand();

  virtual bool
  Exec();

  void
  Display() const;

protected:

private:

  RouteSocket _sock;

};

}

#endif /* __NETLINK_LISTLINKSCOMMAND_H__ */
