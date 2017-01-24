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
#include <zutils/zLoopSocket.h>
#include <zutils/zUnixSocket.h>

#include <zutils/zMessage.h>
#include <zutils/zMessageSocket.h>

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
zMessageTest_MessageLoopSocket(void* arg_);
int
zMessageTest_MessageUnixSocket(void* arg_);

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
  EventHandler(zEvent::EventNotification* notification_)
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
        this->RxSem.Push(n->GetMessage());
        status = true;
        break;
      case zMessage::MessageNotification::ID_MSG_SENT:
        this->TxSem.Push(n->GetMessage());
        status = true;
        break;
      default:
        this->ErrSem.Push(n->GetMessage());
        status = false;
        break;
      }
    }
    return (status);
  }

private:

};

#endif /* _ZMESSAGETEST_H_ */
