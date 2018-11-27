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

#include <zutils/zLog.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_CONFIG);

namespace zUtils
{
namespace zConfig
{

//**********************************************************************
// Class: Configuration
//**********************************************************************

Configuration::Configuration() :
    zEvent::Event(zEvent::TYPE_CONFIG), _lock(zSem::Mutex::LOCKED),
        _modified(false), _connector(NULL)

{
  this->_lock.Unlock();
}

Configuration::Configuration(ConfigData &data_) :
    zEvent::Event(zEvent::TYPE_CONFIG), _lock(zSem::Mutex::LOCKED),
        _modified(false), _connector(NULL), _staging(data_), _working(data_)
{
  this->_lock.Unlock();
}

Configuration::Configuration(const ConfigData &data_) :
    zEvent::Event(zEvent::TYPE_CONFIG), _lock(zSem::Mutex::LOCKED),
        _modified(false), _connector(NULL), _staging(data_), _working(data_)
{
  this->_lock.Unlock();
}

Configuration::Configuration(Configuration &other_) :
    zEvent::Event(zEvent::TYPE_CONFIG), _lock(zSem::Mutex::LOCKED),
        _modified(other_._modified), _connector(NULL), _staging(other_._staging),
        _working(other_._working)
{
  this->_lock.Unlock();
}

Configuration::Configuration(const Configuration &other_) :
    zEvent::Event(zEvent::TYPE_CONFIG), _lock(zSem::Mutex::LOCKED),
        _modified(other_._modified), _connector(NULL), _staging(other_._staging),
        _working(other_._working)
{
  this->_lock.Unlock();
}

Configuration::~Configuration()
{
}

bool
Configuration::operator ==(const Configuration &other_) const
    {
  bool status = false;
  status = (this->_working == other_._working);
  return (status);
}

bool
Configuration::operator !=(const Configuration &other_) const
    {
  bool status = false;
  status = (this->_working != other_._working);
  return (status);
}

bool
Configuration::Connect(ConfigConnector *connector_)
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
Configuration::Disconnect()
{
  bool status = false;
  if (this->_connector)
  {
    this->_connector = NULL;
    status = true;
  }
  return (status);
}

bool
Configuration::Load()
{
  bool status = false;

  ZLOG_INFO("Loading configuration");

  if (this->_connector)
  {
    status = this->_connector->Load(this->_staging);
  }
  return (status);
}

bool
Configuration::Store()
{
  bool status = false;

  ZLOG_INFO("Storing configuration");

  if (this->_connector)
  {
    status = this->_connector->Store(this->_working);
    status = true;
  }
  return (status);
}

bool
Configuration::IsModified() const
{
  bool mod = false;

  // Begin critical section
  if (this->_lock.Lock())
  {
    mod = this->_modified;
    this->_lock.Unlock();
  }
  return (mod);
}

bool
Configuration::Commit()
{
  bool status = false;

  ZLOG_INFO("Committing configuration");

  // Begin critical section
  if (this->_lock.Lock())
  {
    this->_working = this->_staging;
    if (this->_working == this->_staging)
    {
      this->_modified = false;
      status = true;
    }
    this->_lock.Unlock();
  }

  // Return status
  return (status);
}

bool
Configuration::Restore()
{
  bool status = false;

  ZLOG_INFO("Restoring configuration");

  // Begin critical section
  if (this->_lock.Lock())
  {
    this->_staging = this->_working;
    if (this->_working == this->_staging)
    {
      this->_modified = false;
      status = true;
    }
    this->_lock.Unlock();
  }

  // Return status
  return (status);
}

bool
Configuration::Get(ConfigData& child_) const
{
  bool status = false;

  ZLOG_DEBUG(std::string("Getting configuration: ") + this->_working.Path());

  // Begin critical section
  if (this->_lock.Lock())
  {
    status = this->_working.GetChild(child_);
    // End critical section
    this->_lock.Unlock();
  }

  // Return status
  return (status);
}

bool
Configuration::Get(ConfigPath& src_, ConfigData& child_) const
{
  bool status = false;

  ZLOG_DEBUG(std::string("Getting configuration: ") + src_.Path());

  // Begin critical section
  if (this->_lock.Lock())
  {
    status = this->_working.GetChild(src_, child_);
    // End critical section
    this->_lock.Unlock();
  }

  // Return status
  return (status);
}

bool
Configuration::Get(ConfigPath& src_, ConfigPath& dst_, ConfigData& child_) const
{
  bool status = false;

  ZLOG_DEBUG(std::string("Getting configuration: ") + src_.Path());

  // Begin critical section
  if (this->_lock.Lock())
  {
    status = this->_working.GetChild(src_, dst_, child_);
    // End critical section
    this->_lock.Unlock();
  }

  // Return status
  return (status);
}

bool
Configuration::Put(const ConfigData& child_)
{
  bool status = false;

  ZLOG_DEBUG(std::string("Putting configuration: ") + child_.Path());

  // Begin critical section
  if (this->_lock.Lock())
  {
    if (this->_staging.PutChild(child_))
    {
      this->_modified = true;
      status = true;
    }
    // End critical section
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

bool
Configuration::Put(const ConfigPath& dst_, const ConfigData& child_)
{
  bool status = false;

  ZLOG_DEBUG(std::string("Putting configuration: ") + dst_.Path());

  // Begin critical section
  if (this->_lock.Lock())
  {
    if (this->_staging.PutChild(dst_, child_))
    {
      this->_modified = true;
      status = true;
    }
    // End critical section
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

bool
Configuration::Put(const ConfigPath& dst_, const ConfigPath& src_, const ConfigData& child_)
{
  bool status = false;

  ZLOG_DEBUG(std::string("Putting configuration: ") + dst_.Path());

  // Begin critical section
  if (this->_lock.Lock())
  {
    if (this->_staging.PutChild(dst_, src_, child_))
    {
      this->_modified = true;
      status = true;
    }
    // End critical section
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

bool
Configuration::Add(const ConfigData& child_)
{
  bool status = false;

  ZLOG_DEBUG(std::string("Adding configuration: ") + child_.Path());

  // Begin critical section
  if (this->_lock.Lock())
  {
    if (this->_staging.AddChild(child_))
    {
      this->_modified = true;
      status = true;
    }
    // End critical section
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

bool
Configuration::Add(const ConfigPath& dst_, const ConfigData& child_)
{
  bool status = false;

  ZLOG_DEBUG(std::string("Adding configuration: ") + dst_.Path());

  // Begin critical section
  if (this->_lock.Lock())
  {
    if (this->_staging.AddChild(dst_, child_))
    {
      this->_modified = true;
      status = true;
    }
    // End critical section
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

bool
Configuration::Add(const ConfigPath& dst_, const ConfigPath& src_, const ConfigData& child_)
{
  bool status = false;

  ZLOG_DEBUG(std::string("Adding configuration: ") + dst_.Path());

  // Begin critical section
  if (this->_lock.Lock())
  {
    if (this->_staging.AddChild(dst_, src_, child_))
    {
      this->_modified = true;
      status = true;
    }
    // End critical section
    this->_lock.Unlock();
  }

  // Return status
  return (status);

}

void
Configuration::Display(const std::string& prefix_) const
{
  std::cout << prefix_ << "Staging: " << std::endl;
  this->_staging.DisplayPath();
  this->_staging.DisplayJson();
  std::cout << prefix_ << "Working: " << std::endl;
  this->_working.DisplayPath();
  this->_working.DisplayJson();
}

}
}
