/*
 * zSerialTest.h
 *
 *  Created on: Dec 9, 2015
 *      Author: kmahoney
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
#include <zutils/zTty.h>

using namespace zUtils;

int
zSerialTest_PortDefaults(void* arg_);
int
zSerialTest_PortSendRecvChar(void *arg_);
int
zSerialTest_PortSendRecvBuf(void *arg_);
int
zSerialTest_TtyPortDefaults(void* arg_);
int
zSerialTest_TtyPortSendRecvChar(void *arg_);
int
zSerialTest_TtyPortSendRecvBuf(void *arg_);

class TestObserver : public zEvent::EventObserver
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
  EventHandler(const zEvent::EventNotification* notification_)
  {
    bool status = false;
    if (notification_ && (notification_->Type() == zEvent::Event::TYPE_SERIAL))
    {
      zSerial::SerialNotification *n = (zSerial::SerialNotification *)notification_;
      switch(n->Id())
      {
      case zSerial::SerialNotification::ID_CHAR_RCVD:
        this->RxSem.Push(n->Data());
        status = true;
        break;
      case zSerial::SerialNotification::ID_CHAR_SENT:
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

class TestPort : public zSerial::SerialPort, public zThread::Function
{
public:
  TestPort() :
      _thread(this, this)
  {
    ZLOG_INFO("TestPort::TestPort: Creating test port");
  }

  virtual
  ~TestPort()
  {
    ZLOG_INFO("TestPort::~TestPort: Destroying test port");
  }

  virtual bool
  Open(const std::string &dev_)
  {
    bool status = false;
    status = this->_thread.Run();
    return (status);
  }

  virtual bool
  Close()
  {
    this->_thread.Join();
  }

  virtual void *
  ThreadFunction(void *arg_)
  {
    TestPort *self = (TestPort *) arg_;
    char c = 0;
    while (self->txchar(&c, 10000))
    {
      self->rxchar(c);
    }
    return (0);
  }

protected:

private:

  zThread::Thread _thread;

};

class TtyTestThread;

class TtyTestPort : public zSerial::TtyPort
{

  friend TtyTestThread;

public:

  TtyTestPort()
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

  }

  virtual
  ~TtyTestPort()
  {
    ZLOG_INFO("Destroying TTY test port");
    close(this->_master_fd);
  }

  std::string
  Dev()
  {
    return (this->_slave_dev);
  }

protected:

  int _master_fd;

private:

  std::string _slave_dev;

};

class TtyTestThread : public zThread::Function
{
public:
  TtyTestThread(TtyTestPort *port_) :
      _test_port(port_), _thread(this, this)
  {

    ZLOG_INFO("Created new test thread");
    this->_thread.Run();
  }

  virtual
  ~TtyTestThread()
  {
    ZLOG_INFO("Destroying test thread");
    this->_thread.Join();
  }

protected:

  virtual void *
  ThreadFunction(void *arg_)
  {

    int usecs = 1000000;
    fd_set rxFdSet;
    struct timeval to = { 0 };
    ssize_t bytes = -1;
    char buf[256] = { 0 };

    TtyTestThread *self = (TtyTestThread *) arg_;

    // Setup for poll loop
    struct pollfd fds[1];
    fds[0].fd = self->_test_port->_master_fd;
    fds[0].events = (POLLIN | POLLERR);
    int ret = poll(fds, 1, 100);
    if (ret > 0 && ((fds[0].revents & POLLIN) == POLLIN))
    {
      bytes = read(self->_test_port->_master_fd, buf, sizeof(buf));
      if (bytes > 0)
      {
        std::string str(buf);
        ZLOG_INFO("Received " + zLog::IntStr(bytes) + " bytes");
        ZLOG_DEBUG("STR: '" + str + "'");
        bytes = write(self->_test_port->_master_fd, str.c_str(), str.size());
        if (bytes > 0)
        {
          ZLOG_INFO("Sent " + zLog::IntStr(bytes) + " bytes");
          ZLOG_DEBUG("STR: '" + str + "'");
        }
      }
    }

    return (0);
  }

private:

  TtyTestPort *_test_port;
  zThread::Thread _thread;

};

#endif /* _ZCOMTEST_H_ */
