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

#ifndef __NETLINK_ROUTELINK_H__
#define __NETLINK_ROUTELINK_H__

#include <stdint.h>

#include <string>

#include <linux/if_link.h>
#include <netlink/route/link.h>

namespace netlink
{

//*****************************************************************************
// Class: RouteLink
//*****************************************************************************

class RouteLink
{

public:

  enum STATE
  {
    STATE_ERR = -1,
    STATE_UNKNOWN = 0,
    STATE_UP = 1,
    STATE_DOWN = 2,
    STATE_LAST
  };

  RouteLink(struct rtnl_link *link_ = NULL);

  RouteLink(RouteLink& other_);

  virtual
  ~RouteLink();

  virtual struct rtnl_link *
  operator()();

  RouteLink&
  operator=(RouteLink& other_);

  struct rtnl_link*
  operator ()(struct rtnl_link *link_);

  int
  IfIndex() const;

  bool
  IfIndex (const int index_);

  std::string
  IfName() const;

  bool
  IfName(const std::string& name_);

  unsigned int
  Type() const;

  bool
  Type(const unsigned int type_);

  std::string
  TypeString() const;

  bool
  TypeString(const std::string& type_);

  uint32_t
  Flags() const;

  bool
  SetFlags(const uint32_t flags_);

  bool
  ClrFlags(const uint32_t flags_);

  uint32_t
  Mtu() const;

  bool
  Mtu(const uint32_t mtu_);

  RouteLink::STATE
  OperationalState() const;

  bool
  OperationalState(const RouteLink::STATE state_);

  RouteLink::STATE
  CarrierState() const;

  bool
  CarrierState(const RouteLink::STATE state_);

  std::string
  Mac() const;

  bool
  Mac(const std::string& mac_);

  void
  Display() const;

protected:

  struct rtnl_link *_link;

private:

  RouteLink(const RouteLink& other_);

  RouteLink&
  operator=(const RouteLink& other_);

};

}

#endif /* __NETLINK_ROUTELINK_H__ */
