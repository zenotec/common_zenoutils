#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>

#include <sstream>
#include <fstream>

#include <zutils/zLog.h>
#include <zutils/zGpio.h>

namespace zUtils
{
namespace zGpio
{

//*****************************************************************************
// Handler Class
//*****************************************************************************
Handler::Handler() :
        _thread( this, this )
{
}

Handler::~Handler()
{
    std::list<Port*>::iterator it = this->_portList.begin();
    std::list<Port*>::iterator end = this->_portList.end();
    for (; it != end; ++it)
    {
        (*it)->_close();
    }
    this->_portList.clear();
}

bool
Handler::AddPort( Port* port_ )
{
    bool status = false;
    if (port_->_open())
    {
        this->_portList.push_back( port_ );
        this->_portList.unique();
        status = true;
    }
    return (status);
}

bool
Handler::RemovePort( Port* port_ )
{
    bool status = false;
    if (port_->_close())
    {
        this->_portList.remove( port_ );
        status = true;
    }
    return (status);
}

Port*
Handler::GetPort( uint32_t id_ )
{
    std::list<Port*>::iterator it = this->_portList.begin();
    std::list<Port*>::iterator end = this->_portList.end();
    for (; it != end; ++it)
    {
        if ((*it)->GetId() == id_)
        {
            return (*it);
        }
    }

    return (0);
}

Port::STATE
Handler::GetState()
{
    Port::STATE state = Port::STATE_NONE;
    std::list<Port*>::iterator it = this->_portList.begin();
    std::list<Port*>::iterator end = this->_portList.end();
    for (; it != end; ++it)
    {
        if ((state != Port::STATE_NONE) && (state != (*it)->GetState()))
        {
            return (Port::STATE_ERR);
        }
        state = (*it)->GetState();
    }
    return (state);
}

bool
Handler::SetState( Port::STATE state_ )
{
    Port::STATE state = Port::STATE_NONE;
    std::list<Port*>::iterator it = this->_portList.begin();
    std::list<Port*>::iterator end = this->_portList.end();
    for (; it != end; ++it)
    {
        if (!(*it)->SetState( state_ ) || (state_ != (*it)->GetState()))
        {
            return (false);
        }
    }
    return (true);
}

bool
Handler::Register( Observer *obs_ )
{
    if (!obs_)
    {
        return (false);
    }
    this->_obsList.push_back( obs_ );
    return (true);
}

bool
Handler::Unregister( Observer *obs_ )
{
    if (!obs_)
    {
        return (false);
    }
    this->_obsList.remove( obs_ );
    return (true);
}

bool
Handler::Monitor()
{
    return(this->_thread.Run( 1000 ));
}

void *
Handler::ThreadFunction( void *arg_ )
{

    Handler *self = (Handler *) arg_;
    fd_set fds;
    int maxfd = 0;
    struct timeval to;
    std::list<Port*>::iterator it;
    std::list<Port*>::iterator end;

    // Setup for select loop
    FD_ZERO( &fds );
    it = self->_portList.begin();
    end = self->_portList.end();
    for (; it != end; ++it)
    {
        int fd = fileno( (*it)->_state_file );
        if (fd > maxfd)
            maxfd = fd;
        FD_SET( fileno( (*it)->_state_file ), &fds );
    }
    to.tv_sec = 0;
    to.tv_usec = 100000;

    int ret = select( maxfd + 1, &fds, NULL, NULL, &to );
    if (ret > 0)
    {
        it = self->_portList.begin();
        end = self->_portList.end();
        for (; it != end; ++it)
        {
            int fd = fileno( (*it)->_state_file );
            if (FD_ISSET( fd, &fds ))
            {
                self->_notify(*(*it));
            }
        }
    }

    return (0);
}

void
Handler::_notify( zGpio::Port &port_ )
{
    std::list<Observer*>::iterator it = this->_obsList.begin();
    std::list<Observer*>::iterator end = this->_obsList.end();
    for (; it != end; ++it)
    {
        (*it)->PortHandler( port_ );
    }
}

}
}
