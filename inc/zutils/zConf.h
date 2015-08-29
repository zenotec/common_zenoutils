//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zConf.h
//    Description:
//
//*****************************************************************************

#ifndef __ZCONF_H__
#define __ZCONF_H__

#include <string>
#include <list>

#include <zutils/zData.h>

namespace zUtils
{
namespace zConf
{

class Observer;
class Connector;

class Data: private zData::Data
{
    static const std::string ROOT;

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

    Data();

    virtual
    ~Data();

    bool
    operator ==( const Data &other_ ) const;

    bool
    operator !=( const Data &other_ ) const;

    bool
    Get( const std::string &key_, std::string &value_ );

    bool
    Get( zData::Data &data_ );

    bool
    Set( const std::string &key_, std::string &value_ );

    bool
    Set( zData::Data &data_ );

    bool
    Connect( zConf::Connector *connector_);

    bool
    Load();

    bool
    Store();

    bool
    Register( zConf::Observer *obs_ );

    bool
    Unregister( zConf::Observer *obs_ );

protected:

private:

    void
    _notify( zConf::Data::STATE state_ );

    std::list<zConf::Observer *> _obsList;

    zConf::Connector *_connector;

};

class Observer
{

public:

    virtual void
    Handle( Data::STATE state_, zData::Data &item_ ) = 0;

protected:

private:

};

class Connector
{

public:

    virtual bool
    Load( zConf::Data &data_ ) = 0;

    virtual bool
    Store( zConf::Data &data_ ) = 0;

protected:

private:

};

class FileConnector: public Connector
{
public:

    FileConnector( const std::string &filename_ );

    virtual
    ~FileConnector();

    virtual bool
    Load( zConf::Data &data_ );

    virtual bool
    Store( zConf::Data &data_ );

protected:

private:

    std::string _filename;

};

class Handler
{

public:

    Handler();

    virtual
    ~Handler();

protected:

private:

    std::list<zConf::Data *> _obsList;

};

}
}

#endif /* __ZCONF_H__ */
