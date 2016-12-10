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

namespace zUtils
{
namespace zCommand
{

//**********************************************************************
// Class: CommandOption
//**********************************************************************

class CommandOption : public zData::Data
{

public:

  static const std::string ROOT;
  static const std::string OPT;
  static const std::string ARG;

  CommandOption(const std::string &opt_ = std::string(""), const std::string &arg_ = std::string(""));

  CommandOption(const zData::Data &data_);

  virtual
  ~CommandOption();

  std::string
  GetOption() const;

  bool
  SetOption(const std::string opt_);

  std::string
  GetArgument() const;

  bool
  SetArgument(const std::string arg_);

protected:

private:

};

//**********************************************************************
// Class: Command
//**********************************************************************

class Command : public zData::Data, public zEvent::Event
{
public:

  static const std::string ROOT;
  static const std::string NAME;
  static const std::string OUTPUT;

  Command(const std::string &name_ = std::string(""));

  Command(const zData::Data &data_);

  virtual
  ~Command();

  bool
  operator ==(const Command &other_);

  bool
  operator !=(const Command &other_);

  std::string
  GetName() const;

  bool
  SetName(const std::string name_);

  std::vector<CommandOption>
  GetOptions() const;

  bool
  AddOption(zCommand::CommandOption &opt_);

  std::string
  GetOutput() const;

  bool
  SetOutput(const std::string arg_);

  virtual bool
  Execute(zCommand::Command &cmd_);

protected:

  virtual bool
  EventHandler(const zEvent::EventNotification* notification_);

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

  CommandNotification(Command* cmd_);

  virtual
  ~CommandNotification();

protected:

private:

  CommandNotification::ID _id;

};

//**********************************************************************
// Class: CommandHandler
//**********************************************************************

class CommandHandler : public zEvent::EventHandler
{
public:

  CommandHandler();

  virtual
  ~CommandHandler();

  bool
  RegisterCommand(zCommand::Command *cmd_);

  bool
  UnregisterCommand(zCommand::Command *cmd_);

  bool
  ProcessCommand(zCommand::Command &cmd_);

protected:

private:

  zSem::Mutex _lock;
  std::map<std::string, zCommand::Command *> _cmd_table;

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
