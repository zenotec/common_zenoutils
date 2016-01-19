#include "UnitTest.h"
#include "zLcdTest.h"

using namespace Test;
using namespace zUtils;

int
zLcdTest_LcdVarGetSet(void *arg_)
{
  // Create new LCD variable and verify
  zLcd::LcdVar *MyVar = new zLcd::LcdVar("var");
  TEST_ISNOT_NULL(MyVar);
  TEST_EQ("var", MyVar->GetName());
  TEST_EQ("", MyVar->GetValue());
  TEST_EQ(0, MyVar->GetRow());
  TEST_EQ(0, MyVar->GetCol());
  TEST_EQ(0, MyVar->GetLength());

  // Cleanup
  delete (MyVar);

  // Return success
  return (0);

}
