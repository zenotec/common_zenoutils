/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <libgen.h>

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
  this->_opts[opt_.LongName()] = opt_;
  return (ret);
}

bool
Table::Parse(int argc_, const char **argv_)
{
  Option *opt = NULL;

  std::string program(argv_[0]);

  this->_name = program.substr(program.find_last_of("/\\") + 1);

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
        opt->_setArg(std::string(&argv_[arg][0]));
        opt = NULL; // reset option pointer
      } // end if
    } // end else
  } // end for
  return (true);
}

std::string
Table::Name()
{
  return (this->_name);
}

std::string
Table::Usage()
{

  std::string usage;
  std::map<std::string, Option>::iterator it = this->_opts.begin();
  std::map<std::string, Option>::iterator end = this->_opts.end();

  usage = std::string("USAGE: ") + this->_name + std::string("\n");

  for (; it != end; ++it)
  {

    usage += std::string("\t");

    if (isprint(it->second.ShortName()))
    {
      usage += std::string("-") + it->second.ShortName();
      if (!it->second.LongName().empty())
      {
        usage += std::string(",");
      }
      else
      {
        usage += std::string("\t");
      }
    }

    if (!it->second.LongName().empty())
    {
      usage += std::string("--") + it->second.LongName() + std::string("\t");
    }

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
    if (opt_.size() == 1)
    {
      if (it->second.ShortName() == opt_[0])
      {
        ret = &it->second;
      }
    }
    else
    {
      if (it->second.LongName() == opt_)
      {
        ret = &it->second;
      } // end if
    }
  } // end if
  return (ret);
}

}
}
