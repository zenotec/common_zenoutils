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

#include <stdio.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <poll.h>

#include <string>
#include <list>
#include <mutex>
#include <memory>
#include <sstream>
#include <fstream>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>
#include <zutils/zSwitch.h>

#include <zutils/zGpio.h>

namespace zUtils
{
namespace zGpio
{

static std::string
filename(const int id, const std::string &fmt)
{
  char str[1024] = { 0 };
  snprintf(str, sizeof(str), fmt.c_str(), id);
  return (std::string(str));
}

static std::unique_ptr<std::fstream>
fs_open(const int id, const std::string &fmt)
{
  std::unique_ptr<std::fstream> fs(new std::fstream);

  // Generate full path and filename of sysfs file to read
  fs->open(filename(id, fmt), (std::fstream::in | std::fstream::out));

  return (fs);
}

static std::string
dir2str(const GpioPort::DIR dir_)
{
  std::string str;
  switch (dir_)
  {
  case GpioPort::DIR_IN:
    str = GpioConfigData::ConfigDirectionValueIn;
    break;
  case GpioPort::DIR_OUT:
    str = GpioConfigData::ConfigDirectionValueOut;
    break;
  default:
    break;
  }
  return (str);
}

static GpioPort::DIR
str2dir(const std::string &str_)
{
  GpioPort::DIR dir = GpioPort::DIR_ERR;
  // Convert from string to enum
  if (GpioConfigData::ConfigDirectionValueIn == str_)
  {
    dir = GpioPort::DIR_IN;
  }
  else if (GpioConfigData::ConfigDirectionValueOut == str_)
  {
    dir = GpioPort::DIR_OUT;
  }
  else
  {
    dir = GpioPort::DIR_ERR;
  }
  return (dir);
}

static std::string
state2str(const GpioPort::STATE state_)
{
  std::string str;
  switch (state_)
  {
  case GpioPort::STATE_ACTIVE:
    str = GpioConfigData::ConfigStateValueActive;
    break;
  case GpioPort::STATE_INACTIVE:
    str = GpioConfigData::ConfigStateValueInactive;
    break;
  default:
    break;
  }
  return (str);
}

static GpioPort::STATE
str2state(const std::string &str_)
{
  GpioPort::STATE state = GpioPort::STATE_ERR;
  // Convert from string to enum
  if (GpioConfigData::ConfigStateValueActive == str_)
  {
    state = GpioPort::STATE_ACTIVE;
  }
  else if (GpioConfigData::ConfigStateValueInactive == str_)
  {
    state = GpioPort::STATE_INACTIVE;
  }
  else
  {
    state = GpioPort::STATE_ERR;
  }
  return (state);
}

static std::string
edge2str(const GpioPort::EDGE edge_)
{
  std::string str;
  switch (edge_)
  {
  case GpioPort::EDGE_NONE:
    str = GpioConfigData::ConfigEdgeValueNone;
    break;
  case GpioPort::EDGE_LO_HI:
    str = GpioConfigData::ConfigEdgeValueLoHi;
    break;
  case GpioPort::EDGE_HI_LO:
    str = GpioConfigData::ConfigEdgeValueHiLo;
    break;
  case GpioPort::EDGE_BOTH:
    str = GpioConfigData::ConfigEdgeValueBoth;
    break;
  default:
    break;
  }
  return (str);
}

static GpioPort::EDGE
str2edge(const std::string &str_)
{
  GpioPort::EDGE edge = GpioPort::EDGE_ERR;
  // Convert from string to enum
  if (GpioConfigData::ConfigEdgeValueNone == str_)
  {
    edge = GpioPort::EDGE_NONE;
  }
  else if (GpioConfigData::ConfigEdgeValueLoHi == str_)
  {
    edge = GpioPort::EDGE_LO_HI;
  }
  else if (GpioConfigData::ConfigEdgeValueHiLo == str_)
  {
    edge = GpioPort::EDGE_HI_LO;
  }
  else if (GpioConfigData::ConfigEdgeValueBoth == str_)
  {
    edge = GpioPort::EDGE_BOTH;
  }
  else
  {
    edge = GpioPort::EDGE_ERR;
  }
  return (edge);
}

//**********************************************************************
// Class: Port
//**********************************************************************

GpioPort::GpioPort() :
    zEvent::Event(zEvent::Event::TYPE_GPIO), _fd(0), _thread(this, NULL)
{
}

GpioPort::GpioPort(GpioConfigData &config_) :
    GpioConfigData(config_), zEvent::Event(zEvent::Event::TYPE_GPIO), _fd(0),
        _thread(this, NULL)
{
}

GpioPort::~GpioPort()
{
  this->Close();
}

bool
GpioPort::Open()
{

  std::unique_ptr<std::fstream> fs;

  ZLOG_INFO("Opening GPIO Port: " + ZLOG_INT(GpioConfigData::Identifier()));

  // Open export file and verify
  fs = fs_open(GpioConfigData::Identifier(), GpioConfigData::ExportFilename());
  if (!fs->is_open())
  {
    ZLOG_ERR("Failed to open export file: " + GpioConfigData::ExportFilename());
    return (false);
  }

  // Write the GPIO identifier to initialize GPIO
  *fs << GpioConfigData::Identifier() << std::endl;
  fs->close();

  // Set the initial direction of the port
  if (!this->_direction(this->GetDirection()))
  {
    ZLOG_ERR("Failed to set direction");
    return (false);
  }

  // Set the type of edge to generate an interrupt on
  if (!this->_edge(this->GetEdge()))
  {
    ZLOG_ERR("Failed to set interrupt edge");
    return (false);
  }

  // Conditionally set initial state
  if ((this->GetDirection() == GpioPort::DIR_OUT) || (this->GetDirection() == GpioPort::DIR_INOUT))
  {
    if (!this->_state(this->GetState()))
    {
      ZLOG_ERR("Failed to set initial state");
      return (false);
    }
  }

  // Open the value file for watching for state changes
  if ((this->GetEdge() == GpioPort::EDGE_LO_HI) || (this->GetEdge() == GpioPort::EDGE_HI_LO)
      || (this->GetEdge() == GpioPort::EDGE_BOTH))
  {
    ZLOG_INFO(
        std::string("Opening 'state' GPIO file: ")
            + filename(GpioConfigData::Identifier(), GpioConfigData::StateFilename()));
    this->_fd = open(
        filename(GpioConfigData::Identifier(), GpioConfigData::StateFilename()).c_str(),
        (O_RDONLY | O_NONBLOCK));
    if (this->_fd > 0)
    {
      // Start monitoring for state changes
      this->_thread.Start();
    }
    else
    {
      this->_fd = 0;
      return (false);
    }
  }

  return (true);

}

bool
GpioPort::Close()
{

  std::unique_ptr<std::fstream> fs;

  ZLOG_INFO("Closing GPIO Port: " + ZLOG_INT(GpioConfigData::Identifier()));

  if (this->_fd)
  {
    // Stop monitoring for state changes
    this->_thread.Join();
    close(this->_fd);
    this->_fd = 0;
  }

  // Open export file and verify
  fs = fs_open(GpioConfigData::Identifier(), GpioConfigData::UnexportFilename());
  if (!fs->is_open())
  {
    ZLOG_ERR("Failed to open unexport file: " + GpioConfigData::UnexportFilename());
    return (false);
  }

  // Write the GPIO identifier to uninitialize GPIO
  *fs << GpioConfigData::Identifier() << std::endl;
  fs->close();

  return (true);
}

GpioPort::STATE
GpioPort::Get()
{
  return (this->_state());
}

bool
GpioPort::Set(GpioPort::STATE state_)
{
  return (this->_state(state_));
}

GpioPort::DIR
GpioPort::GetDirection() const
{
  return (str2dir(GpioConfigData::Direction()));
}

bool
GpioPort::SetDirection(const GpioPort::DIR dir_)
{
  return (GpioConfigData::Direction(dir2str(dir_)));
}

GpioPort::STATE
GpioPort::GetState() const
{
  return (str2state(GpioConfigData::State()));
}

bool
GpioPort::SetState(GpioPort::STATE state_)
{
  return (GpioConfigData::State(state2str(state_)));
}

GpioPort::EDGE
GpioPort::GetEdge() const
{
  return (str2edge(GpioConfigData::Edge()));
}

bool
GpioPort::SetEdge(const GpioPort::EDGE edge_)
{
  return (GpioConfigData::Edge(edge2str(edge_)));
}

void
GpioPort::Run(zThread::ThreadArg *arg_)
{

  // Setup for poll loop
  struct pollfd fds[1] = { 0 };
  fds[0].fd = this->_fd;
  fds[0].events = (POLLPRI | POLLERR);

  // Poll on GPIO ports
  while (!this->Exit())
  {
    int ret = poll(fds, 1, 100);
    if (ret > 0)
    {
      char buf[2] = { 0 };
      if ((fds[0].revents & POLLPRI) && pread(this->_fd, buf, sizeof(buf), 0))
      {
        zGpio::GpioPort::STATE state = this->_state();
        zGpio::GpioNotification notification(state, this);
        this->NotifyHandlers(&notification);
      }
    }
  }

  return;
}

GpioPort::DIR
GpioPort::_direction() const
{
  std::unique_ptr<std::fstream> fs;
  std::string val;

  // Open direction file and read direction
  fs = fs_open(GpioConfigData::Identifier(), GpioConfigData::DirectionFilename());
  if (fs->is_open())
  {
    *fs >> val;
    fs->close();
  }
  ZLOG_DEBUG("Direction: '" + val + "'");

  // Return port direction
  return (str2dir(val));
}

bool
GpioPort::_direction(const GpioPort::DIR dir_)
{
  bool status = false;
  std::unique_ptr<std::fstream> fs;

  // Open direction file and write direction
  fs = fs_open(GpioConfigData::Identifier(), GpioConfigData::DirectionFilename());
  if (fs->is_open())
  {
    *fs << dir2str(dir_);
    fs->close();
    status = true;
  }

  return (status);
}

GpioPort::STATE
GpioPort::_state() const
{
  std::unique_ptr<std::fstream> fs;
  std::string val;

  // Open direction file and read direction
  fs = fs_open(GpioConfigData::Identifier(), GpioConfigData::StateFilename());
  if (fs->is_open())
  {
    *fs >> val;
    fs->close();
  }
  ZLOG_DEBUG("State: '" + val + "'");

  // Return port direction
  return (str2state(val));
}

bool
GpioPort::_state(const GpioPort::STATE dir_)
{
  bool status = false;
  std::unique_ptr<std::fstream> fs;

  // Open direction file and write direction
  fs = fs_open(GpioConfigData::Identifier(), GpioConfigData::StateFilename());
  if (fs->is_open())
  {
    *fs << state2str(dir_);
    fs->close();
    status = true;
  }

  return (status);
}

GpioPort::EDGE
GpioPort::_edge() const
{
  std::unique_ptr<std::fstream> fs;
  std::string val;

  // Open direction file and read direction
  fs = fs_open(GpioConfigData::Identifier(), GpioConfigData::EdgeFilename());
  if (fs->is_open())
  {
    *fs >> val;
    fs->close();
  }
  ZLOG_DEBUG("State: '" + val + "'");

  // Return port direction
  return (str2edge(val));
}

bool
GpioPort::_edge(const GpioPort::EDGE dir_)
{
  bool status = false;
  std::unique_ptr<std::fstream> fs;

  // Open direction file and write direction
  fs = fs_open(GpioConfigData::Identifier(), GpioConfigData::EdgeFilename());
  if (fs->is_open())
  {
    *fs << edge2str(dir_);
    fs->close();
    status = true;
  }

  return (status);
}

}
}
