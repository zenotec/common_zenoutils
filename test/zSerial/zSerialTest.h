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

#ifndef _ZCOMTEST_H_
#define _ZCOMTEST_H_

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

#include "UnitTest.h"

#include <zutils/zLog.h>
#include <zutils/zThread.h>
#include <zutils/zSerial.h>

#include <zutils/zEchoSerialPort.h>
#include <zutils/zTtySerialPort.h>

using namespace zUtils;

int
zSerialTest_ConfigPathDefaults(void* arg);
int
zSerialTest_ConfigDataDefaults(void* arg);
int
zSerialTest_SerialConfigDataCtor(void* arg);
int
zSerialTest_SerialConfigDataGetSet(void* arg);

int
zSerialTest_PortDefaults(void* arg_);
int
zSerialTest_PortSendRecvChar(void *arg_);
int
zSerialTest_PortSendRecvBuf(void *arg_);

int
zSerialTest_TtyPortDefaults(void* arg_);
int
zSerialTest_TtyConfigDataCtor(void* arg);
int
zSerialTest_TtyConfigDataGetSet(void* arg);
int
zSerialTest_TtyConfigPortGetSet(void* arg);
int
zSerialTest_TtyPortSendRecvChar(void *arg_);
int
zSerialTest_TtyPortSendRecvBuf(void *arg_);

class TestObserver : public zEvent::Observer
{
public:
  TestObserver()
  {
  }

  virtual
  ~TestObserver()
  {
  }

  zQueue<char> RxSem;
  zQueue<char> TxSem;
  zQueue<char> ErrSem;

protected:

  virtual bool
  EventHandler(zEvent::Notification* notification_)
  {
    bool status = false;
    if (notification_ && (notification_->GetType() == zEvent::Event::TYPE_SERIAL))
    {
      zSerial::SerialNotification *n = (zSerial::SerialNotification *) notification_;
      switch (n->Id())
      {
      case zSerial::SerialNotification::ID_CHAR_RCVD:
        ZLOG_DEBUG("zSerialTest::EventHandler(): ID_CHAR_RCVD: " + zLog::CharStr(n->Data()))
        ;
        this->RxSem.Push(n->Data());
        status = true;
        break;
      case zSerial::SerialNotification::ID_CHAR_SENT:
        ZLOG_DEBUG("zSerialTest::EventHandler(): ID_CHAR_SENT: " + zLog::CharStr(n->Data()))
        ;
        this->TxSem.Push(n->Data());
        status = true;
        break;
      default:
        this->ErrSem.Push(n->Data());
        status = false;
        break;
      }
    }
    return (status);
  }

private:

};

class TestPort : public zSerial::EchoSerialPort
{
public:

  TestPort()
  {
    ZLOG_INFO("TestEchoPort::TestEchoPort: Creating test port");
  }

  virtual
  ~TestPort()
  {
    ZLOG_INFO("TestEchoPort::~TestEchoPort: Destroying test port");
  }

protected:

private:

};

class TtyTestPort;

class TtyTestThread : public zThread::ThreadFunction
{
public:

  TtyTestThread(TtyTestPort *port_);

  virtual
  ~TtyTestThread();

protected:

  virtual void
  Run(zThread::ThreadArg *arg_);

private:

  TtyTestPort* _port;

};

class TtyTestPort : public zSerial::TtySerialPort
{

  friend TtyTestThread;

public:

  TtyTestPort() :
      _func(this), _thread(&this->_func, NULL)
  {

    int ret = 0;

    ZLOG_INFO("Created new TTY test port");

    this->_master_fd = posix_openpt(O_RDWR | O_NOCTTY);
    if (this->_master_fd < 0)
    {
      std::string errmsg = "Error opening pseudo terminal";
      ZLOG_ERR(errmsg);
      throw errmsg;
    }

    ret = grantpt(this->_master_fd);
    if (ret < 0)
    {
      std::string errmsg = "Error setting permissions on master pseudo terminal";
      ZLOG_ERR(errmsg);
      throw errmsg;
    }

    ret = unlockpt(this->_master_fd);
    if (ret < 0)
    {
      std::string errmsg = "Error setting permissions on slave pseudo terminal";
      ZLOG_ERR(errmsg);
      throw errmsg;
    }

    this->_slave_dev = std::string(ptsname(this->_master_fd));
    this->SetDevice(this->_slave_dev);

    this->_thread.Start();

  }

  virtual
  ~TtyTestPort()
  {
    ZLOG_INFO("Destroying TTY test port");
    this->_thread.Stop();
    close(this->_master_fd);
  }

protected:

  int _master_fd;

private:

  std::string _slave_dev;

  TtyTestThread _func;
  zThread::Thread _thread;

};

#endif /* _ZCOMTEST_H_ */
