/*
 * zMessageTest.h
 *
 *  Created on: Jan 9, 2015
 *      Author: kmahoney
 */

#ifndef _ZMESSAGETEST_H_
#define _ZMESSAGETEST_H_

#include "UnitTest.h"

#include <zutils/zQueue.h>
#include <zutils/zEvent.h>
#include <zutils/zMessage.h>

int
zMessageTest_MessageDefaults(void* arg_);
int
zMessageTest_MessageSocketDefaults(void* arg_);

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
zMessageTest_MessageSocket(void* arg_);

int
zMessageTest_MessageHandler(void* arg_);

using namespace zUtils;
using namespace Test;

class TestSocketObserver : public zEvent::EventObserver
{
public:
  TestSocketObserver()
  {

  }
  virtual
  ~TestSocketObserver()
  {

  }

  zSem::Semaphore RxSem;
  zSem::Semaphore TxSem;
  zSem::Semaphore ErrSem;

protected:
  virtual bool
  EventHandler(zEvent::Event *event_, void *arg_)
  {
    if (event_ && arg_ && (event_->GetType() == zEvent::Event::TYPE_MSG))
    {
      zMessage::MessageEvent::EVENTID id = (zMessage::MessageEvent::EVENTID)event_->GetId();
      switch(id)
      {
      case zMessage::MessageEvent::EVENTID_MSG_RCVD:
      {
        zMessage::Message *message = static_cast<zMessage::Message *>(arg_);
        ZLOG_DEBUG("Received message: " + zLog::IntStr(message->GetType()));
        this->RxSem.Post();
        break;
      }
      case zMessage::MessageEvent::EVENTID_MSG_SENT:
      {
        this->TxSem.Post();
        break;
      }
      case zMessage::MessageEvent::EVENTID_MSG_ERR:
      {
        this->ErrSem.Post();
        break;
      }
      }
    }
    return (true);
  }

private:
};

class TestHandler : public zEvent::EventObserver
{
public:

  zSem::Semaphore RxSem[zMessage::Message::TYPE_LAST];
  zSem::Semaphore TxSem;
  zSem::Semaphore ErrSem;

  TestHandler(zMessage::MessageSocket *sock_) :
  _hello_handler(zMessage::Message::TYPE_HELLO, sock_),
  _ack_handler(zMessage::Message::TYPE_ACK, sock_),
  _bye_handler(zMessage::Message::TYPE_BYE, sock_)
  {
    ZLOG_DEBUG("New test handler created");
    this->_hello_handler.RegisterObserver(this);
    this->_ack_handler.RegisterObserver(this);
    this->_bye_handler.RegisterObserver(this);
  }

  virtual
  ~TestHandler()
  {
    ZLOG_DEBUG("Test handler deleted");
    this->_hello_handler.UnregisterObserver(this);
    this->_ack_handler.UnregisterObserver(this);
    this->_bye_handler.UnregisterObserver(this);
  }

protected:

  virtual bool
  EventHandler(zEvent::Event *event_, void *arg_)
  {
    if (event_ && arg_ && (event_->GetType() == zEvent::Event::TYPE_MSG))
    {
      zMessage::MessageEvent::EVENTID id = (zMessage::MessageEvent::EVENTID)event_->GetId();
      switch(id)
      {
      case zMessage::MessageEvent::EVENTID_MSG_RCVD:
      {
        ZLOG_DEBUG("Received message");
        zMessage::Message *msg = static_cast<zMessage::Message *>(arg_);
        this->RxSem[msg->GetType()].Post();
        break;
      }
      case zMessage::MessageEvent::EVENTID_MSG_SENT:
      {
        ZLOG_DEBUG("Message sent");
        this->TxSem.Post();
        break;
      }
      case zMessage::MessageEvent::EVENTID_MSG_ERR:
      {
        ZLOG_DEBUG("Message error");
        this->ErrSem.Post();
        break;
      }
      }
    }
    return (true);
  }

private:

  zMessage::MessageHandler _hello_handler;
  zMessage::MessageHandler _ack_handler;
  zMessage::MessageHandler _bye_handler;

};

#endif /* _ZMESSAGETEST_H_ */
