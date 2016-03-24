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
// Class: Configuration
//**********************************************************************

class Configuration : public zConf::Configuration
{

public:

  static const std::string ConfigRoot;

  static const std::string ConfigIdentifierValuePath;
  static const int ConfigIdentifierValueDefault;

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

  Configuration();

  Configuration(zData::Data &data_);

  Configuration(zConf::Configuration &config_);

  virtual
  ~Configuration();

  zConf::Configuration&
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
// Class: Event
//**********************************************************************

class Event : public zEvent::Event
{

public:

  enum EVENTID
  {
    EVENTID_ERR = -1,
    EVENTID_NONE = 0,
    EVENTID_LAST
  };

  Event();

  virtual
  ~Event();

protected:

private:

};

//**********************************************************************
// Class: Port
//**********************************************************************

class Port : public zGpio::Configuration
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

  Port();

  Port(zConf::Configuration& config_);

  virtual
  ~Port();

  bool
  Open();

  bool
  Close();

  Port::STATE
  Get();

  bool
  Set(Port::STATE state_);

protected:

private:

  zGpio::Configuration _config;

  Port::DIR
  _direction() const;

  bool
  _direction(const Port::DIR dir_);

  Port::STATE
  _state() const;

  bool
  _state(const Port::STATE state_);

  Port::EDGE
  _edge() const;

  bool
  _edge(const Port::EDGE edge_);

};

//**********************************************************************
// Class: Handler
//**********************************************************************

class Handler : public zThread::Function
{

public:

  Handler();

  virtual
  ~Handler();

  bool
  Add(Port *port_);

  Port::STATE
  Get();

  bool
  Set(Port::STATE state_);

protected:

  virtual void *
  ThreadFunction(void *arg_);

private:

  std::list<Port *> _portList;
  zThread::Thread _thread;

};

//**********************************************************************
// Class: Configuration
//**********************************************************************

class Switch : public zSwitch::Switch, public Handler
{

public:

  Switch(zSwitch::Switch::STATE state_);

  virtual
  ~Switch();

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
