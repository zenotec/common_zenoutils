/*
 * zCommand.h
 *
 *  Created on: Jan 10, 2016
 *      Author: kmahoney
 */

#ifndef _ZCOMMAND_H_
#define _ZCOMMAND_H_

#include <string>
#include <map>

#include <zutils/zEvent.h>
#include <zutils/zData.h>

namespace zUtils
{
namespace zCommand
{

class Command : public zEvent::EventObserver, private zData::Data
{
public:

  static const std::string ROOT;
  static const std::string NAME;
  static const std::string ARG;

  Command();

  Command(zData::Data &data_);

  virtual
  ~Command();

  bool
  operator ==(const Command &other_) const;

  bool
  operator !=(const Command &other_) const;

  std::string
  GetName();

  bool
  SetName(const std::string name_);

  std::string
  GetArgument();

  bool
  SetArgument(const std::string arg_);

  virtual bool
  Execute(const std::string &arg_ = "");

protected:

  virtual bool
  EventHandler(zEvent::Event *event_, void *arg_);

private:

};

class CommandMessage : public zMessage::Message
{
public:

  CommandMessage();

  virtual
  ~CommandMessage();

protected:

private:

};

class CommandEvent : public zEvent::Event
{
public:

  enum EVENTID
  {
    EVENTID_ERR = -1,
    EVENTID_NONE = 0,
    EVENTID_LAST
  };

  CommandEvent(const CommandEvent::EVENTID id_);

  virtual
  ~CommandEvent();

protected:

private:

};

class CommandHandler : public zEvent::EventHandler, private zEvent::EventObserver
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
  ProcessCommandString(const std::string &str_);

protected:

  virtual bool
  EventHandler(zEvent::Event *event_, void *arg_);

private:

  zSem::Mutex _lock;
  std::map<std::string, zCommand::Command *> _cmd_table;

};

}
}

#endif /* _ZCOMMAND_H_ */
