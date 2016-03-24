/*
 * Array.cpp
 *
 *  Created on: Aug 28, 2015
 *      Author: kmahoney
 */

#include <zutils/zLog.h>

#include "zDataTest.h"

using namespace zUtils;
using namespace Test;

int
zDataTest_Array(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_Array()");
  ZLOG_DEBUG("#############################################################");

  const int num = 4;
  std::string key, obs;

  // Create new data item and validate
  zData::Data *myParent = new zData::Data(std::string("parent"));
  TEST_ISNOT_NULL(myParent);

  // Initialize key value
  key = std::string("key");

  // Create new value array and validate
  for (int i = 0; i < num; i++)
  {
    std::stringstream val;
    val << "value" << i;
    TEST_TRUE(myParent->Add(val.str()));
  }

  // Parse value array and validate
  for (int i = 0; i < num; i++)
  {
    std::stringstream val;
    val << "value" << i;
    std::unique_ptr < zData::Data > myChild = myParent->operator [](i);
    TEST_TRUE(myChild != NULL);
    TEST_TRUE(myChild->Get(obs));
    TEST_EQ(obs, val.str());
  }

  myParent->Clear();

  // Create new data array and validate
  for (int i = 0; i < num; i++)
  {
    std::stringstream val;
    zData::Data *myChild = new zData::Data("child");
    TEST_ISNOT_NULL(myChild);
    val << "value" << i;
    TEST_TRUE(myChild->Put(val.str(), key));
    TEST_TRUE(myParent->Add(*myChild, myChild->Key()));
    delete (myChild);
  }

  // Parse data array and validate
  for (int i = 0; i < num; i++)
  {
    std::stringstream val;
    std::string obs;
    val << "value" << i;
    std::unique_ptr < zData::Data > myChild = myParent->operator []("child");
    TEST_TRUE(myChild != NULL);
    myChild = myChild->operator [](i);
    TEST_TRUE(myChild != NULL);
    TEST_TRUE(myChild->Get(obs, key));
    TEST_EQ(obs, val.str());
  }

  // Cleanup
  if (myParent)
    delete (myParent);

  // Return success
  return (0);

}
