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
#include <zutils/zConf.h>

namespace zUtils
{
namespace zConf
{

//**********************************************************************
// Class: ConfigurationHandler
//**********************************************************************

ConfigurationHandler::ConfigurationHandler () :
    _connector (NULL)
{
}

ConfigurationHandler::~ConfigurationHandler ()
{
}

bool
ConfigurationHandler::Connect (ConfigurationConnector *connector_)
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
ConfigurationHandler::Load ()
{
  bool status = false;
  zData::Data data;
  if (this->_connector && this->_connector->Load (data))
  {
    data.DisplayJson();
    if(data.Key() == Configuration::ROOT)
    {
      this->_working.Put(data);
      this->_working.Commit();
      status = true;
    }
  }
  return (status);
}

bool
ConfigurationHandler::Store ()
{
  bool status = false;
  zData::Data tmp;
  if (this->_connector && this->_working.Get(tmp))
  {
    status = this->_connector->Store (tmp);
  }
  return (status);
}

bool
ConfigurationHandler::Get (Configuration &conf_, const std::string &path_)
{
  return (this->_working.Get(conf_, path_));
}

bool
ConfigurationHandler::Set (Configuration &conf_, const std::string &path_)
{
  return (this->_working.Put(conf_, path_));
}

}
}
