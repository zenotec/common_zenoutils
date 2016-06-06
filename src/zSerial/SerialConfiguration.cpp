//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Configuration.cpp
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
// Class: SerialConfiguration
//**********************************************************************

const std::string SerialConfiguration::ConfigRoot("zSerial");
const std::string SerialConfiguration::ConfigTypePath("Type");
const std::string SerialConfiguration::ConfigTypeNone("NONE");
const std::string SerialConfiguration::ConfigTypeTty("TTY");

SerialConfiguration::SerialConfiguration()
{
  this->Type(ConfigTypeNone);
}

SerialConfiguration::SerialConfiguration(zData::Data &data_) :
    zConfig::Configuration(data_)
{
  this->Type(this->Type());
}

SerialConfiguration::SerialConfiguration(zConfig::Configuration &config_) :
    zConfig::Configuration(config_)
{
  this->Type(this->Type());
}

SerialConfiguration::~SerialConfiguration()
{
}

zConfig::Configuration&
SerialConfiguration::GetConfig()
{
  return (*this);
}

std::string
SerialConfiguration::Type() const
{
  std::string type;
  std::string path = zData::Data::PathConcat(SerialConfiguration::ConfigRoot,
      SerialConfiguration::ConfigTypePath);
  this->Get(type, path);
  return (type);
}

bool
SerialConfiguration::Type(const std::string& type_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(SerialConfiguration::ConfigRoot,
      SerialConfiguration::ConfigTypePath);
  if (this->Put(type_, path))
  {
    status = this->Commit();
  }
  return (status);
}

}
}
