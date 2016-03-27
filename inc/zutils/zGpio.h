//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

#ifndef _ZGPIO_H_
#define _ZGPIO_H_

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

class GpioPort : public GpioConfiguration
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

  int
  Open();

  bool
  Close();

  int
  Fd();

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

private:

  int _fd;
  zGpio::GpioConfiguration _config;

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

class GpioHandler : public zThread::Function , public zEvent::EventHandler
{

public:

  GpioHandler();

  virtual
  ~GpioHandler();

  bool
  Add(GpioPort *port_);

  GpioPort::STATE
  Get();

  bool
  Set(GpioPort::STATE state_);

protected:

  virtual void *
  ThreadFunction(void *arg_);

private:

  std::list<GpioPort *> _port_list;
  zThread::Thread _thread;
  zEvent::Event _event;

};

//**********************************************************************
// Class: GpioNotification
//**********************************************************************

class GpioNotification : public zEvent::EventNotification
{

public:

  GpioNotification(zGpio::GpioPort* port_);

  virtual
  ~GpioNotification();

protected:

private:

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

#endif /* _ZGPIO_H_ */
