/*
 * zCommandMessage.h
 *
 *  Created on: Jan 10, 2016
 *      Author: kmahoney
 */

#ifndef _ZCOMMANDMESSAGE_H_
#define _ZCOMMANDMESSAGE_H_

namespace zUtils
{
namespace zCommand
{

//**********************************************************************
// Class: CommandMessage
//**********************************************************************

class CommandMessage : public zMessage::Message
{
public:

  CommandMessage();

  CommandMessage(const zCommand::Command &command_);

  virtual
  ~CommandMessage();

  bool
  SetCommand(const zCommand::Command &command_);

protected:

private:

};

}
}

#endif /* _ZCOMMANDMESSAGE_H_ */
