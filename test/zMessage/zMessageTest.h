/*
 * zMessageTest.h
 *
 *  Created on: Jan 9, 2015
 *      Author: kmahoney
 */

#ifndef _ZMESSAGETEST_H_
#define _ZMESSAGETEST_H_

#include "UnitTest.h"

#include <zutils/zMessage.h>

int
zMessageTest_Defaults(void* arg_);

int
zMessageTest_FactoryError(void* arg_);
int
zMessageTest_FactoryAuth(void* arg_);
int
zMessageTest_FactoryHello(void* arg_);
int
zMessageTest_FactoryAck(void* arg_);
int
zMessageTest_FactoryBye(void* arg_);
int
zMessageTest_FactoryCfg(void* arg_);
int
zMessageTest_FactoryCmd(void* arg_);
int
zMessageTest_FactoryData(void* arg_);

int
zMessageTest_GetSet(void* arg_);

int
zMessageTest_Handler(void* arg_);

using namespace zUtils;
using namespace Test;

class TestHandler : public zMessage::Handler, public zMessage::Observer
{
public:

  TestHandler()
  {
    zMessage::Handler::Register(zMessage::Message::TYPE_HELLO, this);
    zMessage::Handler::Register(zMessage::Message::TYPE_ACK, this);
    zMessage::Handler::Register(zMessage::Message::TYPE_BYE, this);
  }

  virtual
  ~TestHandler()
  {
    zMessage::Handler::Unregister(zMessage::Message::TYPE_HELLO, this);
    zMessage::Handler::Unregister(zMessage::Message::TYPE_ACK, this);
    zMessage::Handler::Unregister(zMessage::Message::TYPE_BYE, this);
  }

  virtual bool
  RecvMsg(zMessage::Handler &handler_, zMessage::Message &msg_)
  {
    return(false);
  }

protected:

private:

};

#endif /* _ZMESSAGETEST_H_ */
