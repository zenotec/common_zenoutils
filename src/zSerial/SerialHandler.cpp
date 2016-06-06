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
#include <zutils/zThread.h>
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
  std::list<SerialPort*>::iterator it = this->_port_list.begin();
  std::list<SerialPort*>::iterator end = this->_port_list.end();
  for (; it != end; ++it)
  {
    this->UnregisterEvent(*it);
    (*it)->Close();
  }
  this->_port_list.clear();
}

bool
SerialHandler::Add(SerialPort *port_)
{
  bool status = false;
  if (port_ && (port_->Open()))
  {
    this->RegisterEvent(port_);
    this->_port_list.push_back(port_);
    this->_port_list.unique();
    status = true;
  }
  return (status);
}

}
}
