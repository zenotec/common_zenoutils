/*
 * DisplayCommand.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: kmahoney
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
    zCommand::Command(name_), _display(display_)
{
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
DisplayUpdateCmd::Execute(const std::string &arg_)
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
DisplayClearCmd::Execute(const std::string &arg_)
{
  this->_display.Clear();
  return (true);
}

}
}

