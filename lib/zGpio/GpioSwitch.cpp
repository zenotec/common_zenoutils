#include <zutils/zLog.h>
#include <zutils/zGpio.h>

namespace zUtils
{
namespace zGpio
{

//*****************************************************************************
// Switch Class
//*****************************************************************************

GpioSwitch::GpioSwitch(zSwitch::Switch::STATE state_) :
        zSwitch::Switch(state_)
{
}

GpioSwitch::~GpioSwitch()
{

}

bool GpioSwitch::_turnOn()
{
    return (this->SetState(Port::STATE_ACTIVE));
}

bool GpioSwitch::_turnOff()
{
    return (this->SetState(Port::STATE_INACTIVE));
}

}
}
