/*
 * LcdCommand.cpp
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

#include <zutils/zLcd.h>

namespace zUtils
{
namespace zLcd
{

static const std::string LcdUpdateCmdName("update");
static const std::string LcdClearCmdName("clear");

//*****************************************************************************
// zLcd::LcdCommand Class
//*****************************************************************************

LcdCommand::LcdCommand(const std::string &name_, zLcd::Lcd &lcd_) :
    zCommand::Command(name_), _lcd(lcd_)
{
}

LcdCommand::~LcdCommand()
{
}

//*****************************************************************************
// zLcd::LcdUpdateCmd Class
//*****************************************************************************

LcdUpdateCmd::LcdUpdateCmd(zLcd::Lcd &lcd_) :
    zLcd::LcdCommand(LcdUpdateCmdName, lcd_)
{
}

LcdUpdateCmd::~LcdUpdateCmd()
{
}

bool
LcdUpdateCmd::Execute(const std::string &arg_)
{
  return (this->_lcd.Update(arg_));
}

//*****************************************************************************
// zLcd::LcdClearCmd Class
//*****************************************************************************

LcdClearCmd::LcdClearCmd(zLcd::Lcd &lcd_) :
    zLcd::LcdCommand(LcdClearCmdName, lcd_)
{
}

LcdClearCmd::~LcdClearCmd()
{
}

bool
LcdClearCmd::Execute(const std::string &arg_)
{
  return (this->_lcd.Clear());
}

}
}

