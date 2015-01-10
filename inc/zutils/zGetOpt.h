/******************************************************************************
 *
 *
 *
 *****************************************************************************/

#ifndef __ZGETOPT_H__
#define __ZGETOPT_H__

#include <string>
#include <map>

namespace zUtils
{
namespace zOpt
{


class Option
{

public:
  Option(const std::string &name_ = "", const std::string &desc_ = "",
      const std::string &default_ = "");
  ~Option();

  std::string
  GetName() const;
  std::string
  GetDesc() const;
  void
  SetDesc(const std::string &desc_);
  std::string
  GetArg() const;
  void
  SetArg(const std::string &arg_);
  bool
  GetPresent();
  void
  SetPresent(bool present_);

protected:

private:
  std::string _name;
  std::string _desc;
  std::string _arg;
  bool _present;

};

class zGetOpt
{

public:
  zGetOpt();
  ~zGetOpt();

  Option &
  operator[](const std::string &id_);

  void
  SetUsage(const std::string &usage_);
  std::string
  Usage();
  bool
  AddOption(const Option &opt_);
  bool
  Parse(int argc_, const char **argv_);

protected:

private:
  Option *
  _find_opt(const std::string &opt_);

  std::string _usage;
  std::map<std::string, Option> _opts;

};

}
}

#endif
