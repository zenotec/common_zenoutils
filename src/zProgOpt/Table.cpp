//*****************************************************************************
//
//
//
//*****************************************************************************
#include <iostream>

#include <zutils/zProgOpt.h>

namespace zUtils
{
namespace zProgOpt
{

//*****************************************************************************
//*****************************************************************************
Table::Table()
{
}

Table::~Table()
{

}

Option &
Table::operator[](const std::string &id_)
{
  return (this->_opts[id_]);
}

bool
Table::AddOption(const Option &opt_)
{
  bool ret = true;
  this->_opts[opt_.Name()] = opt_;
  return (ret);
}

bool
Table::Parse(int argc_, const char **argv_)
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
        opt->_cnt++;
      } // end if
    } // end if
    else
    {
      if (opt != NULL)
      {
        opt->_setArg(std::string(argv_[arg]));
        opt = NULL; // reset arg pointer
      } // end if
    } // end else
  } // end for
  return(true);
}

ssize_t
Table::Count(const std::string& opt_)
{
  ssize_t cnt = -1;
  Option *opt = this->_find_opt(opt_);
  if (opt)
  {
    cnt = opt->_cnt;
  }
  return (cnt);
}

std::string
Table::Usage()
{
  std::string usage;
  std::map<std::string, Option>::iterator it;
  for (it = this->_opts.begin(); it != this->_opts.end(); ++it)
  {
    usage += it->second.Name() + "\t";
    if (it->second.Flags() & Option::FLAGS_HAVEARG)
    {
      if (!(it->second.Flags() & Option::FLAGS_ARG_ISOPTIONAL))
      {
        usage += std::string("arg");
      }
      else
      {
        usage += std::string("[ arg ]");
      }
    }
    usage += std::string("\t");

    usage += it->second.HelpMsg() + "\n";
  }
  return (usage);
}

std::string
Table::ErrorString()
{
  return (this->_errStr);
}

Option *
Table::_find_opt(const std::string &opt_)
{
  Option *ret = NULL;
  std::map<std::string, Option>::iterator it;
  for (it = this->_opts.begin(); it != this->_opts.end(); ++it)
  {
    if (it->second.Name() == opt_)
    {
      ret = &it->second;
    } // end if
  } // end if
  return (ret);
}

}
}
