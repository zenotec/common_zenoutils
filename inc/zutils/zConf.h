//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

#ifndef _ZCONF_H_
#define _ZCONF_H_

namespace zUtils
{
namespace zConf
{

class ConfigurationConnector;

//**********************************************************************
// Class: ConfigurationEvent
//**********************************************************************

class ConfigurationEvent : public zEvent::Event
{
public:

  enum EVENTID
  {
    EVENTID_ERR = -1,
    EVENTID_NONE = 0,
    EVENTID_UPDATE,
    EVENTID_COMMIT,
    EVENTID_LAST
  };

  ConfigurationEvent(ConfigurationEvent::EVENTID id_);

  virtual
  ~ConfigurationEvent();

protected:

private:

};

//**********************************************************************
// Class: Configuration
//**********************************************************************

class Configuration
{

public:

  static const std::string ROOT;

  Configuration();

  Configuration(Configuration &other_);

  Configuration(const Configuration &other_);

  Configuration(zData::Data &data_);

  virtual
  ~Configuration();

  bool
  operator ==(const Configuration &other_) const;

  bool
  operator !=(const Configuration &other_) const;

  std::unique_ptr<Configuration>
  operator [](const std::string &key_);

  void
  Display()
  {
    this->_working.DisplayJson();
  }

  void
  RegisterEvents(zEvent::EventHandler &handler_);

  void
  UnregisterEvents(zEvent::EventHandler &handler_);

  bool
  IsModified();

  bool
  Commit();

  bool
  Restore();

  template<typename T>
    bool
    Get(T &value_, const std::string &path_ = std::string("")) const
        {
//      std::cout << "Getting configuration value: " << path_ << std::endl;
      return (this->_working.Get<T>(value_, path_));
    }

  bool
  Get(zData::Data &data_, const std::string &path_ = std::string("")) const;

  bool
  Get(Configuration &conf_, const std::string &path_ = std::string("")) const;

  template<typename T>
    bool
    Put(T &value_, const std::string &path_ = std::string(""))
    {
//      std::cout << "Putting configuration value: " << path_ << std::endl;
      return (this->_staging.Put<T>(value_, path_));
    }

  bool
  Put(zData::Data &data_, const std::string &path_ = std::string(""));

  bool
  Put(Configuration &conf_, const std::string &path_ = std::string(""));

  template<typename T>
    bool
    Add(T &value_, const std::string &path_ = std::string(""))
    {
//      std::cout << "Adding configuration value: " << path_ << std::endl;
      return (this->_staging.Add<T>(value_, path_));
    }

  bool
  Add(zData::Data &data_, const std::string &path_ = std::string(""));

  bool
  Add(Configuration &conf_, const std::string &path_ = std::string(""));

protected:

private:

  Configuration(const zData::Data &data_); // Not supported; Cannot ensure thread safeness

  mutable std::mutex _lock;

  bool _modified;

  zData::Data _staging;
  zData::Data _working;

  ConfigurationEvent _update_event;
  ConfigurationEvent _commit_event;

};

//**********************************************************************
// Class: ConfigurationConnector
//**********************************************************************

class ConfigurationConnector
{

public:

  virtual bool
  Load(zData::Data &data_) = 0;

  virtual bool
  Store(zData::Data &data_) = 0;

protected:

private:

};

//**********************************************************************
// Class: ConfigurationFileConnector
//**********************************************************************

class ConfigurationFileConnector : public ConfigurationConnector
{
public:

  ConfigurationFileConnector(const std::string &filename_);

  virtual
  ~ConfigurationFileConnector();

  virtual bool
  Load(zData::Data &data_);

  virtual bool
  Store(zData::Data &data_);

protected:

private:

  std::string _filename;

};

//**********************************************************************
// Class: ConfigurationHandler
//**********************************************************************

class ConfigurationHandler : public zEvent::EventHandler
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

  ConfigurationHandler();

  virtual
  ~ConfigurationHandler();

  bool
  Connect(ConfigurationConnector *connector_);

  bool
  Load();

  bool
  Store();

  bool
  Get(Configuration &data_, const std::string &path_ = std::string(""));

  bool
  Set(Configuration &data_, const std::string &path_ = std::string(""));

protected:

private:

  ConfigurationConnector *_connector;
  Configuration _working;

};

//**********************************************************************
// Class: ConfigurationManager
//**********************************************************************

class ConfigurationManager : public ConfigurationHandler
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

#endif /* _ZCONF_H_ */
