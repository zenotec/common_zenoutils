#include "UnitTest.h"

using namespace Test;
using namespace zUtils;

int
zConfTest_DataDefaults(void* arg_)
{
  zConf::Data *myData = new zConf::Data;

  // Cleanup
  delete(myData);

  // Return success
  return (0);
}

int
zConfTest_ConnectorDefaults(void* arg_)
{
  TestConnector *myConnector = new TestConnector;

  // Cleanup
  delete(myConnector);

  // Return success
  return (0);
}
