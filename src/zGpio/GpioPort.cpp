//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File: GpioPort.cpp
//    Description: Implements the GPIO Port class.
//
//*****************************************************************************

#include <stdio.h>
#include <fcntl.h>

#include <string>
#include <list>
#include <mutex>
#include <sstream>
#include <fstream>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConf.h>
#include <zutils/zSwitch.h>

#include <zutils/zGpio.h>

namespace zUtils
{
namespace zGpio
{


//**********************************************************************
// Class: Port
//**********************************************************************

Port::Port()
{
}

Port::Port(zConf::Configuration &config_) :
    zGpio::Configuration(config_)
{
}

Port::~Port()
{
  this->Close();
}


Port::DIR
Port::_direction() const
{
  Port::DIR dir = Port::DIR_ERR;
  char str[1024] = { 0 };
  std::fstream fs;
  std::string val;

  // Generate full path and filename of sysfs file to read
  if (snprintf(str, sizeof(str), this->DirectionFilename().c_str(), this->Identifier()) > 0)
  {
    // Open direction file and read direction
    fs.open(str, std::fstream::in);
    if (fs.is_open())
    {
      fs >> val;
      fs.close();
    }
  }
  ZLOG_DEBUG("zGpio::Port::GetDirection: Direction: '" + val + "'");

  // Convert from string to enum
  if (Port::ConfigDirectionValueIn == val)
  {
    dir = Port::DIR_IN;
  }
  else if (Port::ConfigDirectionValueOut == val)
  {
    dir = Port::DIR_OUT;
  }
  else
  {
    dir = Port::DIR_ERR;
  }

  // Return port direction
  return (dir);
}

//bool
//Port::SetDirection(Port::DIR dir_)
//{
//  bool status = false;
//  if (!this->_state_file)
//  {
//    if (Port::DIR_IN == dir_)
//    {
//      status = this->Set(Port::ConfigDirectionValuePath, Port::ConfigDirectionValueIn);
//    }
//    else if (Port::DIR_OUT == dir_)
//    {
//      status = this->Set(Port::ConfigDirectionValuePath, Port::ConfigDirectionValueOut);
//    }
//    else
//    {
//      status = false;
//    }
//  }
//  return (status);
//}

Port::STATE
Port::Get()
{
  Port::STATE state = Port::STATE_ERR;
//  std::string val;
//
//  if (this->_state_file)
//  {
//    std::fstream fs;
//
//    // Open value file and read value
//    fs.open(this->_conf_get_state_filename().c_str(), std::fstream::in);
//    if (fs.is_open())
//    {
//      fs >> val;
//      fs.close();
//    }
//
//    ZLOG_DEBUG("zGpio::Port::GetState: State: '" + val + "'");
//
//    if (Configuration::STATE_VALUE_ACTIVE == val)
//    {
//      state = Port::STATE_ACTIVE;
//    }
//    else if (Configuration::STATE_VALUE_INACTIVE == val)
//    {
//      state = Port::STATE_INACTIVE;
//    }
//    else
//    {
//      state = Port::STATE_ERR;
//    }
//  }

  return (state);
}

bool
Port::Set(Port::STATE state_)
{
  bool status = false;
//  if (this->_state_file && (this->GetDirection() == Configuration::DIR_OUT))
//  {
//    std::fstream fs;
//
//    // Open value file and write value
//    fs.open(this->_conf_get_state_filename().c_str(), std::fstream::out);
//    if (fs.is_open())
//    {
//      if (Port::STATE_ACTIVE == state_)
//      {
//        ZLOG_DEBUG("zGpio::Port::SetState: State: '" + Configuration::STATE_VALUE_ACTIVE + "'");
//        fs << Configuration::STATE_VALUE_ACTIVE;
//        status = true;
//      }
//      else if (Port::STATE_INACTIVE == state_)
//      {
//        ZLOG_DEBUG(
//            "zGpio::Port::SetState: State: '" + Configuration::STATE_VALUE_INACTIVE + "'");
//        fs << Configuration::STATE_VALUE_INACTIVE;
//        status = true;
//      }
//      else
//      {
//        ZLOG_ERR("zGpio::Port::SetState: Bad state: " + zLog::IntStr(state_));
//        status = false;
//      }
//      fs.flush();
//      fs.close();
//    }
//  }
  return (status);
}

//Port::EDGE
//Port::GetEdge()
//{
//  Port::EDGE edge = Port::EDGE_ERR;
//  std::string val;
//
//  if (this->_state_file)
//  {
//    std::fstream fs;
//    // Open edge file and read direction
//    fs.open(this->_conf_get_edge_filename().c_str(), std::fstream::in);
//    if (fs.is_open())
//    {
//      fs >> val;
//      fs.close();
//    }
//  }
//  else
//  {
//    val = this->_conf_get_edge_value();
//  }
//  ZLOG_DEBUG("zGpio::Port::GetEdge: Edge: '" + val + "'");
//
//  // Convert from string to enum
//  if (Configuration::ConfigEdgeValueNone == val)
//  {
//    edge = Port::EDGE_NONE;
//  }
//  else if (Configuration::ConfigEdgeValueLoHi == val)
//  {
//    edge = Port::EDGE_LO_HI;
//  }
//  else if (Configuration::ConfigEdgeValueHiLo == val)
//  {
//    edge = Port::EDGE_HI_LO;
//  }
//  else if (Configuration::ConfigEdgeValueBoth == val)
//  {
//    edge = Port::EDGE_BOTH;
//  }
//  else
//  {
//    edge = Port::EDGE_ERR;
//  }
//
//  // Return port edge configuration
//  return (edge);
//}
//
//bool
//Port::SetEdge(Port::EDGE edge_)
//{
//  bool status = false;
//  if (!this->_state_file)
//  {
//    if (Port::EDGE_NONE == edge_)
//    {
//      status = this->Set(Configuration::ConfigEdgeValuePath,
//          Configuration::ConfigEdgeValueNone);
//    }
//    else if (Port::EDGE_LO_HI == edge_)
//    {
//      status = this->Set(Configuration::ConfigEdgeValuePath,
//          Configuration::ConfigEdgeValueLoHi);
//    }
//    else if (Port::EDGE_HI_LO == edge_)
//    {
//      status = this->Set(Configuration::ConfigEdgeValuePath,
//          Configuration::ConfigEdgeValueHiLo);
//    }
//    else if (Port::EDGE_BOTH == edge_)
//    {
//      status = this->Set(Configuration::ConfigEdgeValuePath,
//          Configuration::ConfigEdgeValueBoth);
//    }
//    else
//    {
//      status = false;
//    }
//  }
//  return (status);
//}

bool
Port::Open()
{

//  std::fstream fs;
//  std::string val;
//
//  if (!this->_state_file)
//  {
//
//    ZLOG_INFO("zGpio::Port::_open: Opening GPIO Port: " + zLog::IntStr(this->GetId()));
//
//    // Open export file and write the GPIO identifier
//    fs.open(this->_conf_get_export_filename().c_str(), std::fstream::out);
//    if (!fs.is_open())
//    {
//      ZLOG_ERR("zGpio::Port::_open: Failed to open export file: "
//          + this->_conf_get_export_filename());
//      return (false);
//    }
//    else
//    {
//      fs << this->GetId() << std::endl;
//      fs.flush();
//      fs.close();
//    }
//
//    // Set the direction of the port
//    fs.open(this->_conf_get_direction_filename().c_str(), std::fstream::out);
//    if (!fs.is_open())
//    {
//      ZLOG_ERR("zGpio::Port::_open: Failed to open direction file: "
//          + this->_conf_get_direction_filename());
//      return (false);
//    }
//    else
//    {
//      fs << this->_conf_get_direction_value() << std::endl;
//      fs.flush();
//      fs.close();
//    }
//
//    // Set the type of edge to generate an interrupt on
//    fs.open(this->_conf_get_edge_filename().c_str(), std::fstream::out);
//    if (!fs.is_open())
//    {
//      ZLOG_ERR("zGpio::Port::_open: Failed to open edge file: "
//          + this->_conf_get_edge_filename());
//      return (false);
//    }
//    else
//    {
//      fs << this->_conf_get_edge_value() << std::endl;
//      fs.flush();
//      fs.close();
//    }
//
//    // Open the value file for watching for state changes
//    this->_state_file = open(this->_conf_get_state_filename().c_str(), (O_RDONLY | O_NONBLOCK));
//
//  }

  return (false);
}

bool
Port::Close()
{
//  if (this->_state_file)
//  {
//    std::fstream fs;
//
//    ZLOG_INFO("zGpio::Port::_close: Closing GPIO Port: " + zLog::IntStr(this->GetId()));
//
//    // Open unexport file and write the GPIO id
//    fs.open(this->_conf_get_unexport_filename().c_str(), std::fstream::out);
//    if (fs.is_open())
//    {
//      fs << this->GetId() << std::endl;
//      fs.flush();
//      fs.close();
//    }
//
//    close(this->_state_file);
//    this->_state_file = 0;
//  }
  return (false);
}

}
}
