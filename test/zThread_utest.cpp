#include "UnitTest.h"
#include "zutils/zThread.h"

using namespace std;
using namespace Test;
using namespace zUtils;

static int
UT_zThread(int arg_)
{
  // Return success
  return (0);
}

int
zThread_utest(void)
{
  INIT();
  UTEST( UT_zThread, 0);
  FINI();
}

