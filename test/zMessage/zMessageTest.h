/*
 * zMessageTest.h
 *
 *  Created on: Jan 9, 2015
 *      Author: kmahoney
 */

#ifndef _ZMESSAGETEST_H_
#define _ZMESSAGETEST_H_

#include <stdint.h>
#include <netinet/in.h>
#include <string.h>

#include <string>
#include <list>
#include <mutex>
#include <memory>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>
#include <zutils/zData.h>
#include <zutils/zSocket.h>

#include <zutils/zMessage.h>

#include "UnitTest.h"


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

  zQueue<zMessage::Message*> RxSem;
  zQueue<zMessage::Message*> TxSem;
  zQueue<zMessage::Message*> ErrSem;

protected:

  virtual bool
  EventHandler(const zEvent::EventNotification* notification_)
  {
    zMessage::MessageNotification *n = NULL;
    ZLOG_DEBUG("Handling socket event");

    bool status = false;
    if (notification_ && (notification_->Type() == zEvent::Event::TYPE_MSG))
    {
      n = (zMessage::MessageNotification *) notification_;
      switch (n->Id())
      {
      case zMessage::MessageNotification::ID_MSG_RCVD:
        this->RxSem.Push(n->Message());
        status = true;
        break;
      case zMessage::MessageNotification::ID_MSG_SENT:
        this->TxSem.Push(n->Message());
        status = true;
        break;
      default:
        this->ErrSem.Push(n->Message());
        status = false;
        break;
      }
    }
    return (status);
  }

private:

};

#endif /* _ZMESSAGETEST_H_ */
