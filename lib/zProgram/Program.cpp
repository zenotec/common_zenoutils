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

#include <zutils/zCompatibility.h>
#include <zutils/zProgram.h>

namespace zUtils
{
namespace zProgram
{

//**********************************************************************
// Class: Program
//**********************************************************************

Program::Program() :
    _main_thread(this, this), _exit_status(0)
{

  // Setup command line options
  zProgram::ProgramHelpOption HelpOpt;
  this->AddOption(HelpOpt);

  // Setup command line options
  zProgram::ProgramVersionOption VersionOpt;
  this->AddOption(VersionOpt);

}

Program::~Program()
{
}

ProgramOption&
Program::GetOption(const std::string& name_)
{
  return (this->_opts[name_]);
}

bool
Program::AddOption(const ProgramOption &opt_)
{
  bool status = true;
  this->_opts[opt_.GetLongName()] = opt_;
  return (status);
}

ProgramArgument&
Program::GetArgument(const unsigned int order_)
{
  return (this->_args[order_]);
}

bool
Program::AddArgument(const ProgramArgument &arg_)
{
  bool status = true;
  this->_args[arg_.GetPosition()] = arg_;
  return (status);
}

bool
Program::ParseCommandLine(int argc_, const char **argv_)
{

  bool status = true;

  std::string opt_name;
  ProgramOption *opt = NULL;
  ProgramArgument *arg = NULL;

  std::string program(argv_[0]);
  int pos = 0;

  this->_name = program.substr(program.find_last_of("/\\") + 1);

  for (int i = 0; i < argc_; i++)
  {
    if (argv_[i][0] == '-')
    {
      if (argv_[i][1] == '-')
      {
        opt_name = std::string(&argv_[i][2]);
      } // end if
      else
      {
        opt_name = std::string(&argv_[i][1]);
      } // end else
      if ((opt = this->_find_opt(opt_name)) != NULL)
      {
        if (!(opt->GetFlags() & ProgramOption::FLAGS_HAVEARG))
        {
          opt->AddArgument(std::string(""));
          opt = NULL; // reset option pointer
        }
      }
      else
      {
        std::cerr << "Unexpected option: " << opt_name << std::endl;
        status = false;
        break;
      }
    } // end if
    else
    {
      if (opt != NULL)
      {
        if (opt->GetFlags() & ProgramOption::FLAGS_HAVEARG)
        {
          opt->AddArgument(std::string(&argv_[i][0]));
        }
        opt = NULL; // reset option pointer
      } // end if
      else
      {
        arg = this->_find_arg(pos);
        if (arg)
        {
          arg->SetValue(std::string(&argv_[i][0]));
        }
      }
    } // end else

  } // end for

  return (status);
}

std::string
Program::Name() const
{
  return (this->_name);
}

std::string
Program::Usage() const
{

  std::string usage = std::string("USAGE: ") + this->_name + std::string(" [");

  FOREACH (auto& opt, this->_opts)
  {
    usage += opt.second.GetShortName();
    if (opt.second.GetFlags() & ProgramOption::FLAGS_HAVEARG)
    {
      usage += std::string(":");
    }
  }

  usage += std::string("]");

  FOREACH(auto& arg, this->_args)
  {
    usage += std::string(" ") + arg.second.GetName();
  }

  usage += std::string("\n");

  FOREACH (auto& opt, this->_opts)
  {

    usage += std::string("\t");

    if (isprint(opt.second.GetShortName()))
    {
      usage += std::string("-") + opt.second.GetShortName();
      if (!opt.second.GetLongName().empty())
      {
        usage += std::string(",");
      }
      else
      {
        usage += std::string("\t");
      }
    }

    if (!opt.second.GetLongName().empty())
    {
      usage += std::string("--") + opt.second.GetLongName() + std::string("\t");
    }

    if (opt.second.GetFlags() & ProgramOption::FLAGS_HAVEARG)
    {
      if (!(opt.second.GetFlags() & ProgramOption::FLAGS_ARG_ISOPTIONAL))
      {
        usage += std::string("arg");
      }
      else
      {
        usage += std::string("[ arg ]");
      }
    }
    usage += std::string("\t");

    usage += opt.second.GetDescription() + "\n";
  }

  return (usage);
}

bool
Program::HelpOption()
{
  return (!this->GetOption(ProgramHelpOption::OptStr).Empty());
}

bool
Program::VersionOption()
{
  return (!this->GetOption(ProgramVersionOption::OptStr).Empty());
}

std::string
Program::LogFile()
{
  return(this->GetOption(ProgramLogFileOption::OptStr).GetArguments()[0]);
}

zLog::Log::LEVEL
Program::LogLevel()
{
  return((zLog::Log::LEVEL)zToInt(this->GetOption(ProgramLogLevelOption::OptStr).GetArguments()[0]));
}

bool
Program::EnableLogging()
{

}

int
Program::GetExitStatus() const
{
  return(this->_exit_status);
}

bool
Program::SetExitStatus(int status_)
{
  this->_exit_status = status_;
  return (true);
}

bool
Program::Run()
{
  bool status = false;
  if (this->_main_thread.Start())
  {
    status = this->_main_thread.Join();
  }
  return(status);
}

ProgramOption *
Program::_find_opt(const std::string &opt_)
{
  ProgramOption *ret = NULL;
  FOREACH (auto& opt, this->_opts)
  {
    if (opt_.size() == 1)
    {
      if (opt.second.GetShortName() == opt_[0])
      {
        ret = &opt.second;
      }
    }
    else
    {
      if (opt.second.GetLongName() == opt_)
      {
        ret = &opt.second;
      } // end if
    }
  } // end for
  return (ret);
}

ProgramArgument *
Program::_find_arg(const int pos)
{
  ProgramArgument *ret = NULL;
  FOREACH (auto& arg, this->_args)
  {
    if (arg.second.GetPosition() == pos)
    {
      ret = &arg.second;
      break;
    }
  } // end for
  return (ret);
}

}
}
