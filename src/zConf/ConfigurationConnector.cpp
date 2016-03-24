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
#include <memory>
#include <fstream>
#include <sstream>

#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConf.h>

namespace zUtils
{

namespace zConf
{

//**********************************************************************
// Class: ConfigurationFileConnector
//**********************************************************************

ConfigurationFileConnector::ConfigurationFileConnector (const std::string &filename_) :
    _filename (filename_)
{
}

ConfigurationFileConnector::~ConfigurationFileConnector ()
{
}

bool
ConfigurationFileConnector::Load (zData::Data &data_)
{
  bool status = false;
  std::fstream fs;
  std::stringstream json;

  // Open configuration file and read into local string
  fs.open (this->_filename.c_str (), std::fstream::in);
  if (fs.is_open ())
  {
    // Read file contents into buffer
    json << fs.rdbuf ();
    fs.close ();

    // Store to callers data object
    status = data_.SetJson (json.str ());
  }

  // Return status
  return (status);

}

bool
ConfigurationFileConnector::Store (zData::Data &data_)
{
  bool status = false;
  std::fstream fs;

  // Open configuration file and write configuration data
  fs.open (this->_filename.c_str (), std::fstream::out);
  if (fs.is_open ())
  {
    fs << data_.GetJson ();
    fs.flush ();
    fs.close ();
    status = true;
  }

  // Return status
  return (status);

}

}
}
