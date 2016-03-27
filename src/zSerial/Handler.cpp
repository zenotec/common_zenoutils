//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Port.cpp
//    Description:
//
//*****************************************************************************

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include <list>
#include <mutex>

#include <zutils/zQueue.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>
#include <zutils/zSerial.h>

namespace zUtils
{
namespace zSerial
{

//**********************************************************************
// Class: SerialHandler
//**********************************************************************

SerialHandler::SerialHandler()
{

}

SerialHandler::~SerialHandler()
{

}

bool
SerialHandler::Add(SerialPort *port_)
{

}

}
}
