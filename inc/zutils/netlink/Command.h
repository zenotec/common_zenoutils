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

#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <zutils/zSem.h>
using namespace zUtils;

#include "Attribute.h"

//*****************************************************************************
// Class: Command
//*****************************************************************************

namespace netlink
{

class Command
{
public:

  Command(const unsigned int ifindex_);

  Command(const std::string& ifname_);

  virtual unsigned int
  GetIfIndex();

  virtual bool
  SetIfIndex(const unsigned int ifindex_);

  virtual
  ~Command();

  virtual bool
  Exec() = 0;

  virtual void
  Display() const = 0;

protected:

  bool _status;
  zSem::Semaphore _count;

private:

  unsigned int _ifindex;
  std::string _ifname;

};

}

#endif /* __COMMAND_H__ */
