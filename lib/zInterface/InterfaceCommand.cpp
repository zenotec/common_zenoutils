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
#include <string>
#include <fstream>
#include <iostream>

// libzutils includes

#include <zutils/zCompatibility.h>
#include <zutils/zInterface.h>
#include <zutils/zInterfaceCommand.h>

namespace zUtils
{
namespace zInterface
{

//*****************************************************************************
// Class: InterfaceCommand
//*****************************************************************************

InterfaceCommand::InterfaceCommand(Interface& if_) :
    _if(if_)
{
}

InterfaceCommand::~InterfaceCommand()
{
}

//*****************************************************************************
// Class: ListInterfaceCommand
//*****************************************************************************

ListCommand::ListCommand(Interface& if_) :
    InterfaceCommand(if_)
{
  std::string name = "if." + if_.GetIfName() + ".list";
  this->SetName(name);
}

ListCommand::~ListCommand()
{
}

bool
ListCommand::Execute(zCommand::CommandData& data_)
{
}

}
}
