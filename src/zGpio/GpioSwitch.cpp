#include <zutils/zLog.h>
#include <zutils/zGpio.h>

namespace zUtils
{
namespace zGpio
{

//*****************************************************************************
// Switch Class
//*****************************************************************************

Switch::Switch(zSwitch::Switch::STATE state_) :
        zSwitch::Switch(state_)
{
}

Switch::~Switch()
{

}

bool Switch::_turnOn()
{
    return (this->SetState(Port::STATE_ACTIVE));
}

bool Switch::_turnOff()
{
    return (this->SetState(Port::STATE_INACTIVE));
}

}
}
