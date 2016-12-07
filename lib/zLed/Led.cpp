#include <string.h>

#include <fstream>

#include <zutils/zLed.h>

namespace zUtils
{
namespace zLed
{

const std::string Led::LED_COM1_TX = "COM1-TX";
const std::string Led::LED_COM1_RX = "COM1-RX";
const std::string Led::LED_COM2_TX = "COM2-TX";
const std::string Led::LED_COM2_RX = "COM2-RX";
const std::string Led::LED_TX_GREEN = "TX-GREEN";
const std::string Led::LED_TX_RED = "TX-RED";
const std::string Led::LED_TX_AMBER = "TX-AMBER";
const std::string Led::LED_CD_GREEN = "CD-GREEN";
const std::string Led::LED_CD_RED = "CD-RED";
const std::string Led::LED_CD_AMBER = "CD-AMBER";
const std::string Led::LED_CTS_GREEN = "CTS-GREEN";
const std::string Led::LED_CTS_RED = "CTS-RED";
const std::string Led::LED_CTS_AMBER = "CTS-AMBER";

const std::string Led::DEVROOT_DEFAULT = "/sys/class/leds";

const std::string Led::TRIGGER_FILENAME = "trigger";
const std::string Led::TRIGGER_NONE = "none";
const std::string Led::TRIGGER_ONESHOT = "oneshot";
const std::string Led::TRIGGER_HEARTBEAT = "heartbeat";

const std::string Led::ACTIONFILE_NONE = "brightness";
const std::string Led::ACTIONFILE_ONESHOT = "shot";

Led::Led(const std::string& name_, Led::COLOR color_, Led::MODE mode_, Led::STATE defState_,
    const std::string& root_) :
    _root(root_), _name(name_), _color(color_), _mode(mode_), _defState(defState_),
        _state(defState_)
{
}

Led::~Led()
{
}

std::string
Led::GetName()
{
  return (this->_name);
}

Led::COLOR
Led::GetColor()
{
  return (this->_color);
}

Led::MODE
Led::GetMode()
{
  return (this->_mode);
}

Led::STATE
Led::GetState()
{
  return (this->_state);
}

std::string
Led::_getDevRoot()
{
  std::string devRoot = this->_root + std::string("/") + this->_name + std::string("/");
  return (devRoot);
}

std::string
Led::_getActionFile()
{
  return (this->_actionFile);
}

bool
Led::_setMode(Led::MODE mode_)
{

  bool status = false;
  // Set new mode
  switch (mode_)
  {
  case Led::MODE_NONE:
    status = this->_setModeNone();
    break;
  case Led::MODE_ONESHOT:
    status = this->_setModeOneShot();
    break;
  case Led::MODE_HEARTBEAT:
    status = this->_setModeHeartbeat();
    break;
  default:
    status = false;
  }
  return (status && this->_setState(this->_defState));
}

bool
Led::_setModeNone()
{
  std::fstream fs;
  std::string triggerFile = this->_getDevRoot() + TRIGGER_FILENAME;

  // Open trigger file and write trigger mode
  fs.open(triggerFile.c_str(), std::fstream::out);
  if (!fs.is_open())
  {
    return (false);
  }
  fs << Led::TRIGGER_NONE << std::endl;
  fs.flush();
  fs.close();

  // Update action filename
  this->_actionFile = this->_getDevRoot() + ACTIONFILE_NONE;

  // Update LED mode
  this->_mode = Led::MODE_NONE;

  // Set default state
  return (true);

}

bool
Led::_setModeOneShot()
{
  std::fstream fs;
  std::string triggerFile = this->_getDevRoot() + TRIGGER_FILENAME;

  // Open trigger file and write trigger mode
  fs.open(triggerFile.c_str(), std::fstream::out);
  if (!fs.is_open())
  {
    return (false);
  }
  fs << Led::TRIGGER_ONESHOT << std::endl;
  fs.flush();
  fs.close();

  // Update action filename
  this->_actionFile = this->_getDevRoot() + ACTIONFILE_ONESHOT;

  // Update LED mode
  this->_mode = Led::MODE_ONESHOT;

  // Set default state
  return (this->_setState(this->_defState));
}

bool
Led::_setModeHeartbeat()
{
  std::fstream fs;
  std::string triggerFile = this->_getDevRoot() + TRIGGER_FILENAME;

  // Open trigger file and write trigger mode
  fs.open(triggerFile.c_str(), std::fstream::out);
  if (!fs.is_open())
  {
    return (false);
  }
  fs << Led::TRIGGER_HEARTBEAT << std::endl;
  fs.flush();
  fs.close();

  // There is no action file for heartbeat so just make sure it is empty
  this->_actionFile = "";

  // Update LED mode
  this->_mode = Led::MODE_HEARTBEAT;

  // There is no default state so just return success
  return (true);
}

bool
Led::_setState(Led::STATE state_)
{
  bool status = false;
  std::fstream fs;
  fs.open(this->_actionFile.c_str(), std::fstream::out | std::fstream::trunc);
  if (fs.is_open())
  {
    fs << state_ << std::endl;
    fs.flush();
    fs.close();
    this->_state = state_;
    status = true;
  }
  return (status);
}

}
}
