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

#ifndef __ZGPIO_H__
#define __ZGPIO_H__

#include <string>

#include <zutils/zEvent.h>
#include <zutils/zThread.h>
#include <zutils/zConfig.h>

namespace zUtils
{
namespace zGpio
{

//**********************************************************************
// Class: GpioConfiguration
//**********************************************************************

class GpioConfiguration : public zConfig::Configuration
{

public:

  static const std::string ConfigRoot;

  static const std::string ConfigIdentifierPath;
  static const int ConfigIdentifierDefault;

  static const std::string ConfigExportFilenamePath;
  static const std::string ConfigExportFilenameDefault;

  static const std::string ConfigUnexportFilenamePath;
  static const std::string ConfigUnexportFilenameDefault;

  static const std::string ConfigDirectionFilenamePath;
  static const std::string ConfigDirectionFilenameDefault;
  static const std::string ConfigDirectionValuePath;
  static const std::string ConfigDirectionValueIn;
  static const std::string ConfigDirectionValueOut;
  static const std::string ConfigDirectionValueDefault;

  static const std::string ConfigStateFilenamePath;
  static const std::string ConfigStateFilenameDefault;
  static const std::string ConfigStateValuePath;
  static const std::string ConfigStateValueActive;
  static const std::string ConfigStateValueInactive;
  static const std::string ConfigStateValueDefault;

  static const std::string ConfigEdgeFilenamePath;
  static const std::string ConfigEdgeFilenameDefault;
  static const std::string ConfigEdgeValuePath;
  static const std::string ConfigEdgeValueNone;
  static const std::string ConfigEdgeValueLoHi;
  static const std::string ConfigEdgeValueHiLo;
  static const std::string ConfigEdgeValueBoth;
  static const std::string ConfigEdgeValueDefault;

  GpioConfiguration();

  GpioConfiguration(zData::Data &data_);

  GpioConfiguration(zConfig::Configuration &config_);

  virtual
  ~GpioConfiguration();

  zConfig::Configuration&
  GetConfig();

  int
  Identifier() const;

  bool
  Identifier(const int id_);

  std::string
  ExportFilename() const;

  bool
  ExportFilename(const std::string &filename_);

  std::string
  UnexportFilename() const;

  bool
  UnexportFilename(const std::string &filename_);

  std::string
  DirectionFilename() const;

  bool
  DirectionFilename(const std::string &filename_);

  std::string
  Direction() const;

  bool
  Direction(const std::string &dir_);

  std::string
  StateFilename() const;

  bool
  StateFilename(const std::string &filename_);

  std::string
  State() const;

  bool
  State(const std::string &state_);

  std::string
  EdgeFilename() const;

  bool
  EdgeFilename(const std::string &filename_);

  std::string
  Edge() const;

  bool
  Edge(const std::string &edge_);

};

//**********************************************************************
// Class: GpioPort
//**********************************************************************

class GpioPort : public zEvent::Event, public zThread::ThreadFunction
{

public:

  enum DIR
  {
    DIR_ERR = -1,
    DIR_NONE = 0,
    DIR_IN = 1,
    DIR_DEF = 1,
    DIR_OUT = 2,
    DIR_INOUT = 3,
    DIR_LAST
  };

  enum STATE
  {
    STATE_ERR = -1,
    STATE_NONE = 0,
    STATE_DEF = 0,
    STATE_ACTIVE = 1,
    STATE_INACTIVE = 2,
    STATE_LAST
  };

  enum EDGE
  {
    EDGE_ERR = -1,
    EDGE_NONE = 0,
    EDGE_DEF = 0,
    EDGE_LO_HI = 1,
    EDGE_HI_LO = 2,
    EDGE_BOTH = 3,
    EDGE_LAST
  };

  GpioPort();

  GpioPort(zConfig::Configuration& config_);

  virtual
  ~GpioPort();

  bool
  Open();

  bool
  Close();

  GpioPort::STATE
  Get();

  bool
  Set(GpioPort::STATE state_);

  GpioPort::DIR
  Direction() const;

  bool
  Direction(const GpioPort::DIR dir_);

  GpioPort::STATE
  State() const;

  bool
  State(const GpioPort::STATE state_);

  GpioPort::EDGE
  Edge() const;

  bool
  Edge(const GpioPort::EDGE edge_);

protected:

  virtual void
  Run(zThread::ThreadArg *arg_);

private:

  int _fd;
  zGpio::GpioConfiguration _config;
  zThread::Thread _thread;
  zSem::Mutex _lock;

  GpioPort::DIR
  _direction() const;

  bool
  _direction(const GpioPort::DIR dir_);

  GpioPort::STATE
  _state() const;

  bool
  _state(const GpioPort::STATE state_);

  GpioPort::EDGE
  _edge() const;

  bool
  _edge(const GpioPort::EDGE edge_);

};

//**********************************************************************
// Class: GpioHandler
//**********************************************************************

class GpioHandler : public zEvent::EventHandler
{

public:

  GpioHandler();

  virtual
  ~GpioHandler();

  bool
  Add(GpioPort *port_);

  bool
  Remove(GpioPort *port_);

  GpioPort::STATE
  Get();

  bool
  Set(GpioPort::STATE state_);

protected:

private:

  std::list<GpioPort *> _port_list;

};

//**********************************************************************
// Class: GpioNotification
//**********************************************************************

class GpioNotification : public zEvent::EventNotification
{

public:

  GpioNotification(zGpio::GpioPort::STATE state_, zGpio::GpioPort* port_);

  virtual
  ~GpioNotification();

  zGpio::GpioPort::STATE
  State();

  zGpio::GpioPort*
  Port();

protected:

private:

  zGpio::GpioPort::STATE _state;
  zGpio::GpioPort* _port;

};

//**********************************************************************
// Class: GpioManager
//**********************************************************************

class GpioManager : public GpioHandler
{
public:

  static GpioManager&
  Instance()
  {
    static GpioManager instance;
    return instance;
  }

protected:

private:

  GpioManager()
  {
  }

  GpioManager(GpioManager const&);

  void
  operator=(GpioManager const&);

};

//**********************************************************************
// Class: GpioSwitch
//**********************************************************************

class GpioSwitch : public zSwitch::Switch, public GpioHandler
{

public:

  GpioSwitch(zSwitch::Switch::STATE state_);

  virtual
  ~GpioSwitch();

protected:

private:

  virtual bool
  _turnOn();

  virtual bool
  _turnOff();

};

}
}

#endif /* __ZGPIO_H__ */
