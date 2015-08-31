#include <fcntl.h>

#include <sstream>
#include <fstream>

#include <zutils/zLog.h>
#include <zutils/zGpio.h>

namespace zUtils
{
namespace zGpio
{

//*****************************************************************************
// Port Class
//*****************************************************************************

const std::string Port::EXPORT_FILENAME_KEY = "export.filename";
const std::string Port::EXPORT_FILENAME_DEF = "/sys/class/gpio/export";

const std::string Port::UNEXPORT_FILENAME_KEY = "unexport.filename";
const std::string Port::UNEXPORT_FILENAME_DEF = "/sys/class/gpio/unexport";

const std::string Port::ID_VALUE_KEY = "gpio_id";

const std::string Port::DIR_FILENAME_KEY = "direction.filename";
const std::string Port::DIR_FILENAME_DEF = "/sys/class/gpio/gpio%d/direction";

const std::string Port::DIR_VALUE_KEY = "direction.value";
const std::string Port::DIR_VALUE_IN = "in";
const std::string Port::DIR_VALUE_OUT = "out";
const std::string Port::DIR_VALUE_DEF = Port::DIR_VALUE_IN;

const std::string Port::STATE_FILENAME_KEY = "state.filename";
const std::string Port::STATE_FILENAME_DEF = "/sys/class/gpio/gpio%d/value";

const std::string Port::STATE_VALUE_ACTIVE = "1";
const std::string Port::STATE_VALUE_INACTIVE = "0";

const std::string Port::EDGE_FILENAME_KEY = "edge.filename";
const std::string Port::EDGE_FILENAME_DEF = "/sys/class/gpio/gpio%d/edge";

const std::string Port::EDGE_VALUE_KEY = "edge.value";
const std::string Port::EDGE_VALUE_NONE = "none";
const std::string Port::EDGE_VALUE_LO_HI = "rising";
const std::string Port::EDGE_VALUE_HI_LO = "falling";
const std::string Port::EDGE_VALUE_BOTH = "both";
const std::string Port::EDGE_VALUE_DEF = Port::EDGE_VALUE_NONE;

Port::Port( uint32_t id_ ) :
        _state_file( 0 )
{
    this->Set( Port::EXPORT_FILENAME_KEY, Port::EXPORT_FILENAME_DEF );
    this->Set( Port::UNEXPORT_FILENAME_KEY, Port::UNEXPORT_FILENAME_DEF );
    this->Set( Port::ID_VALUE_KEY, id_ );
    this->Set( Port::DIR_FILENAME_KEY, Port::DIR_FILENAME_DEF );
    this->Set( Port::DIR_VALUE_KEY, Port::DIR_VALUE_DEF );
    this->Set( Port::STATE_FILENAME_KEY, Port::STATE_FILENAME_DEF );
    this->Set( Port::EDGE_FILENAME_KEY, Port::EDGE_FILENAME_DEF );
    this->Set( Port::EDGE_VALUE_KEY, Port::EDGE_VALUE_DEF );
}

Port::Port( zConf::Data &conf_ ) :
        _state_file( 0 )
{

    uint32_t id;

    if (!conf_.Get( Port::ID_VALUE_KEY, id ))
    {
        std::string err_msg = "Missing port identifier in configuration";
        throw err_msg;
    }
    this->Set( Port::ID_VALUE_KEY, id );

    std::string val;

    if (!conf_.Get( Port::EXPORT_FILENAME_KEY, val ))
    {
        val = Port::EXPORT_FILENAME_DEF;
    }
    this->Set( Port::EXPORT_FILENAME_KEY, val );

    if (!conf_.Get( Port::UNEXPORT_FILENAME_KEY, val ))
    {
        val = Port::UNEXPORT_FILENAME_DEF;
    }
    this->Set( Port::UNEXPORT_FILENAME_KEY, val );

    if (!conf_.Get( Port::DIR_FILENAME_KEY, val ))
    {
        val = Port::DIR_FILENAME_DEF;
    }
    this->Set( Port::DIR_FILENAME_KEY, val );

    if (!conf_.Get( Port::DIR_VALUE_KEY, val ))
    {
        val = Port::DIR_VALUE_DEF;
    }
    this->Set( Port::DIR_VALUE_KEY, val );

    if (!conf_.Get( Port::STATE_FILENAME_KEY, val ))
    {
        val = Port::STATE_FILENAME_DEF;
    }
    this->Set( Port::STATE_FILENAME_KEY, val );

    if (!conf_.Get( Port::EDGE_FILENAME_KEY, val ))
    {
        val = Port::EDGE_FILENAME_DEF;
    }
    this->Set( Port::EDGE_FILENAME_KEY, val );

    if (!conf_.Get( Port::EDGE_VALUE_KEY, val ))
    {
        val = Port::EDGE_VALUE_DEF;
    }
    this->Set( Port::EDGE_VALUE_KEY, val );
}

Port::~Port()
{
    this->_close();
}

uint32_t
Port::GetId()
{
    uint32_t id;
    this->Get( Port::ID_VALUE_KEY, id );
    return (id);
}

Port::DIR
Port::GetDirection()
{
    Port::DIR dir = Port::DIR_ERR;
    std::string val;

    if (this->_state_file)
    {
        std::fstream fs;
        // Open direction file and read direction
        fs.open( this->_conf_get_direction_filename().c_str(), std::fstream::in );
        if (fs.is_open())
        {
            fs >> val;
            fs.close();
        }
    }
    else
    {
        val = this->_conf_get_direction_value();
    }
    ZLOG_DEBUG( "zGpio::Port::GetDirection: Direction: '" + val + "'" );

    // Convert from string to enum
    if (Port::DIR_VALUE_IN == val)
    {
        dir = Port::DIR_IN;
    }
    else if (Port::DIR_VALUE_OUT == val)
    {
        dir = Port::DIR_OUT;
    }
    else
    {
        dir = Port::DIR_ERR;
    }

    // Return port direction
    return (dir);
}

bool
Port::SetDirection( Port::DIR dir_ )
{
    bool status = false;
    if (!this->_state_file)
    {
        if (Port::DIR_IN == dir_)
        {
            status = this->Set( Port::DIR_VALUE_KEY, Port::DIR_VALUE_IN );
        }
        else if (Port::DIR_OUT == dir_)
        {
            status = this->Set( Port::DIR_VALUE_KEY, Port::DIR_VALUE_OUT );
        }
        else
        {
            status = false;
        }
    }
    return (status);
}

Port::STATE
Port::GetState()
{
    Port::STATE state = Port::STATE_ERR;
    std::string val;

    if (this->_state_file)
    {
        std::fstream fs;

        // Open value file and read value
        fs.open( this->_conf_get_state_filename().c_str(), std::fstream::in );
        if (fs.is_open())
        {
            fs >> val;
            fs.close();
        }

        ZLOG_DEBUG( "zGpio::Port::GetState: State: '" + val + "'" );

        if (Port::STATE_VALUE_ACTIVE == val)
        {
            state = Port::STATE_ACTIVE;
        }
        else if (Port::STATE_VALUE_INACTIVE == val)
        {
            state = Port::STATE_INACTIVE;
        }
        else
        {
            state = Port::STATE_ERR;
        }
    }

    return (state);
}

bool
Port::SetState( Port::STATE state_ )
{
    bool status = false;
    if (this->_state_file && (this->GetDirection() == Port::DIR_OUT))
    {
        std::fstream fs;

        // Open value file and write value
        fs.open( this->_conf_get_state_filename().c_str(), std::fstream::out );
        if (fs.is_open())
        {
            if (Port::STATE_ACTIVE == state_)
            {
                ZLOG_DEBUG( "zGpio::Port::SetState: State: '" + Port::STATE_VALUE_ACTIVE + "'" );
                fs << Port::STATE_VALUE_ACTIVE;
                status = true;
            }
            else if (Port::STATE_INACTIVE == state_)
            {
                ZLOG_DEBUG( "zGpio::Port::SetState: State: '" + Port::STATE_VALUE_INACTIVE + "'" );
                fs << Port::STATE_VALUE_INACTIVE;
                status = true;
            }
            else
            {
                ZLOG_ERR( "zGpio::Port::SetState: Bad state: " + zLog::IntStr( state_ ) );
                status = false;
            }
            fs.flush();
            fs.close();
        }
    }
    return (status);
}

Port::EDGE
Port::GetEdge()
{
    Port::EDGE edge = Port::EDGE_ERR;
    std::string val;

    if (this->_state_file)
    {
        std::fstream fs;
        // Open edge file and read direction
        fs.open( this->_conf_get_edge_filename().c_str(), std::fstream::in );
        if (fs.is_open())
        {
            fs >> val;
            fs.close();
        }
    }
    else
    {
        val = this->_conf_get_edge_value();
    }
    ZLOG_DEBUG( "zGpio::Port::GetEdge: Edge: '" + val + "'" );

    // Convert from string to enum
    if (Port::EDGE_VALUE_NONE == val)
    {
        edge = Port::EDGE_NONE;
    }
    else if (Port::EDGE_VALUE_LO_HI == val)
    {
        edge = Port::EDGE_LO_HI;
    }
    else if (Port::EDGE_VALUE_HI_LO == val)
    {
        edge = Port::EDGE_HI_LO;
    }
    else if (Port::EDGE_VALUE_BOTH == val)
    {
        edge = Port::EDGE_BOTH;
    }
    else
    {
        edge = Port::EDGE_ERR;
    }

    // Return port edge configuration
    return (edge);
}

bool
Port::SetEdge( Port::EDGE edge_ )
{
    bool status = false;
    if (!this->_state_file)
    {
        if (Port::EDGE_NONE == edge_)
        {
            status = this->Set( Port::EDGE_VALUE_KEY, Port::EDGE_VALUE_NONE );
        }
        else if (Port::EDGE_LO_HI == edge_)
        {
            status = this->Set( Port::EDGE_VALUE_KEY, Port::EDGE_VALUE_LO_HI );
        }
        else if (Port::EDGE_HI_LO == edge_)
        {
            status = this->Set( Port::EDGE_VALUE_KEY, Port::EDGE_VALUE_HI_LO );
        }
        else if (Port::EDGE_BOTH == edge_)
        {
            status = this->Set( Port::EDGE_VALUE_KEY, Port::EDGE_VALUE_BOTH );
        }
        else
        {
            status = false;
        }
    }
    return (status);
}

bool
Port::_open()
{

    std::fstream fs;
    std::string val;

    if (!this->_state_file)
    {

        ZLOG_INFO( "zGpio::Port::_open: Opening GPIO Port: " + zLog::IntStr( this->GetId() ) );

        // Open export file and write the GPIO identifier
        fs.open( this->_conf_get_export_filename().c_str(), std::fstream::out );
        if (!fs.is_open())
        {
            ZLOG_ERR( "zGpio::Port::_open: Failed to open export file: "
                    + this->_conf_get_export_filename() );
            return (false);
        }
        else
        {
            fs << this->GetId() << std::endl;
            fs.flush();
            fs.close();
        }

        // Set the direction of the port
        fs.open( this->_conf_get_direction_filename().c_str(), std::fstream::out );
        if (!fs.is_open())
        {
            ZLOG_ERR( "zGpio::Port::_open: Failed to open direction file: "
                    + this->_conf_get_direction_filename() );
            return (false);
        }
        else
        {
            fs << this->_conf_get_direction_value() << std::endl;
            fs.flush();
            fs.close();
        }

        // Set the type of edge to generate an interrupt on
        fs.open( this->_conf_get_edge_filename().c_str(), std::fstream::out );
        if (!fs.is_open())
        {
            ZLOG_ERR( "zGpio::Port::_open: Failed to open edge file: "
                    + this->_conf_get_edge_filename() );
            return (false);
        }
        else
        {
            fs << this->_conf_get_edge_value() << std::endl;
            fs.flush();
            fs.close();
        }

        // Open the value file for watching for state changes
        this->_state_file = open( this->_conf_get_state_filename().c_str(), (O_RDONLY | O_NONBLOCK) );

    }

    return (this->_state_file != 0);
}

bool
Port::_close()
{
    if (this->_state_file)
    {
        std::fstream fs;

        ZLOG_INFO( "zGpio::Port::_close: Closing GPIO Port: " + zLog::IntStr( this->GetId() ) );

        // Open unexport file and write the GPIO id
        fs.open( this->_conf_get_unexport_filename().c_str(), std::fstream::out );
        if (fs.is_open())
        {
            fs << this->GetId() << std::endl;
            fs.flush();
            fs.close();
        }

        close(this->_state_file);
        this->_state_file = 0;
    }
    return (this->_state_file == 0);
}

std::string
Port::_conf_get_export_filename()
{
    std::string val;
    this->Get( EXPORT_FILENAME_KEY, val );
    return (val);
}

std::string
Port::_conf_get_unexport_filename()
{
    std::string val;
    this->Get( UNEXPORT_FILENAME_KEY, val );
    return (val);
}

std::string
Port::_conf_get_direction_filename()
{
    uint32_t id;
    char str[512] = { 0 };
    this->Get( ID_VALUE_KEY, id );

    std::string val;
    this->Get( DIR_FILENAME_KEY, val );

    snprintf( str, 512, val.c_str(), id );
    return (std::string( str ));
}

std::string
Port::_conf_get_direction_value()
{
    std::string val;
    this->Get( DIR_VALUE_KEY, val );
    return (val);
}

std::string
Port::_conf_get_state_filename()
{
    uint32_t id;
    char str[512] = { 0 };
    this->Get( ID_VALUE_KEY, id );

    std::string val;
    this->Get( STATE_FILENAME_KEY, val );

    snprintf( str, 512, val.c_str(), id );
    return (std::string( str ));
}

std::string
Port::_conf_get_edge_filename()
{
    uint32_t id;
    char str[512] = { 0 };
    this->Get( ID_VALUE_KEY, id );

    std::string val;
    this->Get( EDGE_FILENAME_KEY, val );

    snprintf( str, 512, val.c_str(), id );
    return (std::string( str ));
}

std::string
Port::_conf_get_edge_value()
{
    std::string val;
    this->Get( EDGE_VALUE_KEY, val );
    return (val);
}

}
}
