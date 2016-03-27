
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

int
zSerialTest_PortDefaults(void *arg_)
{

  ZLOG_DEBUG( "#############################################################" );
  ZLOG_DEBUG( "# zComTest_PortDefaults()" );
  ZLOG_DEBUG( "#############################################################" );

  bool status = false;
  TestPort *MyPort = new TestPort;
  TEST_ISNOT_ZERO(MyPort);

  // Cleanup
  delete (MyPort);

  // Return success
  return (0);

}

int
zSerialTest_TtyPortDefaults(void *arg_)
{

  ZLOG_DEBUG( "#############################################################" );
  ZLOG_DEBUG( "# zComTest_TtyPortDefaults()" );
  ZLOG_DEBUG( "#############################################################" );

  bool status = false;
  TtyTestPort *MyPort = new TtyTestPort;
  TEST_ISNOT_ZERO(MyPort);

  // Cleanup
  delete (MyPort);

  // Return success
  return (0);

}
