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

#ifndef __ZCOMMAND_H__
#define __ZCOMMAND_H__

#include <string>
#include <map>

namespace zUtils
{
namespace zCommand
{

//**********************************************************************
// Class: CommandOptionPath
//**********************************************************************

class CommandOptionPath : public zData::DataPath
{

public:

  static const std::string DataRoot;
  static const std::string DataNamePath;
  static const std::string DataArgPath;

  CommandOptionPath();

  virtual
  ~CommandOptionPath();

  zData::DataPath
  Name();

  zData::DataPath
  Argument();

protected:

private:

};

//**********************************************************************
// Class: CommandOption
//**********************************************************************

class CommandOption : public zData::Data
{

public:

  CommandOption();

  CommandOption(const zData::Data &data_);

  virtual
  ~CommandOption();

  zData::Data&
  GetData();

  std::string
  GetName() const;

  bool
  SetName(const std::string opt_);

  std::string
  GetArgument() const;

  bool
  SetArgument(const std::string arg_);

protected:

private:

};

//**********************************************************************
// Class: CommandPath
//**********************************************************************

class CommandPath : public zData::DataPath
{

public:

  static const std::string DataRoot;
  static const std::string DataNamePath;
  static const std::string DataOutputPath;

  CommandPath();

  virtual
  ~CommandPath();

  zData::DataPath
  Name();

  zData::DataPath
  Option();

  zData::DataPath
  Output();

protected:

private:

};

//**********************************************************************
// Class: CommandData
//**********************************************************************

class CommandData : public zData::Data
{

public:

  CommandData();

  CommandData(zData::Data &data_);

  CommandData(const zData::Data &data_);

  CommandData(CommandData &data_);

  CommandData(const CommandData &data_);

  virtual
  ~CommandData();

  CommandData &
  operator=(CommandData &other_)
  {
    zData::Data::operator =(other_);
    return (*this);
  }

  CommandData &
  operator=(const CommandData &other_)
  {
    zData::Data::operator =(other_);
    return (*this);
  }

  bool
  operator ==(const CommandData &other_) const;

  bool
  operator !=(const CommandData &other_) const;

  std::string
  GetName() const;

  bool
  SetName(const std::string name_);

  std::map<std::string, CommandOption>
  GetOptions() const;

  bool
  AddOption(zCommand::CommandOption &opt_);

  std::string
  GetOutput() const;

  bool
  SetOutput(const std::string arg_);

protected:

private:

};

//**********************************************************************
// Class: CommandNotification
//**********************************************************************

class CommandNotification : public zEvent::EventNotification
{
public:

  enum ID
  {
    ID_ERR = -1,
    ID_NONE = 0,
    ID_LAST
  };

  CommandNotification();

  virtual
  ~CommandNotification();

  const CommandData&
  GetCommandData() const;

  bool
  SetCommandData(const CommandData& data_);

protected:

private:

  CommandNotification::ID _id;
  CommandData _data;

};

//**********************************************************************
// Class: Command
//**********************************************************************

class Command : public CommandData, public zEvent::EventObserver
{

public:

  Command();

  Command(const CommandData& data_);

  virtual
  ~Command();

  virtual bool
  Execute(CommandData& data_) = 0;

protected:

  virtual bool
  EventHandler(const zEvent::EventNotification* notification_);

private:

};

//**********************************************************************
// Class: CommandManager
//**********************************************************************

class CommandManager : public zEvent::EventHandler
{

public:

  static CommandManager&
  Instance()
  {
    static CommandManager instance;
    return instance;
  }

protected:

private:

  CommandManager()
  {
  }

  CommandManager(CommandManager const&);

  void
  operator=(CommandManager const&);

};

}
}

#endif /* __ZCOMMAND_H__ */
