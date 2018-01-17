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

#ifndef __ZCONFIG_H__
#define __ZCONFIG_H__

#include <string>

#include <zutils/zCompatibility.h>
#include <zutils/zSem.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>

namespace zUtils
{
namespace zConfig
{

class ConfigConnector;

//**********************************************************************
// Class: ConfigNotification
//**********************************************************************

class ConfigNotification : public zEvent::EventNotification
{
public:

  enum ID
  {
    ID_ERR = -1,
    ID_NONE = 0,
    ID_PRELOAD,
    ID_LOAD,
    ID_POSTLOAD,
    ID_PREUPDATE,
    ID_UPDATE,
    ID_POSTUPDATE,
    ID_PRECOMMIT,
    ID_COMMIT,
    ID_POSTCOMMIT,
    ID_LAST
  };

  ConfigNotification(ConfigNotification::ID id_);

  virtual
  ~ConfigNotification();

  ConfigNotification::ID
  Id();

protected:

private:

  ConfigNotification::ID _id;

};

//**********************************************************************
// Class: ConfigPath
//**********************************************************************

class ConfigPath : public zData::DataPath
{

public:

  static const std::string ConfigRoot;

  ConfigPath(const std::string& root_ = std::string(""));

  ConfigPath(zData::DataPath& path_);

  ConfigPath(const zData::DataPath& path_);

  virtual
  ~ConfigPath();

  zData::DataPath&
  GetDataPath();

  const zData::DataPath&
  GetDataPath() const;

  ConfigPath&
  GetConfigPath();

  const ConfigPath&
  GetConfigPath() const;

protected:

private:

};

//**********************************************************************
// Class: ConfigData
//**********************************************************************

class ConfigData : public zData::Data
{

public:

  ConfigData(const std::string& path_ = std::string(""));

  ConfigData(ConfigPath& path_);

  ConfigData(zData::Data& data_);

  ConfigData(const zData::Data& data_);

  virtual
  ~ConfigData();

  ConfigData&
  operator=(zData::Data& data_);

  ConfigData&
  operator=(const zData::Data& data_);

  zData::Data&
  GetData();

  const zData::Data&
  GetData() const;

  ConfigData&
  GetConfigData();

  const ConfigData&
  GetConfigData() const;

protected:

private:

};

//**********************************************************************
// Class: ConfigurationConnector
//**********************************************************************

class ConfigConnector
{

public:

  ConfigConnector()
  {
  }

  virtual
  ~ConfigConnector()
  {
  }

  virtual bool
  Load(ConfigData &data_) = 0;

  virtual bool
  Store(ConfigData &data_) = 0;

protected:

private:

};

//**********************************************************************
// Class: ConfigurationFileConnector
//**********************************************************************

class ConfigurationFileConnector : public ConfigConnector
{
public:

  ConfigurationFileConnector(const std::string &filename_);

  virtual
  ~ConfigurationFileConnector();

  virtual bool
  Load(ConfigData &data_);

  virtual bool
  Store(ConfigData &data_);

protected:

private:

  std::string _filename;

};

//**********************************************************************
// Class: Configuration
//**********************************************************************

class Configuration : public zEvent::Event
{

public:

  enum STATE
  {
    STATE_ERR = -1,
    STATE_NONE = 0,
    STATE_PREGET = 1,
    STATE_PRESET = 2,
    STATE_POSTGET = 3,
    STATE_POSTSET = 4,
    STATE_PRECOMMIT = 5,
    STATE_POSTCOMMIT = 6,
    STATE_LAST
  };

  Configuration();

  Configuration(Configuration &other_);

  Configuration(const Configuration &other_);

  Configuration(ConfigData &data_);

  Configuration(const ConfigData &data_);

  virtual
  ~Configuration();

  bool
  operator ==(const Configuration &other_) const;

  bool
  operator !=(const Configuration &other_) const;

  UNIQUE_PTR(Configuration)operator [](const std::string &key_);

  ConfigPath&
  GetConfigPath()
  {
    return (this->_path);
  }

  bool
  Connect(ConfigConnector *connector_);

  bool
  Disconnect();

  bool
  Load();

  bool
  Store();

  bool
  IsModified() const;

  bool
  Commit();

  bool
  Restore();

  bool
  Get(ConfigData& child_) const;

  bool
  Get(ConfigPath& src_, ConfigData& child_) const;

  bool
  Get(ConfigPath& src_, ConfigPath& dst_, ConfigData& child_) const;

  template<typename T>
  bool
  Get(ConfigPath path_, T& val_) const
  {
    bool status = false;

    // Begin critical section
    if (this->_lock.Lock())
    {
      status = this->_working.GetValue<T>(path_.GetDataPath(), val_);
      // End critical section
      this->_lock.Unlock();
    }

    // Return status
    return (status);
  }

  bool
  Put(const ConfigData& child_);

  bool
  Put(const ConfigPath& dst_, const ConfigData& child_);

  bool
  Put(const ConfigPath& dst_, const ConfigPath& src_, const ConfigData& child_);

  template<typename T>
  bool
  Put(const ConfigPath& path_, T &value_)
  {
    bool status = false;

    // Begin critical section
    if (this->_lock.Lock())
    {
      if (this->_staging.PutValue<T>(path_.GetDataPath(), value_))
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
  Add(const ConfigData& child_);

  bool
  Add(const ConfigPath& dst_, const ConfigData& child_);

  bool
  Add(const ConfigPath& dst_, const ConfigPath& src_, const ConfigData& child_);

  template<typename T>
  bool
  Add(const ConfigPath& path_, T &value_)
  {
    bool status = false;

    // Begin critical section
    if (this->_lock.Lock())
    {
      if (this->_staging.AddValue<T>(path_.GetDataPath(), value_))
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
  Display(const std::string& prefix_ = std::string("")) const;

protected:

private:

  mutable zSem::Mutex _lock;

  bool _modified;

  ConfigConnector *_connector;

  ConfigPath _path;
  ConfigData _staging;
  ConfigData _working;

};

//**********************************************************************
// Class: ConfigurationManager
//**********************************************************************

class ConfigurationManager : public zEvent::EventHandler, public Configuration
{
public:

  static ConfigurationManager&
  Instance()
  {
    static ConfigurationManager instance;
    return instance;
  }

protected:

private:

  ConfigurationManager()
  {
  }

  ConfigurationManager(ConfigurationManager const&);

  void
  operator=(ConfigurationManager const&);

};

}
}

#endif /* __ZCONFIG_H__ */
