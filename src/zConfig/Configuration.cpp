//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

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
// Class: Configuration
//**********************************************************************

const std::string Configuration::ROOT = "zConfig";

Configuration::Configuration() :
    _modified(false), _staging(Configuration::ROOT),
        _working(Configuration::ROOT), _event(zEvent::Event::TYPE_CONFIG)
{
}

Configuration::Configuration(Configuration &other_) :
    _modified(false), _staging(other_._staging),
        _working(other_._working), _event(zEvent::Event::TYPE_CONFIG)
{
}

Configuration::Configuration(const Configuration &other_) :
    _modified(false), _staging(other_._staging),
        _working(other_._working), _event(zEvent::Event::TYPE_CONFIG)
{
}

Configuration::Configuration(zData::Data &data_) :
    _modified(false), _staging(Configuration::ROOT),
        _working(Configuration::ROOT), _event(zEvent::Event::TYPE_CONFIG)
{
  this->_lock.lock();
  this->_staging.Put(data_, data_.Key());
  this->_working = this->_staging;
  this->_lock.unlock();
}

Configuration::~Configuration()
{
  this->_lock.lock();
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

void
Configuration::RegisterEvents(zEvent::EventHandler &handler_)
{
  handler_.RegisterEvent(&this->_event);
}

void
Configuration::UnregisterEvents(zEvent::EventHandler &handler_)
{
  handler_.UnregisterEvent(&this->_event);
}

bool
Configuration::IsModified() const
{
  bool mod = false;

  // Begin critical section
  this->_lock.lock();
  mod = this->_modified;
  // End critical section
  this->_lock.unlock();
  return (mod);
}

bool
Configuration::Commit()
{
  bool status = false;

  // Begin critical section
  this->_lock.lock();

  this->_working = this->_staging;
  status = (this->_working == this->_staging);
  this->_modified = false;

  // End critical section
  this->_lock.unlock();

  // Return status
  return (status);
}

bool
Configuration::Restore()
{
  bool status = false;

  // Begin critical section
  this->_lock.lock();

  this->_staging = this->_working;
  status = (this->_working == this->_staging);
  this->_modified = false;

  // End critical section
  this->_lock.unlock();

  // Return status
  return (status);
}

bool
Configuration::Get(zData::Data &data_, const std::string &path_) const
    {
  bool status = false;

  // Begin critical section
  this->_lock.lock();

//  std::cout << "Getting configuration data: " << path_ << std::endl;

  status = this->_working.Get(data_, path_);

//  data_.DisplayJson ();

// End critical section
  this->_lock.unlock();

  // Return status
  return (status);
}

bool
Configuration::Get(Configuration &conf_, const std::string &path_) const
    {
  bool status = false;
  zData::Data data;

//  std::cout << "Getting configuration: " << path_ << std::endl;

  if (this->Get(data, path_) && conf_.Put(data, zData::Data::PathLast(path_)))
  {
    status = conf_.Commit();
  }
  return (status);
}

bool
Configuration::Put(zData::Data &data_, const std::string &path_)
{
  bool status = false;

  // Begin critical section
  this->_lock.lock();

//  std::cout << "Putting configuration data: " << path_ << std::endl;

  status = this->_staging.Put(data_, path_);
  this->_modified = true;

//  this->_staging.DisplayJson ();

// End critical section
  this->_lock.unlock();

  // Return status
  return (status);
}

bool
Configuration::Put(Configuration &conf_, const std::string &path_)
{
  bool status = false;
  zData::Data data;

//  std::cout << "Putting configuration: " << path_ << std::endl;

  if (conf_.Get(data, path_) && this->Put(data, path_))
  {
    status = this->Commit();
  }
  return (status);
}

bool
Configuration::Add(zData::Data &data_, const std::string &path_)
{
  bool status = false;

  // Begin critical section
  this->_lock.lock();

//  std::cout << "Adding configuration data: " << path_ << std::endl;

  status = this->_staging.Add(data_, path_);
  this->_modified = true;

//  this->_staging.DisplayJson ();

// End critical section
  this->_lock.unlock();

  // Return status
  return (status);
}

bool
Configuration::Add(Configuration &conf_, const std::string &path_)
{
  bool status = false;
  zData::Data data;

//  std::cout << "Adding configuration: " << path_ << std::endl;

  if (conf_.Get(data, path_) && this->Add(data, path_))
  {
    status = this->Commit();
  }
  return (status);
}

//**********************************************************************
// Class: ConfigurationNotification
//**********************************************************************

ConfigurationNotification::ConfigurationNotification(ConfigurationNotification::ID id_) :
    _id(id_)
{
}

ConfigurationNotification::~ConfigurationNotification()
{
}

ConfigurationNotification::ID
ConfigurationNotification::Id()
{
  return (this->_id);
}

}
}
