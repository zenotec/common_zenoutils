/******************************************************************************
 *
 *
 *
 *****************************************************************************/

#ifndef __ZPROGOPT_H__
#define __ZPROGOPT_H__

#include <stdint.h>
#include <unistd.h>

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

  Option(uint32_t flags_ = Option::FLAGS_NONE, const std::string &name_ = std::string(""));
  ~Option();

  uint32_t Flags() const;

  std::string
  Name() const;

  template<typename T>
    T
    Arg();

  std::string
  HelpMsg() const;

  void
  HelpMsg(const std::string &msg_);

  template<typename T>
    bool
    Default(T default_);

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
  std::string _name;
  std::string _help;
  std::string _arg;

};

class Table
{

public:
  Table();
  ~Table();

  Option &
  operator[](const std::string &id_);

  bool
  AddOption(const Option &opt_);

  bool
  Parse(int argc_, const char **argv_);

  ssize_t
  Count(const std::string &opt_);

  std::string
  Usage();

  std::string
  ErrorString();

protected:

private:
  Option *
  _find_opt(const std::string &opt_);

  std::string _errStr;
  std::map<std::string, Option> _opts;

};

}
}

#endif
