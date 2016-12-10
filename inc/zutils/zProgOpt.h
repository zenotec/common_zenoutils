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

namespace zUtils
{
namespace zProgOpt
{

class Table;

class Option
{

  friend class Table;

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

  Option(uint32_t flags_ = Option::FLAGS_NONE);

  virtual
  ~Option();

  uint32_t
  Flags() const;

  const char
  ShortName() const;

  bool
  ShortName(const char name_);

  std::string
  LongName() const;

  bool
  LongName(const std::string& name_);

  template<typename T>
    T
    Arg()
    {
      T arg;
      this->_getArg(arg);
      return (arg);
    }

  std::string
  HelpMsg() const;

  void
  HelpMsg(const std::string &msg_);

  template<typename T>
    bool
    Default(T default_)
    {
      std::stringstream ss;
      ss << default_;
      this->_arg = ss.str();
      return (true);
    }

protected:

  bool
  _setArg(const std::string &arg_);

  bool
  _setFlags(uint32_t flags_);

  uint32_t _cnt;

private:

  void
  _getArg(std::string &arg_);

  void
  _getArg(int &arg_);

  void
  _getArg(unsigned int &arg_);

  void
  _getArg(double &arg_);

  uint32_t _flags;
  char _short_name;
  std::string _long_name;
  std::string _help;
  std::string _arg;

};

class Table
{

public:
  Table();

  virtual
  ~Table();

  Option &
  operator[](const std::string &id_);

  bool
  AddOption(const Option &opt_);

  std::string
  Name();

  std::string
  Usage();

  bool
  Parse(int argc_, const char **argv_);

  ssize_t
  Count(const std::string &opt_);

  std::string
  ErrorString();

protected:

private:

  Option *
  _find_opt(const std::string &opt_);

  std::string _name;
  std::string _errStr;
  std::map<std::string, Option> _opts;

};

}
}

#endif /* __ZPROGOPT_H__ */
