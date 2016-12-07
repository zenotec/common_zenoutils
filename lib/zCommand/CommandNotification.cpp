/*
 * Command.cpp
 *
 *  Created on: Jan 10, 2016
 *      Author: kmahoney
 */

#include <string>
#include <mutex>
#include <list>
#include <map>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zEvent.h>
#include <zutils/zData.h>
#include <zutils/zQueue.h>
#include <zutils/zSocket.h>
#include <zutils/zMessage.h>

#include <zutils/zCommand.h>

namespace zUtils
{
namespace zCommand
{

//**********************************************************************
// Class: CommandNotification
//**********************************************************************

CommandNotification::CommandNotification(Command* cmd_) :
    _id(CommandNotification::ID_NONE), zEvent::EventNotification(cmd_)
{
}

CommandNotification::~CommandNotification()
{

}

}
}
