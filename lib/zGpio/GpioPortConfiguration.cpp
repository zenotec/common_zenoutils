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

#include <string>
#include <list>
#include <mutex>

#include <zutils/zSem.h>
#include <zutils/zLog.h>
#include <zutils/zEvent.h>
#include <zutils/zThread.h>
#include <zutils/zData.h>
#include <zutils/zConfig.h>
#include <zutils/zSwitch.h>

#include <zutils/zGpio.h>

namespace zUtils
{
namespace zGpio
{

//**********************************************************************
// Class: GpioConfigPath
//**********************************************************************

const std::string GpioConfigPath::ConfigRoot("zGpio");

const std::string GpioConfigPath::ConfigIdentifierPath("Identifier");
const std::string GpioConfigPath::ConfigExportFilenamePath("ExportFilename");
const std::string GpioConfigPath::ConfigUnexportFilenamePath("UnexportFilename");
const std::string GpioConfigPath::ConfigDirectionFilenamePath("DirectionFilename");
const std::string GpioConfigPath::ConfigDirectionValuePath("Direction");
const std::string GpioConfigPath::ConfigStateFilenamePath("StateFilename");
const std::string GpioConfigPath::ConfigStateValuePath("State");
const std::string GpioConfigPath::ConfigEdgeFilenamePath("EdgeFilename");
const std::string GpioConfigPath::ConfigEdgeValuePath("Edge");

GpioConfigPath::GpioConfigPath() :
    zConfig::ConfigPath(ConfigRoot)
{
}

GpioConfigPath::~GpioConfigPath()
{
}

zConfig::ConfigPath
GpioConfigPath::Identifier() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigIdentifierPath);
  return (path);
}

zConfig::ConfigPath
GpioConfigPath::ExportFilename() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigExportFilenamePath);
  return (path);
}

zConfig::ConfigPath
GpioConfigPath::UnexportFilename() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigUnexportFilenamePath);
  return (path);
}

zConfig::ConfigPath
GpioConfigPath::DirectionFilename() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigDirectionFilenamePath);
  return (path);
}

zConfig::ConfigPath
GpioConfigPath::Direction() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigDirectionValuePath);
  return (path);
}

zConfig::ConfigPath
GpioConfigPath::StateFilename() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigStateFilenamePath);
  return (path);
}

zConfig::ConfigPath
GpioConfigPath::State() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigStateValuePath);
  return (path);
}

zConfig::ConfigPath
GpioConfigPath::EdgeFilename() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigEdgeFilenamePath);
  return (path);
}

zConfig::ConfigPath
GpioConfigPath::Edge() const
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigEdgeValuePath);
  return (path);
}

//**********************************************************************
// Class: Configuration
//**********************************************************************

const int GpioConfigData::ConfigIdentifierDefault(0);

const std::string GpioConfigData::ConfigExportFilenameDefault("/sys/class/gpio/export");

const std::string GpioConfigData::ConfigUnexportFilenameDefault("/sys/class/gpio/unexport");

const std::string GpioConfigData::ConfigDirectionFilenameDefault(
    "/sys/class/gpio/gpio%d/direction");

const std::string GpioConfigData::ConfigDirectionValueIn("in");
const std::string GpioConfigData::ConfigDirectionValueOut("out");
const std::string GpioConfigData::ConfigDirectionValueDefault(ConfigDirectionValueIn);

const std::string GpioConfigData::ConfigStateFilenameDefault("/sys/class/gpio/gpio%d/value");

const std::string GpioConfigData::ConfigStateValueActive("1");
const std::string GpioConfigData::ConfigStateValueInactive("0");
const std::string GpioConfigData::ConfigStateValueDefault(ConfigStateValueInactive);

const std::string GpioConfigData::ConfigEdgeFilenameDefault("/sys/class/gpio/gpio%d/edge");

const std::string GpioConfigData::ConfigEdgeValueNone("none");
const std::string GpioConfigData::ConfigEdgeValueLoHi("rising");
const std::string GpioConfigData::ConfigEdgeValueHiLo("falling");
const std::string GpioConfigData::ConfigEdgeValueBoth("both");
const std::string GpioConfigData::ConfigEdgeValueDefault(ConfigEdgeValueNone);

GpioConfigData::GpioConfigData() :
    zConfig::ConfigData(GpioConfigPath::ConfigRoot)
{
}

GpioConfigData::GpioConfigData(zData::Data& data_) :
    zConfig::ConfigData(GpioConfigPath::ConfigRoot)
{
  this->PutChild(data_);
}

GpioConfigData::GpioConfigData(zConfig::ConfigData& config_) :
    zConfig::ConfigData(config_)
{
}

GpioConfigData::~GpioConfigData()
{
}

int
GpioConfigData::Identifier() const
{
  int id;
  GpioConfigPath path;
  if (!this->GetValue(path.Identifier(), id))
  {
    id = ConfigIdentifierDefault;
  }
  return (id);
}

bool
GpioConfigData::Identifier(const int id_)
{
  GpioConfigPath path;
  return (this->PutValue(path.Identifier(), id_));
}

std::string
GpioConfigData::ExportFilename() const
{
  std::string filename;
  GpioConfigPath path;
  if (!this->GetValue(path.ExportFilename(), filename))
  {
    filename = ConfigExportFilenameDefault;
  }
  return (filename);
}

bool
GpioConfigData::ExportFilename(const std::string& filename_)
{
  GpioConfigPath path;
  return (this->PutValue(path.ExportFilename(), filename_));
}

std::string
GpioConfigData::UnexportFilename() const
{
  std::string filename;
  GpioConfigPath path;
  if (!this->GetValue(path.UnexportFilename(), filename))
  {
    filename = ConfigUnexportFilenameDefault;
  }
  return (filename);
}

bool
GpioConfigData::UnexportFilename(const std::string& filename_)
{
  GpioConfigPath path;
  return (this->PutValue(path.UnexportFilename(), filename_));
}

std::string
GpioConfigData::DirectionFilename() const
{
  std::string filename;
  GpioConfigPath path;
  if (!this->GetValue(path.DirectionFilename(), filename))
  {
    filename = ConfigDirectionFilenameDefault;
  }
  return (filename);
}

bool
GpioConfigData::DirectionFilename(const std::string& filename_)
{
  GpioConfigPath path;
  return (this->PutValue(path.DirectionFilename(), filename_));
}

std::string
GpioConfigData::Direction() const
{
  std::string dir;
  GpioConfigPath path;
  if (!this->GetValue(path.Direction(), dir))
  {
    dir = ConfigDirectionValueDefault;
  }
  return (dir);
}

bool
GpioConfigData::Direction(const std::string& dir_)
{
  GpioConfigPath path;
  return (this->PutValue(path.Direction(), dir_));
}

std::string
GpioConfigData::StateFilename() const
{
  std::string filename;
  GpioConfigPath path;
  if (!this->GetValue(path.StateFilename(), filename))
  {
    filename = ConfigStateFilenameDefault;
  }
  return (filename);
}

bool
GpioConfigData::StateFilename(const std::string& filename_)
{
  GpioConfigPath path;
  return (this->PutValue(path.StateFilename(), filename_));
}

std::string
GpioConfigData::State() const
{
  std::string state;
  GpioConfigPath path;
  if (!this->GetValue(path.State(), state))
  {
    state = ConfigStateValueDefault;
  }
  return (state);
}

bool
GpioConfigData::State(const std::string& state_)
{
  GpioConfigPath path;
  return (this->PutValue(path.State(), state_));
}

std::string
GpioConfigData::EdgeFilename() const
{
  std::string filename;
  GpioConfigPath path;
  if (!this->GetValue(path.EdgeFilename(), filename))
  {
    filename = ConfigEdgeFilenameDefault;
  }
  return (filename);
}

bool
GpioConfigData::EdgeFilename(const std::string& filename_)
{
  GpioConfigPath path;
  return (this->PutValue(path.EdgeFilename(), filename_));
}

std::string
GpioConfigData::Edge() const
{
  std::string edge;
  GpioConfigPath path;
  if (!this->GetValue(path.Edge(), edge))
  {
    edge = ConfigEdgeValueDefault;
  }
  return (edge);
}

bool
GpioConfigData::Edge(const std::string& edge_)
{
  GpioConfigPath path;
  return (this->PutValue(path.Edge(), edge_));
}

}
}
