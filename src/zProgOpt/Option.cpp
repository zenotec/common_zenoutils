//*****************************************************************************
//
//
//
//*****************************************************************************

#include <stdio.h>

#include <iostream>
#include <sstream>

#include <zutils/zProgOpt.h>

namespace zUtils
{
namespace zProgOpt
{

//*****************************************************************************
//*****************************************************************************
Option::Option(uint32_t flags_, const std::string &name_) :
    _cnt(0), _flags(Option::FLAGS_NONE), _name(name_)
{
  this->_setFlags(flags_);
}

Option::~Option()
{
}

uint32_t
Option::Flags() const
{
  return (this->_flags);
}

std::string
Option::Name() const
{
  return (this->_name);
}

std::string
Option::HelpMsg() const
{
  return (this->_help);
}

void
Option::HelpMsg(const std::string &msg_)
{
  this->_help = msg_;
}

template<typename T>
  bool
  Option::Default(T default_)
  {
    std::stringstream ss;
    ss << default_;
    this->_arg = ss.str();
    return (true);
  }

template bool
Option::Default(std::string default_);
template bool
Option::Default(int default_);
template bool
Option::Default(unsigned int default_);
template bool
Option::Default(double default_);

template<typename T>
  T
  Option::Arg()
  {
    T arg;
    this->_getArg(arg);
    return (arg);
  }

template std::string
Option::Arg();

void
Option::_getArg(std::string &arg_)
{
  arg_ = this->_arg;
}

template int32_t
Option::Arg();

void
Option::_getArg(int &arg_)
{
  arg_ = 0;
  sscanf(this->_arg.c_str(), "%d", &arg_);
}

template uint32_t
Option::Arg();

void
Option::_getArg(unsigned int &arg_)
{
  arg_ = 0;
  sscanf(this->_arg.c_str(), "%u", &arg_);
}

template double
Option::Arg();

void
Option::_getArg(double &arg_)
{
  arg_ = 0.0;
  sscanf(this->_arg.c_str(), "%lf", &arg_);
}

bool
Option::_setArg(const std::string &arg_)
{
  this->_arg = arg_;
  return (true);
}

bool
Option::_setFlags(uint32_t flags_)
{
  this->_flags = flags_;
  return (true);
}

}
}
