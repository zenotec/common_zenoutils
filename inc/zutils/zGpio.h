#ifndef _ZGPIO_H_
#define _ZGPIO_H_

#include <stdint.h>

#include <string>
#include <list>

#include <zutils/zThread.h>
#include <zutils/zSwitch.h>
#include <zutils/zConf.h>

namespace zUtils
{
namespace zGpio
{

class Handler;

class Port: zConf::Data
{
    friend class Handler;

public:

    static const std::string EXPORT_FILENAME_KEY;
    static const std::string EXPORT_FILENAME_DEF;

    static const std::string UNEXPORT_FILENAME_KEY;
    static const std::string UNEXPORT_FILENAME_DEF;

    static const std::string ID_VALUE_KEY;

    static const std::string DIR_FILENAME_KEY;
    static const std::string DIR_FILENAME_DEF;
    static const std::string DIR_VALUE_KEY;
    static const std::string DIR_VALUE_DEF;
    static const std::string DIR_VALUE_IN;
    static const std::string DIR_VALUE_OUT;

    static const std::string STATE_FILENAME_KEY;
    static const std::string STATE_FILENAME_DEF;
    static const std::string STATE_VALUE_ACTIVE;
    static const std::string STATE_VALUE_INACTIVE;

    static const std::string EDGE_FILENAME_KEY;
    static const std::string EDGE_FILENAME_DEF;
    static const std::string EDGE_VALUE_KEY;
    static const std::string EDGE_VALUE_DEF;
    static const std::string EDGE_VALUE_NONE;
    static const std::string EDGE_VALUE_LO_HI;
    static const std::string EDGE_VALUE_HI_LO;
    static const std::string EDGE_VALUE_BOTH;

    enum DIR
    {
        DIR_ERR = -1,
        DIR_NONE = 0,
        DIR_IN = 1,
        DIR_DEF = 1,
        DIR_OUT = 2,
        DIR_INOUT = 3,
        DIR_LAST
    };

    enum STATE
    {
        STATE_ERR = -1,
        STATE_NONE = 0,
        STATE_DEF = 0,
        STATE_ACTIVE = 1,
        STATE_INACTIVE = 2,
        STATE_LAST
    };

    enum EDGE
    {
        EDGE_ERR = -1,
        EDGE_NONE = 0,
        EDGE_DEF = 0,
        EDGE_LO_HI = 1,
        EDGE_HI_LO = 2,
        EDGE_BOTH = 3,
        EDGE_LAST
    };

    Port( uint32_t id_ );
    Port( zConf::Data &conf_ );

    virtual
    ~Port();

    uint32_t
    GetId();

    Port::DIR
    GetDirection();

    bool
    SetDirection( Port::DIR dir_ );

    Port::STATE
    GetState();

    bool
    SetState( Port::STATE state_ );

    Port::EDGE
    GetEdge();

    bool
    SetEdge( Port::EDGE edge_ );

protected:

    FILE *_state_file;

    bool
    _open();

    bool
    _close();

private:

    std::string
    _conf_get_export_filename();

    std::string
    _conf_get_unexport_filename();

    std::string
    _conf_get_direction_filename();

    std::string
    _conf_get_direction_value();

    std::string
    _conf_get_state_filename();

    std::string
    _conf_get_edge_filename();

    std::string
    _conf_get_edge_value();

};

class Observer
{

public:

    virtual void
    PortHandler( zGpio::Port &port_ ) = 0;

protected:

private:

};

class Handler: public zThread::Function
{

public:

    Handler();

    virtual
    ~Handler();

    bool
    AddPort( Port* port_ );

    bool
    RemovePort( Port* port_ );

    Port*
    GetPort( uint32_t id_ );

    Port::STATE
    GetState();

    bool
    SetState( Port::STATE state_ );

    bool
    Register( Observer *obs_ );

    bool
    Unregister( Observer *obs_ );

protected:

    virtual void *
    ThreadFunction( void *arg_ );

private:

    std::list<Port *> _portList;

    std::list<zGpio::Observer *> _obsList;

    zThread::Thread _thread;

    void
    _notify( zGpio::Port &port_ );

};

class Switch: public zSwitch::Switch, public Handler
{

public:

    Switch( zSwitch::Switch::STATE state_ );

    virtual
    ~Switch();

protected:

private:

    virtual bool
    _turnOn();

    virtual bool
    _turnOff();

};

}
}

#endif /* _ZGPIO_H_ */
