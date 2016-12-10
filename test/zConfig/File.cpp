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

#include "UnitTest.h"
#include "zConfTest.h"

using namespace Test;
using namespace zUtils;

int
zConfigTest_FileLoadStore(void* arg_)
{

  // Create new configuration data item and verify
  zData::Data *expData = new zData::Data;
  TEST_ISNOT_NULL(expData);
  zData::Data *obsData = new zData::Data;
  TEST_ISNOT_NULL(obsData);

  // Create new configuration data connector and verify
  TestConnector *myConnector = new TestConnector;
  TEST_ISNOT_NULL(myConnector);

  // Attempt to load configuration and verify
  TEST_FALSE(myConnector->Load(*expData));

  // Attempt to store configuration and verify
  TEST_TRUE(myConnector->Store(*expData));

  // Attempt to load configuration and verify
  TEST_TRUE(myConnector->Load(*obsData));

  // Update configuration data
  std::string key1 = "Key1";
  std::string val1 = "Value1";
  TEST_TRUE(expData->Put(val1, key1));
  std::string key2 = "Key2";
  std::string val2 = "Value2";
  TEST_TRUE(expData->Put(val2, key2));

  // Verify data is not equal
  TEST_TRUE(*obsData != *expData);

  // Attempt to store configuration and verify
  TEST_TRUE(myConnector->Store(*expData));

  // Attempt to load configuration and verify
  TEST_TRUE(myConnector->Load(*obsData));

  // Verify data is equal
  TEST_TRUE(*obsData == *expData);

  // Cleanup
  delete (myConnector);
  delete (obsData);
  delete (expData);

  // Return success
  return (0);
}
