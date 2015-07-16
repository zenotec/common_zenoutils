//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zSwitch.h
//    Description:
//
//*****************************************************************************

#ifndef __ZSWITCH_H__
#define __ZSWITCH_H__

namespace zUtils
{
namespace zSwitch
{

class Switch
{

public:

  enum STATE
  {
    STATE_NONE = 0,
    STATE_ON = 1,
    STATE_OFF = 2,
    STATE_LAST
  };

  Switch(Switch::STATE state);

  virtual
  ~Switch();

  bool
  On();

  bool
  Off();

  bool
  Toggle();

  bool
  IsOn();

  bool
  IsOff();

protected:

  virtual bool
  _turnOn() = 0;

  virtual bool
  _turnOff() = 0;

private:

  Switch::STATE _state;

};

}
}

#endif /* __ZSWITCH_H__ */
