/*
 * Copyright (c) 2016 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
 *
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
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

// libc includes

// libc++ includes
#include <iostream>

// libzutils includes

#include <zutils/zMacVlanInterface.h>
#include <zutils/netlink/NewLinkCommand.h>
#include <zutils/netlink/DelLinkCommand.h>
using namespace netlink;

// local includes

namespace zUtils
{
namespace zInterface
{

// ****************************************************************************
// Class: MacVlan
// ****************************************************************************

MacVlan::MacVlan(const std::string& name_) :
    Interface(name_)
{
  this->SetIfType(ConfigData::IFTYPE_MACVLAN);
}

MacVlan::~MacVlan()
{
}

bool
MacVlan::Refresh()
{
  bool status = Interface::Refresh();
  return (status);
}

bool
MacVlan::Create()
{
  bool status = false;

  if (this->lock.Lock())
  {
    NewLinkCommand *cmd =
        new NewLinkCommand(NewLinkCommand::TYPE_MACVLAN, this->stagingConfig.GetIfName());
    cmd->MasterIfIndex(this->GetMasterIfIndex());
    cmd->HwAddress(this->stagingConfig.GetHwAddress());
    this->addCommand(cmd);
    status = this->execCommands();
    this->lock.Unlock();
  }

  if (status && this->GetIfIndex())
  {
    this->workingConfig.SetIfIndex(this->GetIfIndex());
    status &= this->Commit();
    status &= this->Refresh();
  }

  if (status)
  {
    *this->stagingConfig.GetData() = *this->workingConfig.GetData();
  }

  return (status);

}

bool
MacVlan::Destroy()
{

  bool status = false;

  if (this->lock.Lock())
  {
    DelLinkCommand* cmd = new DelLinkCommand(this->workingConfig.GetIfIndex());
    this->addCommand(cmd);
    status = this->execCommands();
    this->lock.Unlock();
  }

  return (status);

}

void
MacVlan::Display(const std::string& prefix_)
{
  Interface::Display(prefix_);
  std::cout << prefix_ << "-------- MacVlan Interface ------------" << std::endl;
}

}
}
