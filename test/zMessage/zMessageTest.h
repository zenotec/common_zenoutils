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
zMessageTest_MessageGetSet(void* arg_);
int
zMessageTest_MessageCopy(void* arg_);

int
zMessageTest_Handler(void* arg_);

using namespace zUtils;
using namespace Test;

class TestHandler : public zMessage::Handler, public zMessage::Observer
{
public:

  TestHandler() :
      _cnt(0)
  {
    ZLOG_DEBUG("TestHandler::TestHandler(): New test handler created");
    this->_cnt = new uint32_t[zMessage::Message::TYPE_LAST];
    zMessage::Handler::Register(zMessage::Message::TYPE_HELLO, this);
    zMessage::Handler::Register(zMessage::Message::TYPE_ACK, this);
    zMessage::Handler::Register(zMessage::Message::TYPE_BYE, this);
  }

  virtual
  ~TestHandler()
  {
    ZLOG_DEBUG("TestHandler::~TestHandler(): Test handler deleted");
    zMessage::Handler::Unregister(zMessage::Message::TYPE_HELLO, this);
    zMessage::Handler::Unregister(zMessage::Message::TYPE_ACK, this);
    zMessage::Handler::Unregister(zMessage::Message::TYPE_BYE, this);
    delete[] (this->_cnt);
  }

  virtual bool
  MessageRecv(zMessage::Handler &handler_, zMessage::Message &msg_)
  {
    ZLOG_DEBUG("TestHandler::MessageRecv(): Received message");
    this->_cnt[msg_.GetType()]++;
    this->_sem.Post();
    return (true);
  }

  bool
  WaitForMessage(uint32_t msecs_)
  {
    return (this->_sem.TimedWait(msecs_));
  }

  uint32_t
  GetCount(zMessage::Message::TYPE type_)
  {
    return (this->_cnt[type_]);
  }

protected:

private:

  zSem::Semaphore _sem;
  uint32_t *_cnt;

};

#endif /* _ZMESSAGETEST_H_ */
