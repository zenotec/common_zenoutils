#include <list>
#include <mutex>

#include <zutils/zLog.h>
#include <zutils/zQueue.h>
#include <zutils/zThread.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>
#include <zutils/zSerial.h>

#include "UnitTest.h"
#include "zSerialTest.h"

using namespace Test;
using namespace zUtils;

TtyTestThread::TtyTestThread(TtyTestPort *port_) :
    _port(port_)
{

  ZLOG_INFO("Created new test thread");
}

TtyTestThread::~TtyTestThread()
{
  ZLOG_INFO("Destroying test thread");
}

void
TtyTestThread::Run(zThread::ThreadArg *arg_)
{

  int usecs = 1000000;
  fd_set rxFdSet;
  struct timeval to = { 0 };
  ssize_t bytes = -1;
  char buf[256] = { 0 };

  if (!this->_port)
  {
    return;
  }

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = this->_port->_master_fd;
  fds[0].events = (POLLIN | POLLERR);

  while (!this->Exit())
  {

    int ret = poll(fds, 1, 100);
    if (ret > 0 && ((fds[0].revents & POLLIN) == POLLIN))
    {
      bytes = read(this->_port->_master_fd, buf, sizeof(buf));
      if (bytes > 0)
      {
        std::string str(buf);
        ZLOG_INFO("Received " + zLog::IntStr(bytes) + " bytes");
        ZLOG_DEBUG("STR: '" + str + "'");
        bytes = write(this->_port->_master_fd, str.c_str(), str.size());
        if (bytes > 0)
        {
          ZLOG_INFO("Sent " + zLog::IntStr(bytes) + " bytes");
          ZLOG_DEBUG("STR: '" + str + "'");
        }
      }
    }
  }

  return;
}

int
zSerialTest_TtyPortSendRecvChar(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSerialTest_TtyPortSendRecvChar()");
  ZLOG_DEBUG("#############################################################");

  bool status = false;

// Create new TTY test port and validate
  TtyTestPort *MyPort = new TtyTestPort;
  TEST_ISNOT_NULL(MyPort);

// Create new TTY test thread for looping back data and validate
  TtyTestThread *MyThread = new TtyTestThread(MyPort);
  TEST_ISNOT_NULL(MyThread);

// Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);
  TEST_FALSE(MyObserver->RxSem.TryWait());
  TEST_FALSE(MyObserver->TxSem.TryWait());
  TEST_FALSE(MyObserver->ErrSem.TryWait());

// Create new handler and validate
  zEvent::EventHandler *MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_NULL(MyHandler);

// Register observer
  MyHandler->RegisterObserver(MyObserver);

// Register port with handler
  MyHandler->RegisterEvent(MyPort);

// Open port
  status = MyPort->Open();
  TEST_TRUE(status);

// Send byte
  status = MyPort->SendChar('a');
  TEST_TRUE(status);

// Wait for byte to be sent
  status = MyObserver->TxSem.TimedWait(100000);
  TEST_TRUE(status);
  TEST_EQ('a', MyObserver->TxSem.Front());
  MyObserver->TxSem.Pop();

// Wait for byte to be received
  status = MyObserver->RxSem.TimedWait(100000);
  TEST_TRUE(status);
  TEST_EQ('a', MyObserver->RxSem.Front());
  MyObserver->RxSem.Pop();

// Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

// Unregister observer
  MyHandler->UnregisterObserver(MyObserver);

// Cleanup
  delete (MyHandler);
  delete (MyObserver);
  delete (MyThread);
  MyPort->Close();
  delete (MyPort);

// Return success
  return (0);

}

int
zSerialTest_TtyPortSendRecvBuf(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSerialTest_TtyPortSendRecvBuf()");
  ZLOG_DEBUG("#############################################################");

  int cnt = 0;
  char c = 0;
  bool status = false;
  char exp_buf[100] = { 0 };
  char obs_buf[100] = { 0 };
  ssize_t bytes = 0;

// Create new TTY test port and validate
  TtyTestPort *MyPort = new TtyTestPort;
  TEST_ISNOT_NULL(MyPort);

// Create new TTY test thread for looping back data and validate
  TtyTestThread *MyThread = new TtyTestThread(MyPort);
  TEST_ISNOT_NULL(MyThread);

// Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);
  TEST_FALSE(MyObserver->RxSem.TryWait());
  TEST_FALSE(MyObserver->TxSem.TryWait());
  TEST_FALSE(MyObserver->ErrSem.TryWait());

// Create new handler and validate
  zEvent::EventHandler *MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_NULL(MyHandler);

// Register observer
  MyHandler->RegisterObserver(MyObserver);

// Register port with handler
  MyHandler->RegisterEvent(MyPort);

// Open port
  status = MyPort->Open();
  TEST_TRUE(status);

// Initialize data buffer
  for (cnt = 0; cnt < sizeof(exp_buf); cnt++)
  {
    exp_buf[cnt] = (cnt + 1);
  }

// Send data
  bytes = MyPort->SendBuf(exp_buf, sizeof(exp_buf));
  TEST_EQ(sizeof(exp_buf), bytes);

// Verify data was sent
  for (cnt = 0; MyObserver->TxSem.TimedWait(100000); cnt++)
  {
    c = MyObserver->TxSem.Front();
    MyObserver->TxSem.Pop();
    TEST_EQ_MSG(exp_buf[cnt], c, (zLog::IntStr(cnt) + ": " + zLog::CharStr(c)));
  }
  TEST_EQ(cnt, sizeof(exp_buf));

// Verify data was received
  for (cnt = 0; MyObserver->RxSem.TimedWait(100000); cnt++)
  {
    c = MyObserver->RxSem.Front();
    MyObserver->RxSem.Pop();
    TEST_EQ_MSG(exp_buf[cnt], c, (zLog::IntStr(cnt) + ": " + zLog::CharStr(c)));
  }
  TEST_EQ(cnt, sizeof(exp_buf));

// Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

// Unregister observer
  MyHandler->UnregisterObserver(MyObserver);

// Cleanup
  delete (MyHandler);
  delete (MyObserver);
  delete (MyThread);
  MyPort->Close();
  delete (MyPort);

// Return success
  return (0);

}

