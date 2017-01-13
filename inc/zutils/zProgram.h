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

#ifndef __ZPROGOPT_H__
#define __ZPROGOPT_H__

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include <zutils/zUtils.h>
#include <zutils/zLog.h>
#include <zutils/zThread.h>

namespace zUtils
{
namespace zProgram
{

//**********************************************************************
// Class: ProgramOption
//**********************************************************************

class ProgramOption
{

public:

  enum FLAGS
  {
    FLAGS_ERR = -1,
    FLAGS_NONE = 0,
    FLAGS_ISOPTIONAL = (1 << 0),
    FLAGS_HAVEARG = (1 << 1),
    FLAGS_ARG_ISOPTIONAL = (1 << 2),
    FLAGS_LAST = (1 << 2),
  };

  ProgramOption();

  virtual
  ~ProgramOption();

  ssize_t
  Count() const;

  bool
  Empty() const;

  uint32_t
  GetFlags() const;

  bool
  SetFlags(const uint32_t flags_);

  const char
  GetShortName() const;

  bool
  SetShortName(const char name_);

  const std::string&
  GetLongName() const;

  bool
  SetLongName(const std::string& name_);

  const std::string&
  GetDescription() const;

  void
  SetDescription(const std::string &msg_);

  std::string
  GetDefault() const;

  bool
  SetDefault(const std::string& default_);

  const std::vector<std::string>&
  GetArguments() const;

  bool
  AddArgument(const std::string& arg_);

protected:

private:

  uint32_t _flags;
  char _short_name;
  std::string _long_name;
  std::string _desc;
  std::vector<std::string> _default;
  std::vector<std::string> _args;

};

//**********************************************************************
// Class: ProgramHelpOption
//**********************************************************************

class ProgramHelpOption : public ProgramOption
{

public:

  static const std::string OptStr;

  ProgramHelpOption();

  virtual
  ~ProgramHelpOption();

protected:

private:

};

//**********************************************************************
// Class: ProgramVersionOption
//**********************************************************************

class ProgramVersionOption : public ProgramOption
{

public:

  static const std::string OptStr;

  ProgramVersionOption();

  virtual
  ~ProgramVersionOption();

protected:

private:

};

//**********************************************************************
// Class: ProgramLogFileOption
//**********************************************************************

class ProgramLogFileOption : public ProgramOption
{

public:

  static const std::string OptStr;

  ProgramLogFileOption();

  virtual
  ~ProgramLogFileOption();

protected:

private:

};

//**********************************************************************
// Class: ProgramLogLevelOption
//**********************************************************************

class ProgramLogLevelOption : public ProgramOption
{

public:

  static const std::string OptStr;

  ProgramLogLevelOption();

  virtual
  ~ProgramLogLevelOption();

protected:

private:

};

//**********************************************************************
// Class: ProgramArgument
//**********************************************************************

class ProgramArgument
{

public:

  enum FLAGS
  {
    FLAGS_ERR = -1,
    FLAGS_NONE = 0,
    FLAGS_ISOPTIONAL = (1 << 0),
    FLAGS_LAST = (1 << 1),
  };

  ProgramArgument();

  virtual
  ~ProgramArgument();

  ssize_t
  Count() const;

  bool
  Empty() const;

  uint32_t
  GetFlags() const;

  bool
  SetFlags(const uint32_t flags_);

  unsigned int
  GetPosition() const;

  bool
  SetPosition(const unsigned int pos_);

  std::string
  GetName() const;

  bool
  SetName(const std::string& name_);

  std::string
  GetDescription() const;

  bool
  SetDescription(const std::string& desc_);

  std::string
  GetDefault() const;

  bool
  SetDefault(const std::string& default_);

  std::string
  GetValue() const;

  bool
  SetValue(const std::string& value_);

protected:

private:

  uint32_t _flags;
  unsigned int _pos;
  std::string _name;
  std::string _desc;
  std::string _default;
  std::string _value;

};

//**********************************************************************
// Class: Program
//**********************************************************************

class Program : public zThread::ThreadFunction, public zThread::ThreadArg
{

public:

  Program();

  virtual
  ~Program();

  ProgramOption&
  GetOption(const std::string& name_);

  bool
  AddOption(const ProgramOption &opt_);

  ProgramArgument&
  GetArgument(const unsigned int order_);

  bool
  AddArgument(const ProgramArgument &arg_);

  bool
  ParseCommandLine(int argc_, const char **argv_);

  std::string
  Name() const;

  std::string
  Usage() const;

  bool
  HelpOption();

  bool
  VersionOption();

  bool
  EnableLogging();

  std::string
  LogFile();

  zLog::LogLevel
  LogLevel();

  bool
  Run();

  virtual void
  Run(zThread::ThreadArg *arg_) = 0;

protected:

private:

  std::string _name;
  std::map<std::string, ProgramOption> _opts;
  std::map<int, ProgramArgument> _args;
  zThread::Thread _main_thread;

  ProgramOption *
  _find_opt(const std::string &opt_);

  ProgramArgument *
  _find_arg(const int pos);

};

}
}

#endif /* __ZPROGOPT_H__ */
