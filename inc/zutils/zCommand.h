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

#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zMessage.h>
#include <zutils/zMessageSocket.h>

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
  static const std::string NameDataPath;
  static const std::string ArgDataPath;

  CommandOptionPath(const std::string& root_ = std::string(""));

  CommandOptionPath(const CommandOptionPath& other_);

  CommandOptionPath(const zData::DataPath& path_);

  virtual
  ~CommandOptionPath();

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

  CommandOption(CommandOption &other_);

  CommandOption(const CommandOption &other_);

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
  static const std::string NameDataPath;
  static const std::string OptionDataPath;
  static const std::string OutputDataPath;

  CommandPath(const std::string& root_ = std::string(""));

  CommandPath(const CommandPath& other_);

  CommandPath(const zData::DataPath& path_);

  virtual
  ~CommandPath();

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

  CommandData(const zData::Data &data_);

  CommandData(CommandData &data_);

  CommandData(const CommandData &data_);

  virtual
  ~CommandData();

  CommandData &
  operator=(const CommandData &other_);

  bool
  operator ==(const CommandData &other_) const;

  bool
  operator !=(const CommandData &other_) const;

  zData::Data&
  GetData();

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

  CommandData&
  GetCommandData();

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

  bool
  EventHandler(zEvent::EventNotification* notification_);

private:

  bool
  EventHandler(zMessage::MessageNotification* notification_);

  bool
  EventHandler(CommandNotification* notification_);

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
