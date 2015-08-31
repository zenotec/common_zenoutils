//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Data.cpp
//    Description:
//
//*****************************************************************************

#include <zutils/zConf.h>

namespace zUtils
{
namespace zConf
{

const std::string Data::ROOT = "zConf";

Data::Data() :
        _connector( 0 ), zData::Data( zConf::Data::ROOT )
{
}

Data::~Data()
{
}

bool
Data::operator ==( const Data &other_ ) const
{
    return (this->_pt == other_._pt);
}

bool
Data::operator !=( const Data &other_ ) const
{
    return (this->_pt != other_._pt);
}

bool
Data::Get( const std::string &key_, std::string &value_ )
{
    bool status = false;
    this->_notify( zConf::Data::STATE_PREGET );
    status = this->GetValue( key_, value_ );
    this->_notify( zConf::Data::STATE_POSTGET );
    return (status);
}

bool
Data::Get( const std::string &key_, int &value_ )
{
    bool status = false;
    this->_notify( zConf::Data::STATE_PREGET );
    status = this->GetValue( key_, value_ );
    this->_notify( zConf::Data::STATE_POSTGET );
    return (status);
}

bool
Data::Get( const std::string &key_, unsigned int &value_ )
{
    bool status = false;
    this->_notify( zConf::Data::STATE_PREGET );
    status = this->GetValue( key_, value_ );
    this->_notify( zConf::Data::STATE_POSTGET );
    return (status);
}

bool
Data::Get( zData::Data &data_ )
{
    bool status = false;
    this->_notify( zConf::Data::STATE_PREGET );
    status = data_.SetJson( this->GetJson() );
    this->_notify( zConf::Data::STATE_POSTGET );
    return (status);
}

bool
Data::Set( const std::string &key_, const std::string &value_ )
{
    bool status = false;
    this->_notify( zConf::Data::STATE_PRESET );
    status = this->SetValue( key_, value_ );
    this->_notify( zConf::Data::STATE_POSTSET );
    return (status);
}

bool
Data::Set( const std::string &key_, const int &value_ )
{
    bool status = false;
    this->_notify( zConf::Data::STATE_PRESET );
    status = this->SetValue( key_, value_ );
    this->_notify( zConf::Data::STATE_POSTSET );
    return (status);
}

bool
Data::Set( const std::string &key_, const unsigned int &value_ )
{
    bool status = false;
    this->_notify( zConf::Data::STATE_PRESET );
    status = this->SetValue( key_, value_ );
    this->_notify( zConf::Data::STATE_POSTSET );
    return (status);
}

bool
Data::Set( zData::Data &data_ )
{
    bool status = false;

    if (data_.GetKey() == zConf::Data::ROOT)
    {
        this->_notify( zConf::Data::STATE_PRESET );
        zData::Data::SetJson( data_.GetJson() );
        this->_notify( zConf::Data::STATE_POSTSET );
        status = true;
    }

    // Return status
    return (status);
}

bool
Data::Connect( zConf::Connector *connector_ )
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
Data::Load()
{
    bool status = false;
    if (this->_connector)
    {
        status = this->_connector->Load( *this );
    }
    return (status);
}

bool
Data::Commit()
{
    bool status = false;
    this->_notify( zConf::Data::STATE_PRECOMMIT );
    if (this->_connector)
    {
        status = this->_connector->Store( *this );
    }
    this->_notify( zConf::Data::STATE_POSTCOMMIT);
    return (status);
}

bool
Data::Register( Observer *obs_ )
{
    if (!obs_)
    {
        return (false);
    }
    this->_obsList.push_back( obs_ );
    return (true);
}

bool
Data::Unregister( Observer *obs_ )
{
    if (!obs_)
    {
        return (false);
    }
    this->_obsList.remove( obs_ );
    return (true);
}

void
Data::_notify( Data::STATE state_ )
{
    std::list<Observer*>::iterator it = this->_obsList.begin();
    std::list<Observer*>::iterator end = this->_obsList.end();
    for (; it != end; ++it)
    {
        switch (state_)
        {
            case Data::STATE_PREGET:
            case Data::STATE_PRESET:
            case Data::STATE_POSTGET:
            case Data::STATE_POSTSET:
            case Data::STATE_PRECOMMIT:
            case Data::STATE_POSTCOMMIT:
                (*it)->Handle( state_, *this );
                break;
            default:
                break;
        }
    }
}

}
}
