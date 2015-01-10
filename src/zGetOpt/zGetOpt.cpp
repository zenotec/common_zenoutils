//*****************************************************************************
//
//
//
//*****************************************************************************
#include <iostream>

#include "zutils/zGetOpt.h"

namespace zUtils
{
namespace zOpt
{

//*****************************************************************************
Option::Option(const std::string &name_, const std::string &desc_, const std::string &default_) :
    _name(name_), _desc(desc_), _arg(default_), _present(false)
{
}

//*****************************************************************************
Option::~Option()
{

}

std::string
Option::GetName() const
{
  return (this->_name);
}

std::string
Option::GetDesc() const
{
  return (this->_desc);
}

void
Option::SetDesc(const std::string &desc_)
{
  this->_desc = desc_;
}

std::string
Option::GetArg() const
{
  return (this->_arg);
}

void
Option::SetArg(const std::string &arg_)
{
  this->_arg = arg_;
}

bool
Option::GetPresent()
{
  return (this->_present);
}

void
Option::SetPresent(bool present_)
{
  this->_present = present_;
}

//*****************************************************************************
zGetOpt::zGetOpt()
{
  Option opt("help", "Display usage and exit");
  this->AddOption(opt);
}

//*****************************************************************************
zGetOpt::~zGetOpt()
{

}

Option &
zGetOpt::operator[](const std::string &id_)
{
  return (this->_opts[id_]);
}

void
zGetOpt::SetUsage(const std::string &usage_)
{
  this->_usage = usage_;
}

std::string
zGetOpt::Usage()
{
  return (this->_usage);
}

bool
zGetOpt::AddOption(const Option &opt_)
{
  bool ret = true;
  if (!opt_.GetName().empty())
  {
    this->_opts[opt_.GetName()] = opt_;
  } // end if
  else
  {
    ret = false;
  } // end else
  return (ret);
}

bool
zGetOpt::Parse(int argc_, const char **argv_)
{
  Option *opt = NULL;

  for (int arg = 0; arg < argc_; arg++)
  {
    if (argv_[arg][0] == '-')
    {
      if (argv_[arg][1] == '-')
      {
        opt = this->_find_opt(std::string(&argv_[arg][2]));
      } // end if
      else
      {
        opt = this->_find_opt(std::string(&argv_[arg][1]));
      } // end else
      if (opt != NULL)
      {
        opt->SetPresent(true);
      } // end if
    } // end if
    else
    {
      if (opt != NULL)
      {
        opt->SetArg(std::string(argv_[arg]));
        opt = NULL; // reset arg pointer
      } // end if
    } // end else
  } // end for
}

Option *
zGetOpt::_find_opt(const std::string &opt_)
{
  Option *ret = NULL;
  std::map<std::string, Option>::iterator itr = this->_opts.begin();
  std::map<std::string, Option>::iterator end = this->_opts.end();
  for (; itr != end; itr++)
  {
    if (itr->second.GetName() == opt_)
    {
      ret = &itr->second;
    } // end if
  } // end if
  return (ret);
}

}
}
