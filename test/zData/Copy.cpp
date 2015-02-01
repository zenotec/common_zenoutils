#include "zDataTest.h"

using namespace zUtils;
using namespace Test;

int
zDataTest_DataCopy(void* arg)
{

  // Create new data object and validate
  std::string key1 = "key1";
  zData::Data *myData1 = new zData::Data(key1);
  TEST_EQ(myData1->GetKey(), key1);

  // Create new data object and validate
  std::string key2 = "key2";
  zData::Data *myData2 = new zData::Data(key2);
  TEST_EQ(myData2->GetKey(), key2);

  // Verify data is NOT equal
  TEST_TRUE(*myData1 != *myData2);

  // Copy
  *myData2 = *myData1;

  std::cout << "zDataTest_DataCopy(): Data1: " << myData1->GetJson() << std::endl;
  std::cout << "zDataTest_DataCopy(): Data2: " << myData2->GetJson() << std::endl;

  // Verify data is equal
  TEST_TRUE(*myData1 == *myData2);

  // Clean up
  delete (myData1);
  delete (myData2);

  // Return success
  return (0);

}

