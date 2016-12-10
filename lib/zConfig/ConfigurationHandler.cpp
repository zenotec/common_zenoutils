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

#include <list>
#include <mutex>
#include <memory>

#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>

namespace zUtils
{
namespace zConfig
{

//**********************************************************************
// Class: ConfigurationHandler
//**********************************************************************

ConfigurationHandler::ConfigurationHandler() :
    _connector(NULL)
{
}

ConfigurationHandler::~ConfigurationHandler()
{
}

bool
ConfigurationHandler::Connect(ConfigurationConnector *connector_)
{
  bool status = false;
  if (connector_)
  {
    this->_connector = connector_;
    status = true;
  }
  return (status);
}

bool
ConfigurationHandler::Load()
{
  bool status = false;
  zData::Data data;
  if (this->_connector && this->_connector->Load(data))
  {
    //data.DisplayJson();
    if (data.Key() == Configuration::ROOT)
    {
      this->_working.Put(data);
      this->_working.Commit();
      status = true;
    }
  }
  return (status);
}

bool
ConfigurationHandler::Store()
{
  bool status = false;
  zData::Data tmp;
  if (this->_connector && this->_working.Get(tmp))
  {
    status = this->_connector->Store(tmp);
  }
  return (status);
}

bool
ConfigurationHandler::Commit()
{
  return (this->_working.Commit());
}

bool
ConfigurationHandler::Get(Configuration &conf_, const std::string &path_)
{
  return (this->_working.Get(conf_, path_));
}

bool
ConfigurationHandler::Put(Configuration &conf_, const std::string &path_)
{
  return (this->_working.Put(conf_, path_));
}

}
}
