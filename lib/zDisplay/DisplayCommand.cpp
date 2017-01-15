/*
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

#include <signal.h>

#include <string>
#include <mutex>
#include <list>
#include <vector>
#include <map>

#include <zutils/zSem.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>
#include <zutils/zTimer.h>
#include <zutils/zLog.h>
#include <zutils/zData.h>
#include <zutils/zSocket.h>
#include <zutils/zMessage.h>
#include <zutils/zCommand.h>

#include <zutils/zDisplay.h>

namespace zUtils
{
namespace zDisplay
{

static const std::string DisplayUpdateCmdName("update");
static const std::string DisplayClearCmdName("clear");

//*****************************************************************************
// Class: zDisplay::DisplayCommand
//*****************************************************************************

DisplayCommand::DisplayCommand(const std::string &name_, zDisplay::Display &display_) :
    _display(display_)
{
  this->SetName(name_);
}

DisplayCommand::~DisplayCommand()
{
}

//*****************************************************************************
// Class: zDisplay::DisplayUpdateCmd
//*****************************************************************************

DisplayUpdateCmd::DisplayUpdateCmd(zDisplay::Display &display_) :
    zDisplay::DisplayCommand(DisplayUpdateCmdName, display_)
{
}

DisplayUpdateCmd::~DisplayUpdateCmd()
{
}

bool
DisplayUpdateCmd::Execute(CommandData& data_)
{
  return (false);
}

//*****************************************************************************
// Class: zDisplay::DisplayClearCmd
//*****************************************************************************

DisplayClearCmd::DisplayClearCmd(zDisplay::Display &display_) :
    zDisplay::DisplayCommand(DisplayClearCmdName, display_)
{
}

DisplayClearCmd::~DisplayClearCmd()
{
}

bool
DisplayClearCmd::Execute(CommandData& data_)
{
  this->_display.Buffer.Clear();
  return (true);
}

}
}

