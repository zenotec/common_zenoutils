/*
 * Data.cpp
 *
 *  Created on: May 8, 2014
 *      Author: kmahoney
 */

#include "zutils/zLog.h"
#include "zutils/zData.h"

namespace zUtils
{
namespace zData
{

//**********************************************************************
// Data Class
//**********************************************************************

const std::string Data::ROOT = "zData";
const std::string Data::KEY = "Key";

Data::Data( const std::string &key_ )
{
    ZLOG_DEBUG( "Creating new zData object: " + key_ );
    this->_setKey( key_ );
    this->_pt.put( key_, "" );
}

Data::~Data()
{
    ZLOG_DEBUG( "Destroying zData object: " + this->GetKey() );
    this->_pt.clear();
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

std::string
Data::GetKey() const
{
    std::string key;
    try
    {
        key = this->_pt.get<std::string>( Data::KEY );
    }
    catch (boost::property_tree::ptree_bad_path const &e)
    {
        ZLOG_WARN( "zData key does not exist, returning empty key" );
    }
    return (key);
}

bool
Data::_setKey( const std::string &key_ )
{
    bool status = false;
    try
    {
        this->_pt.put( Data::KEY, key_ );
        status = true;
    }
    catch (boost::property_tree::ptree_bad_path const &e)
    {
        ZLOG_ERR( "Cannot set zData key value, leaving blank" );
    }
    return (status);
}

std::string
Data::GetValue( const std::string &name_ ) const
{
    std::string name;
    std::string value;
    try
    {
        name = this->GetKey() + "." + name_;
        value = this->_pt.get<std::string>( name );
        ZLOG_DEBUG( "Getting zData value: " + name_ + " = " + value );
    }
    catch (boost::property_tree::ptree_bad_path const &e)
    {
        ZLOG_WARN( "zData value '" + name + "' does not exist, returning empty value" );
    }
    return (value);
}

bool
Data::GetValue( const std::string &name_, std::string &value_ )
{
    bool status = false;
    std::string name;
    std::string value;
    try
    {
        name = this->GetKey() + "." + name_;
        value = this->_pt.get<std::string>( name );
        ZLOG_DEBUG( "Getting zData value: " + name_ + " = " + value );
        value_ = value;
        status = true;
    }
    catch (boost::property_tree::ptree_bad_path &e)
    {
        ZLOG_WARN( "zData value '" + name + "' does not exist, returning empty value" );
    }
    return (status);
}

bool
Data::GetValue( const std::string &name_, int &value_ )
{
    bool status = false;
    std::string name;
    int value;
    try
    {
        name = this->GetKey() + "." + name_;
        value = this->_pt.get<int>( name );
        ZLOG_DEBUG( "Getting zData value: " + name_ + " = " + zLog::IntStr(value) );
        value_ = value;
        status = true;
    }
    catch (boost::property_tree::ptree_bad_path &e)
    {
        ZLOG_WARN( "zData value '" + name + "' does not exist, returning empty value" );
    }
    return (status);
}

bool
Data::GetValue( const std::string &name_, unsigned int &value_ )
{
    bool status = false;
    std::string name;
    int value;
    try
    {
        name = this->GetKey() + "." + name_;
        value = this->_pt.get<unsigned int>( name );
        ZLOG_DEBUG( "Getting zData value: " + name_ + " = " + zLog::IntStr(value) );
        value_ = value;
        status = true;
    }
    catch (boost::property_tree::ptree_bad_path &e)
    {
        ZLOG_WARN( "zData value '" + name + "' does not exist, returning empty value" );
    }
    return (status);
}

boost::property_tree::ptree
Data::_getValue( const std::string &name_ ) const
{
    boost::property_tree::ptree value;
    try
    {
        value = this->_pt.get_child( name_ );
    }
    catch (boost::property_tree::ptree_bad_path const &e)
    {
        ZLOG_WARN( "zData value '" + name_ + "' does not exist, returning empty value" );
    }
    return (value);
}

bool
Data::SetValue( const std::string &name_, const std::string &value_ )
{
    bool status = false;
    std::string name;
    try
    {
        name = this->GetKey() + "." + name_;
        this->_pt.put( name, value_ );
        ZLOG_DEBUG( "Setting zData value: " + name + " = " + value_ );
        status = true;
    }
    catch (boost::property_tree::ptree_bad_path const &e)
    {
        ZLOG_WARN( "Cannot set zData value:" + name );
    }
    return (status);
}

bool
Data::SetValue( const std::string &name_, const int &value_ )
{
    bool status = false;
    std::string name;
    try
    {
        name = this->GetKey() + "." + name_;
        this->_pt.put( name, value_ );
        ZLOG_DEBUG( "Setting zData value: " + name + " = " + zLog::IntStr(value_) );
        status = true;
    }
    catch (boost::property_tree::ptree_bad_path const &e)
    {
        ZLOG_WARN( "Cannot set zData value:" + name );
    }
    return (status);
}

bool
Data::SetValue( const std::string &name_, const unsigned int &value_ )
{
    bool status = false;
    std::string name;
    try
    {
        name = this->GetKey() + "." + name_;
        this->_pt.put( name, value_ );
        ZLOG_DEBUG( "Setting zData value: " + name + " = " + zLog::IntStr(value_) );
        status = true;
    }
    catch (boost::property_tree::ptree_bad_path const &e)
    {
        ZLOG_WARN( "Cannot set zData value:" + name );
    }
    return (status);
}

bool
Data::_setValue( const std::string &name_, const boost::property_tree::ptree &pt_ )
{
    bool status = false;
    try
    {
        this->_pt.put_child( name_, pt_ );
        status = true;
    }
    catch (boost::property_tree::ptree_bad_path const &e)
    {
        ZLOG_WARN( "Cannot set zData value:" + name_ );
    }
    return (status);
}

bool
Data::GetChild( const std::string &name_, Data &child_ ) const
{
    bool status = false;
    std::string name = this->GetKey() + "." + name_;
    ZLOG_DEBUG( "Getting zData child: '" + name + "'" );
    try
    {
        child_._setKey( name_ );
        child_._setValue( name_, this->_pt.get_child( name ) );
        ZLOG_DEBUG( "\n" + child_.GetJson() );
        status = true;
    }
    catch (boost::property_tree::ptree_bad_path const &e)
    {
        ZLOG_WARN( "Cannot get zData child: " + name );
    }
    return (status);
}

bool
Data::PutChild( const std::string &name_, const Data &child_ )
{
    bool status = false;
    std::string name = this->GetKey() + "." + name_;
    ZLOG_DEBUG( "Putting zData child: '" + name + "'\n" + child_.GetJson() );
    try
    {
        this->_pt.put_child( name, child_._getValue( child_.GetKey() ) );
        status = true;
    }
    catch (boost::property_tree::ptree_bad_path const &e)
    {
        ZLOG_WARN( "Cannot put zData child: " + name );
    }
    return (status);
}

bool
Data::AddChild( const std::string &name_, const Data &child_ )
{
    bool status = false;
    std::string name = this->GetKey() + "." + name_;
    ZLOG_DEBUG( "Adding zData child: '" + name + "'\n" + child_.GetJson() );

    boost::property_tree::ptree parent;
    boost::property_tree::ptree pt;

    // Get the parent node to add to
    try
    {
        parent = this->_pt.get_child( name );
    }
    catch (boost::property_tree::ptree_bad_path const &e)
    {
        ZLOG_WARN( "zData parent does not exist: " + name );
    }

    try
    {
        pt = child_._getValue( child_.GetKey() );
        parent.push_back(std::make_pair("", pt));
        this->_pt.put_child( name, parent );
        status = true;
    }
    catch (boost::property_tree::ptree_bad_path const &e)
    {
        ZLOG_WARN( "Cannot add zData child: " + name );
    }
    return (status);
}

std::string
Data::GetJson() const
{
    std::stringstream json;
    boost::property_tree::write_json( json, this->_pt );
    return (json.str());
}

bool
Data::SetJson( const std::string &json_ )
{
    bool status = false;
    std::stringstream json( json_ );
    try
    {
        boost::property_tree::read_json( json, this->_pt );
        status = true;
    }
    catch (boost::property_tree::json_parser_error const &e)
    {
        ZLOG_ERR( "Cannot set data from JSON: " + json_ );
    }
    return (status);
}

std::string
Data::GetXml() const
{
    std::stringstream xml;
    boost::property_tree::write_xml( xml, this->_pt );
    return (xml.str());
}

bool
Data::SetXml( const std::string &xml_ )
{
    std::stringstream xml( xml_ );
    boost::property_tree::read_xml( xml, this->_pt );
    return (true);
}

}
}
