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
#include <zutils/zConf.h>
#include <zutils/zSwitch.h>

#include <zutils/zGpio.h>

namespace zUtils
{
namespace zGpio
{

//**********************************************************************
// Class: Configuration
//**********************************************************************

const std::string Configuration::ConfigRoot("zGpio");

const std::string Configuration::ConfigIdentifierValuePath("Identifier");
const int Configuration::ConfigIdentifierValueDefault(0);

const std::string Configuration::ConfigExportFilenamePath("ExportFilename");
const std::string Configuration::ConfigExportFilenameDefault("/sys/class/gpio/export");

const std::string Configuration::ConfigUnexportFilenamePath("UnexportFilename");
const std::string Configuration::ConfigUnexportFilenameDefault("/sys/class/gpio/unexport");

const std::string Configuration::ConfigDirectionFilenamePath("DirectionFilename");
const std::string Configuration::ConfigDirectionFilenameDefault("/sys/class/gpio/gpio%d/direction");

const std::string Configuration::ConfigDirectionValuePath("Direction");
const std::string Configuration::ConfigDirectionValueIn("in");
const std::string Configuration::ConfigDirectionValueOut("out");
const std::string Configuration::ConfigDirectionValueDefault(Configuration::ConfigDirectionValueIn);

const std::string Configuration::ConfigStateFilenamePath("StateFilename");
const std::string Configuration::ConfigStateFilenameDefault("/sys/class/gpio/gpio%d/value");

const std::string Configuration::ConfigStateValuePath("State");
const std::string Configuration::ConfigStateValueActive("1");
const std::string Configuration::ConfigStateValueInactive("0");
const std::string Configuration::ConfigStateValueDefault(Configuration::ConfigStateValueInactive);

const std::string Configuration::ConfigEdgeFilenamePath("EdgeFilename");
const std::string Configuration::ConfigEdgeFilenameDefault("/sys/class/gpio/gpio%d/edge");

const std::string Configuration::ConfigEdgeValuePath("Edge");
const std::string Configuration::ConfigEdgeValueNone("none");
const std::string Configuration::ConfigEdgeValueLoHi("rising");
const std::string Configuration::ConfigEdgeValueHiLo("falling");
const std::string Configuration::ConfigEdgeValueBoth("both");
const std::string Configuration::ConfigEdgeValueDefault(Configuration::ConfigEdgeValueNone);

Configuration::Configuration()
{
  this->Identifier(ConfigIdentifierValueDefault);
  this->ExportFilename(ConfigExportFilenameDefault);
  this->UnexportFilename(ConfigUnexportFilenameDefault);
  this->DirectionFilename(ConfigDirectionFilenameDefault);
  this->Direction(ConfigDirectionValueDefault);
  this->StateFilename(ConfigStateFilenameDefault);
  this->State(ConfigStateValueDefault);
  this->EdgeFilename(ConfigEdgeFilenameDefault);
  this->Edge(ConfigEdgeValueDefault);
}

Configuration::Configuration(zData::Data& data_) :
    zConf::Configuration(data_)
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

Configuration::Configuration(zConf::Configuration& config_) :
    zConf::Configuration(config_)
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

Configuration::~Configuration()
{
}

zConf::Configuration&
Configuration::GetConfig()
{
  return (*this);
}

int
Configuration::Identifier() const
{
  int id;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigIdentifierValuePath);
  this->Get(id, path);
  return (id);
}

bool
Configuration::Identifier(const int id_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigIdentifierValuePath);
  if (this->Put(id_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
Configuration::ExportFilename() const
{
  std::string filename;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigExportFilenamePath);
  this->Get(filename, path);
  return (filename);
}

bool
Configuration::ExportFilename(const std::string& filename_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigExportFilenamePath);
  if (this->Put(filename_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
Configuration::UnexportFilename() const
{
  std::string filename;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigUnexportFilenamePath);
  this->Get(filename, path);
  return (filename);
}

bool
Configuration::UnexportFilename(const std::string& filename_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigUnexportFilenamePath);
  if (this->Put(filename_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
Configuration::DirectionFilename() const
{
  std::string filename;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigDirectionFilenamePath);
  this->Get(filename, path);
  return (filename);
}

bool
Configuration::DirectionFilename(const std::string& filename_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigDirectionFilenamePath);
  if (this->Put(filename_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
Configuration::Direction() const
{
  std::string dir;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigDirectionValuePath);
  this->Get(dir, path);
  return (dir);
}

bool
Configuration::Direction(const std::string& dir_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigDirectionValuePath);
  if (this->Put(dir_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
Configuration::StateFilename() const
{
  std::string filename;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigStateFilenamePath);
  this->Get(filename, path);
  return (filename);
}

bool
Configuration::StateFilename(const std::string& filename_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigStateFilenamePath);
  if (this->Put(filename_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
Configuration::State() const
{
  std::string state;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigStateValuePath);
  this->Get(state, path);
  return (state);
}

bool
Configuration::State(const std::string& state_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigStateValuePath);
  if (this->Put(state_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
Configuration::EdgeFilename() const
{
  std::string filename;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigEdgeFilenamePath);
  this->Get(filename, path);
  return (filename);
}

bool
Configuration::EdgeFilename(const std::string& filename_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigEdgeFilenamePath);
  if (this->Put(filename_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
Configuration::Edge() const
{
  std::string dir;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigEdgeValuePath);
  this->Get(dir, path);
  return (dir);
}

bool
Configuration::Edge(const std::string& edge_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(Configuration::ConfigRoot,
      Configuration::ConfigEdgeValuePath);
  if (this->Put(edge_, path))
  {
    status = this->Commit();
  }
  return (status);
}

}
}
