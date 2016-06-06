//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: SerialNotification.cpp
//    Description:
//
//*****************************************************************************

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
// Class: SerialNotification
//**********************************************************************

SerialNotification::SerialNotification(const SerialNotification::ID id_, SerialPort* port_) :
    _id(id_), _port(port_)
{
}

SerialNotification::~SerialNotification()
{
}

SerialNotification::ID
SerialNotification::Id() const
{
  return(this->_id);
}


}
}
