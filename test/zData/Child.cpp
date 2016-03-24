#include <zutils/zLog.h>

#include "zDataTest.h"

using namespace zUtils;

int
zDataTest_GetChildSingle(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_GetChildSingle()");
  ZLOG_DEBUG("#############################################################");

  std::string expKey;
  std::string expVal;
  std::string obsVal;

  // Create new data object and validate
  std::string ParentKey = "Employee";
  zData::Data Parent(ParentKey);
  TEST_EQ(Parent.Key(), ParentKey);

  // Set first value and validate
  std::string ChildKey = "Name";
  expKey = ChildKey + std::string(".First");
  expVal = "Elvis";
  TEST_TRUE(Parent.Put(expVal, expKey));
  TEST_TRUE(Parent.Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Set second value and validate
  expKey = ChildKey + std::string(".Last");
  expVal = "Presley";
  TEST_TRUE(Parent.Put(expVal, expKey));
  TEST_TRUE(Parent.Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Get the data object for the child "employee"
  zData::Data Child(ChildKey);
  TEST_TRUE(Parent.Get(Child, ChildKey));
  TEST_EQ(Child.Key(), ChildKey);

  // validate first value
  expKey = "First";
  expVal = "Elvis";
  TEST_TRUE(Child.Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // validate second value
  expKey = "Last";
  expVal = "Presley";
  TEST_TRUE(Child.Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Return success
  return (0);

}

int
zDataTest_GetChildMultiple(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_GetChildMultiple()");
  ZLOG_DEBUG("#############################################################");

  std::string path;
  std::string expVal;
  std::string obsVal;

  // Create new data object and validate
  std::string ParentKey = "Employee";
  zData::Data Parent(ParentKey);
  TEST_EQ(Parent.Key(), ParentKey);

  // Set first value and validate
  std::string Child1Key = "Name";
  path = Child1Key + ".First";
  expVal = "Elvis";
  TEST_TRUE(Parent.Put(expVal, path));
  TEST_TRUE(Parent.Get(obsVal, path));
  TEST_EQ(obsVal, expVal);

  // Set second value and validate
  path = Child1Key + ".Last";
  expVal = "Presley";
  TEST_TRUE(Parent.Put(expVal, path));
  TEST_TRUE(Parent.Get(obsVal, path));
  TEST_EQ(obsVal, expVal);

  // Set first value and validate
  std::string Child2Key = "Address";
  path = Child2Key + ".Street";
  expVal = "123 Some St.";
  TEST_TRUE(Parent.Put(expVal, path));
  TEST_TRUE(Parent.Get(obsVal, path));
  TEST_EQ(obsVal, expVal);

  // Set second value and validate
  path = Child2Key + ".State";
  expVal = "Colorado";
  TEST_TRUE(Parent.Put(expVal, path));
  TEST_TRUE(Parent.Get(obsVal, path));
  TEST_EQ(obsVal, expVal);

  // Get the data object for the child "employee"
  zData::Data Child1;
  TEST_TRUE(Parent.Get(Child1, Child1Key));
  TEST_EQ(Child1.Key(), Child1Key);
  zData::Data Child2;
  TEST_TRUE(Parent.Get(Child2, Child2Key));
  TEST_EQ(Child2.Key(), Child2Key);

  // validate first value
  path = "First";
  expVal = "Elvis";
  TEST_TRUE(Child1.Get(obsVal, path));
  TEST_EQ(obsVal, expVal);

  // validate second value
  path = "Last";
  expVal = "Presley";
  TEST_TRUE(Child1.Get(obsVal, path));
  TEST_EQ(obsVal, expVal);

  // validate first value
  path = "Street";
  expVal = "123 Some St.";
  TEST_TRUE(Child2.Get(obsVal, path));
  TEST_EQ(obsVal, expVal);

  // validate second value
  path = "State";
  expVal = "Colorado";
  TEST_TRUE(Child2.Get(obsVal, path));
  TEST_EQ(obsVal, expVal);

  // Return success
  return (0);

}

int
zDataTest_PutChildSingle(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_PutChildSingle()");
  ZLOG_DEBUG("#############################################################");

  std::string expKey;
  std::string obsKey;
  std::string expVal;
  std::string obsVal;

  // Create parent data object and validate
  std::string ParentKey = "Employee";
  zData::Data Parent(ParentKey);
  TEST_EQ(Parent.Key(), ParentKey);

  // Create child data object and validate
  std::string ChildKey = "Name";
  zData::Data Child(ChildKey);
  TEST_EQ(Child.Key(), ChildKey);

  // Set first value and validate
  expKey = "First";
  expVal = "Elvis";
  TEST_TRUE(Child.Put(expVal, expKey));
  TEST_TRUE(Child.Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Set second value and validate
  expKey = "Last";
  expVal = "Presley";
  TEST_TRUE(Child.Put(expVal, expKey));
  TEST_TRUE(Child.Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Set the child and validate
  TEST_TRUE(Parent.Put(Child, ChildKey));

  // Get first value and validate
  expKey = ChildKey + ".First";
  expVal = "Elvis";
  TEST_TRUE(Parent.Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Get second value and validate
  expKey = ChildKey + ".Last";
  expVal = "Presley";
  TEST_TRUE(Parent.Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Return success
  return (0);

}

int
zDataTest_PutChildMultiple(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_PutChildMultiple()");
  ZLOG_DEBUG("#############################################################");

  std::string expKey;
  std::string expVal;
  std::string obsVal;

  // Create parent data object and validate
  std::string ParentKey = "Employee";
  zData::Data Parent(ParentKey);
  TEST_EQ(Parent.Key(), ParentKey);

  // Create child data object and validate
  std::string Child1Key = "Name";
  zData::Data Child1(Child1Key);
  TEST_EQ(Child1.Key(), Child1Key);

  // Create child data object and validate
  std::string Child2Key("Address");
  zData::Data Child2(Child2Key);
  TEST_EQ(Child2.Key(), Child2Key);

  // Set first value and validate
  expKey = "First";
  expVal = "Elvis";
  TEST_TRUE(Child1.Put(expVal, expKey));
  TEST_TRUE(Child1.Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Set second value and validate
  expKey = "Last";
  expVal = "Presley";
  TEST_TRUE(Child1.Put(expVal, expKey));
  TEST_TRUE(Child1.Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Set first value and validate
  expKey = "Street";
  expVal = "123 Some St.";
  TEST_TRUE(Child2.Put(expVal, expKey));
  TEST_TRUE(Child2.Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Set second value and validate
  expKey = "State";
  expVal = "Colorado";
  TEST_TRUE(Child2.Put(expVal, expKey));
  TEST_TRUE(Child2.Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Set the child and validate
  TEST_TRUE(Parent.Put(Child1, Child1Key));
  TEST_TRUE(Parent.Put(Child2, Child2Key));

  // Get first value and validate
  expKey = Child1Key + ".First";
  expVal = "Elvis";
  TEST_TRUE(Parent.Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Get second value and validate
  expKey = Child1Key + ".Last";
  expVal = "Presley";
  TEST_TRUE(Parent.Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Get first value and validate
  expKey = Child2Key + ".Street";
  expVal = "123 Some St.";
  TEST_TRUE(Parent.Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Get second value and validate
  expKey = Child2Key + ".State";
  expVal = "Colorado";
  TEST_TRUE(Parent.Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Return success
  return (0);

}

