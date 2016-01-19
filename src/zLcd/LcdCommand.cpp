/*
 * LcdCommand.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: kmahoney
 */

#include <zutils/zLcd.h>

namespace zUtils
{
namespace zLcd
{

//*****************************************************************************
// zLcd::LcdUpdateCmd Class
//*****************************************************************************

LcdUpdateCmd::LcdUpdateCmd(zLcd::Lcd &lcd_) :
    _lcd(lcd_)
{
}

LcdUpdateCmd::~LcdUpdateCmd()
{
}

bool
LcdUpdateCmd::EventHandler(zEvent::Event *event_, void *arg_)
{

}

}
}


