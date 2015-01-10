
#include "zLogTest.h"

int
main(int argc, const char **argv)
{

  // Test all classes
  UTEST_INIT();
  UTEST_TEST( zLogTest_Defaults, 0);
  UTEST_TEST( zLogTest_FileConnector, 0);
  UTEST_FINI();

  // Exit
  exit( 0 );


}
