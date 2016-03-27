//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

#include <string>
#include <list>
#include <mutex>

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

//**********************************************************************
// Class: Configuration
//**********************************************************************

const std::string GpioConfiguration::ConfigRoot("zGpio");

const std::string GpioConfiguration::ConfigIdentifierPath("Identifier");
const int GpioConfiguration::ConfigIdentifierDefault(0);

const std::string GpioConfiguration::ConfigExportFilenamePath("ExportFilename");
const std::string GpioConfiguration::ConfigExportFilenameDefault("/sys/class/gpio/export");

const std::string GpioConfiguration::ConfigUnexportFilenamePath("UnexportFilename");
const std::string GpioConfiguration::ConfigUnexportFilenameDefault("/sys/class/gpio/unexport");

const std::string GpioConfiguration::ConfigDirectionFilenamePath("DirectionFilename");
const std::string GpioConfiguration::ConfigDirectionFilenameDefault("/sys/class/gpio/gpio%d/direction");

const std::string GpioConfiguration::ConfigDirectionValuePath("Direction");
const std::string GpioConfiguration::ConfigDirectionValueIn("in");
const std::string GpioConfiguration::ConfigDirectionValueOut("out");
const std::string GpioConfiguration::ConfigDirectionValueDefault(GpioConfiguration::ConfigDirectionValueIn);

const std::string GpioConfiguration::ConfigStateFilenamePath("StateFilename");
const std::string GpioConfiguration::ConfigStateFilenameDefault("/sys/class/gpio/gpio%d/value");

const std::string GpioConfiguration::ConfigStateValuePath("State");
const std::string GpioConfiguration::ConfigStateValueActive("1");
const std::string GpioConfiguration::ConfigStateValueInactive("0");
const std::string GpioConfiguration::ConfigStateValueDefault(GpioConfiguration::ConfigStateValueInactive);

const std::string GpioConfiguration::ConfigEdgeFilenamePath("EdgeFilename");
const std::string GpioConfiguration::ConfigEdgeFilenameDefault("/sys/class/gpio/gpio%d/edge");

const std::string GpioConfiguration::ConfigEdgeValuePath("Edge");
const std::string GpioConfiguration::ConfigEdgeValueNone("none");
const std::string GpioConfiguration::ConfigEdgeValueLoHi("rising");
const std::string GpioConfiguration::ConfigEdgeValueHiLo("falling");
const std::string GpioConfiguration::ConfigEdgeValueBoth("both");
const std::string GpioConfiguration::ConfigEdgeValueDefault(GpioConfiguration::ConfigEdgeValueNone);

GpioConfiguration::GpioConfiguration()
{
  this->Identifier(ConfigIdentifierDefault);
  this->ExportFilename(ConfigExportFilenameDefault);
  this->UnexportFilename(ConfigUnexportFilenameDefault);
  this->DirectionFilename(ConfigDirectionFilenameDefault);
  this->Direction(ConfigDirectionValueDefault);
  this->StateFilename(ConfigStateFilenameDefault);
  this->State(ConfigStateValueDefault);
  this->EdgeFilename(ConfigEdgeFilenameDefault);
  this->Edge(ConfigEdgeValueDefault);
}

GpioConfiguration::GpioConfiguration(zData::Data& data_) :
    zConfig::Configuration(data_)
{
  this->Identifier(this->Identifier());
  this->ExportFilename(this->ExportFilename());
  this->UnexportFilename(this->UnexportFilename());
  this->DirectionFilename(this->DirectionFilename());
  this->Direction(this->Direction());
  this->StateFilename(this->StateFilename());
  this->State(this->State());
  this->EdgeFilename(this->EdgeFilename());
  this->Edge(this->Edge());
}

GpioConfiguration::GpioConfiguration(zConfig::Configuration& config_) :
    zConfig::Configuration(config_)
{
  this->Identifier(this->Identifier());
  this->ExportFilename(this->ExportFilename());
  this->UnexportFilename(this->UnexportFilename());
  this->DirectionFilename(this->DirectionFilename());
  this->Direction(this->Direction());
  this->StateFilename(this->StateFilename());
  this->State(this->State());
  this->EdgeFilename(this->EdgeFilename());
  this->Edge(this->Edge());
}

GpioConfiguration::~GpioConfiguration()
{
}

zConfig::Configuration&
GpioConfiguration::GetConfig()
{
  return (*this);
}

int
GpioConfiguration::Identifier() const
{
  int id;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigIdentifierPath);
  this->Get(id, path);
  return (id);
}

bool
GpioConfiguration::Identifier(const int id_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigIdentifierPath);
  if (this->Put(id_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
GpioConfiguration::ExportFilename() const
{
  std::string filename;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigExportFilenamePath);
  this->Get(filename, path);
  return (filename);
}

bool
GpioConfiguration::ExportFilename(const std::string& filename_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigExportFilenamePath);
  if (this->Put(filename_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
GpioConfiguration::UnexportFilename() const
{
  std::string filename;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigUnexportFilenamePath);
  this->Get(filename, path);
  return (filename);
}

bool
GpioConfiguration::UnexportFilename(const std::string& filename_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigUnexportFilenamePath);
  if (this->Put(filename_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
GpioConfiguration::DirectionFilename() const
{
  std::string filename;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigDirectionFilenamePath);
  this->Get(filename, path);
  return (filename);
}

bool
GpioConfiguration::DirectionFilename(const std::string& filename_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigDirectionFilenamePath);
  if (this->Put(filename_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
GpioConfiguration::Direction() const
{
  std::string dir;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigDirectionValuePath);
  this->Get(dir, path);
  return (dir);
}

bool
GpioConfiguration::Direction(const std::string& dir_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigDirectionValuePath);
  if (this->Put(dir_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
GpioConfiguration::StateFilename() const
{
  std::string filename;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigStateFilenamePath);
  this->Get(filename, path);
  return (filename);
}

bool
GpioConfiguration::StateFilename(const std::string& filename_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigStateFilenamePath);
  if (this->Put(filename_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
GpioConfiguration::State() const
{
  std::string state;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigStateValuePath);
  this->Get(state, path);
  return (state);
}

bool
GpioConfiguration::State(const std::string& state_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigStateValuePath);
  if (this->Put(state_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
GpioConfiguration::EdgeFilename() const
{
  std::string filename;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigEdgeFilenamePath);
  this->Get(filename, path);
  return (filename);
}

bool
GpioConfiguration::EdgeFilename(const std::string& filename_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigEdgeFilenamePath);
  if (this->Put(filename_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
GpioConfiguration::Edge() const
{
  std::string dir;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigEdgeValuePath);
  this->Get(dir, path);
  return (dir);
}

bool
GpioConfiguration::Edge(const std::string& edge_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(GpioConfiguration::ConfigRoot,
      GpioConfiguration::ConfigEdgeValuePath);
  if (this->Put(edge_, path))
  {
    status = this->Commit();
  }
  return (status);
}

}
}
