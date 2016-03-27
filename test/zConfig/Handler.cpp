/*
 * Data.cpp
 *
 *  Created on: Aug 27, 2015
 *      Author: kmahoney
 */

#include "UnitTest.h"
#include "zConfTest.h"

using namespace Test;
using namespace zUtils;

int
zConfigTest_HandlerLoadStore(void* arg_)
{

  std::string obs;

  // Create new configuration data item and verify
  zConfig::Configuration *myConfig = new zConfig::Configuration;
  TEST_ISNOT_NULL(myConfig);

  // Create new configuration data connector and verify
  TestConnector *myConnector = new TestConnector;
  TEST_ISNOT_NULL(myConnector);

  // Create new configuration handler and verify
  zConfig::ConfigurationHandler *myHandler = new zConfig::ConfigurationHandler;
  TEST_ISNOT_NULL(myHandler);

  // Attempt to load configuration and verify
  TEST_FALSE(myHandler->Load());

  // Attempt to store configuration and verify
  TEST_FALSE(myHandler->Store());

  // Connect data to file connector
  TEST_TRUE(myHandler->Connect(myConnector));

  // Attempt to load configuration and verify
  TEST_FALSE(myHandler->Load());

  // Update configuration data and verify
  std::string key = "Key";
  std::string val1 = "Value1";
  std::string val2 = "Value2";
  TEST_TRUE(myConfig->Put(val1, key));
  TEST_TRUE(myConfig->Commit());
  TEST_TRUE(myConfig->Get(obs, key));
  TEST_EQ(obs, val1);

  // Store configuration and verify
  TEST_TRUE(myHandler->Put(*myConfig));
  TEST_TRUE(myHandler->Commit());
  TEST_TRUE(myHandler->Store());

  // Modify data and verify
  TEST_TRUE(myConfig->Put(val2, key));
  TEST_TRUE(myConfig->Commit());
  TEST_TRUE(myConfig->Get(obs, key));
  TEST_EQ(obs, val2);

  // Load configuration over modified configuration and verify
  TEST_TRUE(myHandler->Load());
  TEST_TRUE(myHandler->Get(*myConfig));
  TEST_TRUE(myConfig->Get(obs, key));
  TEST_EQ(obs, val1);

  // Cleanup
  delete (myHandler);
  delete (myConnector);
  delete (myConfig);

  // Return success
  return (0);
}
