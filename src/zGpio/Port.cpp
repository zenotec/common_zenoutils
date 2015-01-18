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

const std::string Port::DEFAULT_ROOT = "/sys/class/gpio/";
const std::string Port::EXPORT_FILENAME = "export";
const std::string Port::UNEXPORT_FILENAME = "unexport";
const std::string Port::STATE_FILENAME = "value";
const std::string Port::DIR_FILENAME = "direction";

const std::string Port::DIR_IN_STR = "in";
const std::string Port::DIR_OUT_STR = "out";

const std::string Port::STATE_ACTIVE_STR = "1";
const std::string Port::STATE_INACTIVE_STR = "0";

Port::Port(uint32_t id_, Port::DIR dir_,
    Port::STATE initialState_,
    const std::string& root_) :
    _root(root_), _opened(false), _id(id_), _dir(dir_), _state(initialState_)
{
}

Port::~Port()
{
  this->_close();
}

uint32_t
Port::GetId()
{
  return (this->_id);
}

bool
Port::_open()
{
  if (!this->_opened)
  {
    std::fstream fs;
    std::string fileName = this->_getRootDir() + Port::EXPORT_FILENAME;

    // Open export file and write the GPIO id
    fs.open(fileName.c_str(), std::fstream::out);
    if (fs.is_open())
    {
      fs << this->_id << std::endl;
      fs.flush();
      fs.close();
      this->_opened = true;
    }

    // Setup GPIO based on configuration
    switch (this->_dir)
    {
    case Port::DIR_IN:
      this->_setDirection(this->_dir);
      this->_state = this->_getState();
      break;
    case Port::DIR_OUT:
      this->_setDirection(this->_dir);
      this->_setState(this->_state);
      break;
    default:
      break;
    }
  }
  return (this->_opened);
}

bool
Port::_close()
{
  if (this->_opened)
  {
    std::fstream fs;
    std::string fileName = this->_getRootDir() + Port::UNEXPORT_FILENAME;

    // Open export file and write the GPIO id
    fs.open(fileName.c_str(), std::fstream::out);
    if (fs.is_open())
    {
      fs << this->_id << std::endl;
      fs.flush();
      fs.close();
      this->_opened = false;
    }
  }
  return (!this->_opened);
}

Port::DIR
Port::_getDirection()
{
  if (this->_opened)
  {
    std::fstream fs;
    std::string fileName = this->_getClassDir() + Port::DIR_FILENAME;

    // Open direction file and read direction
    fs.open(fileName.c_str(), std::fstream::in);
    if (fs.is_open())
    {
      std::string dir;
      fs >> dir;
      fs.close();
      if (Port::DIR_IN_STR == dir)
      {
        this->_dir = Port::DIR_IN;
      }
      else if (Port::DIR_OUT_STR == dir)
      {
        this->_dir = Port::DIR_OUT;
      }
      else
      {
        this->_dir = Port::DIR_ERR;
      }
    }
  }
  return (this->_dir);
}

bool
Port::_setDirection(Port::DIR dir_)
{
  if (this->_opened)
  {
    std::fstream fs;
    std::string fileName = this->_getClassDir() + Port::DIR_FILENAME;

    // Open direction file and write direction
    fs.open(fileName.c_str(), std::fstream::out);
    if (fs.is_open())
    {
      if (Port::DIR_IN == dir_)
      {
        fs << Port::DIR_IN_STR;
        this->_dir = dir_;
      }
      else if (Port::DIR_OUT == dir_)
      {
        fs << Port::DIR_OUT_STR;
        this->_dir = dir_;
      }
      else
      {
        this->_dir = Port::DIR_ERR;
      }
      fs.flush();
      fs.close();
    }
  }
  return (this->_dir == dir_);
}

Port::STATE
Port::_getState()
{
  if (this->_opened)
  {
    std::fstream fs;
    std::string fileName = this->_getClassDir() + Port::STATE_FILENAME;

    // Open value file and read value
    fs.open(fileName.c_str(), std::fstream::in);
    if (fs.is_open())
    {
      std::string state;
      fs >> state;
      fs.close();
      if (Port::STATE_ACTIVE_STR == state)
      {
        this->_state = Port::STATE_ACTIVE;
      }
      else if (Port::STATE_INACTIVE_STR == state)
      {
        this->_state = Port::STATE_INACTIVE;
      }
      else
      {
        this->_state = Port::STATE_ERR;
      }
    }
  }
  return (this->_state);
}

bool
Port::_setState(Port::STATE state_)
{
  if (this->_opened)
  {
    std::fstream fs;
    std::string fileName = this->_getClassDir() + Port::STATE_FILENAME;

    // Open value file and write value
    fs.open(fileName.c_str(), std::fstream::out);
    if (fs.is_open())
    {
      if (Port::STATE_ACTIVE == state_)
      {
        fs << Port::STATE_ACTIVE_STR;
        this->_state = state_;
      }
      else if (Port::STATE_INACTIVE == state_)
      {
        fs << Port::STATE_INACTIVE_STR;
        this->_state = state_;
      }
      else
      {
        this->_state = Port::STATE_ERR;
      }
      fs.flush();
      fs.close();
    }
  }
  return (this->_state == state_);
}

std::string
Port::_getRootDir()
{
  return (this->_root);
}

bool
Port::_setRootDir(const std::string& root_)
{
  this->_root = root_;
  return (this->_root == root_);
}

std::string
Port::_getClassDir()
{
  std::stringstream ss;
  ss << this->_getRootDir() << "gpio" << this->_id << "/";
  return (ss.str());
}

}
}
