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
#include <memory>
#include <fstream>
#include <sstream>

#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>

namespace zUtils
{

namespace zConfig
{

//**********************************************************************
// Class: ConfigurationFileConnector
//**********************************************************************

ConfigurationFileConnector::ConfigurationFileConnector(const std::string &filename_) :
    _filename(filename_)
{
}

ConfigurationFileConnector::~ConfigurationFileConnector()
{
}

bool
ConfigurationFileConnector::Load(ConfigData &data_)
{
  bool status = false;
  std::fstream fs;
  std::stringstream json;

  // Open configuration file and read into local string
  fs.open(this->_filename.c_str(), std::fstream::in);
  if (fs.is_open())
  {
    // Read file contents into buffer
    json << fs.rdbuf();
    fs.close();

    // Store to callers configuration data object
    status = data_.SetJson(json.str());
  }

  // Return status
  return (status);

}

bool
ConfigurationFileConnector::Store(ConfigData &data_)
{
  bool status = false;
  std::fstream fs;

  // Open configuration file and write configuration data
  fs.open(this->_filename.c_str(), std::fstream::out);
  if (fs.is_open())
  {
    fs << data_.GetJsonPretty();
    fs.flush();
    fs.close();
    status = true;
  }

  // Return status
  return (status);

}

}
}
