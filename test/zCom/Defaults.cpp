#include "UnitTest.h"
#include "zComTest.h"

using namespace Test;
using namespace zUtils;

int
zComTest_PortDefaults(void *arg_)
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
zComTest_TtyPortDefaults(void *arg_)
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
