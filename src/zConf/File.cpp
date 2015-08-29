//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: File.cpp
//    Description:
//
//*****************************************************************************

#include <zutils/zConf.h>

namespace zUtils
{
namespace zConf
{

FileConnector::FileConnector( const std::string &filename_ ) :
        _filename( filename_ )
{
}

FileConnector::~FileConnector()
{
}

bool
FileConnector::Load( zConf::Data &data_ )
{
    bool status = false;
    std::fstream fs;
    std::stringstream json;

    ZLOG_INFO( "zConf::FileConnector::Load: Reading configuration file: " + this->_filename );

    // Open configuration file and read into local string
    fs.open( this->_filename.c_str(), std::fstream::in );
    if (fs.is_open())
    {
        // Read file contents into buffer
        json << fs.rdbuf();
        fs.close();

        zData::Data conf_data;
        status = conf_data.SetJson(json.str());

        // Update the configuration data
        if (status)
        {
            status = data_.Set(conf_data);
        }
    }

    // Return status
    return (status);

}

bool
FileConnector::Store( zConf::Data &data_ )
{
    bool status = false;
    std::fstream fs;

    ZLOG_INFO( "zConf::FileConnector::Store: Writing configuration file: " + this->_filename );

    // Open configuration file and write configuration data
    fs.open( this->_filename.c_str(), std::fstream::out );
    if (fs.is_open())
    {
        zData::Data conf_data;
        data_.Get(conf_data);
        fs << conf_data.GetJson();
        fs.flush();
        fs.close();
        status = true;
    }

    // Return status
    return (status);

}

}
}
