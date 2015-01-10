#include "zDataTest.h"

using namespace zUtils;

int
zDataTest_GetChildSingle(void* arg_)
{

  std::string expKey;
  std::string expvalue;

  // Create new data object and validate
  std::string ParentKey = "Employee";
  zData::Data Parent(ParentKey);
  TEST_EQ(Parent.GetKey(), ParentKey);

  // Set first value and validate
  std::string ChildKey = "Name";
  expKey = ChildKey + std::string(".First");
  expvalue = "Elvis";
  Parent.SetValue(expKey, expvalue);
  TEST_EQ(Parent.GetValue(expKey), expvalue);

  // Set second value and validate
  expKey = ChildKey + std::string(".Last");
  expvalue = "Presley";
  Parent.SetValue(expKey, expvalue);
  TEST_EQ(Parent.GetValue(expKey), expvalue);

  // Get the data object for the child "employee"
  zData::Data Child(ChildKey);
  Parent.GetChild(ChildKey, Child);
  TEST_EQ(Child.GetKey(), ChildKey);

  // validate first value
  expKey = "First";
  expvalue = "Elvis";
  TEST_EQ(Child.GetValue(expKey), expvalue);

  // validate second value
  expKey = "Last";
  expvalue = "Presley";
  TEST_EQ(Child.GetValue(expKey), expvalue);

  // Return success
  return (0);

}

int
zDataTest_GetChildMultiple(void* arg_)
{

  std::string expKey;
  std::string expvalue;

  // Create new data object and validate
  std::string ParentKey = "Employee";
  zData::Data Parent(ParentKey);
  TEST_EQ(Parent.GetKey(), ParentKey);

  // Set first value and validate
  std::string Child1Key = "Name";
  expKey = Child1Key + ".First";
  expvalue = "Elvis";
  Parent.SetValue(expKey, expvalue);
  TEST_EQ(Parent.GetValue(expKey), expvalue);

  // Set second value and validate
  expKey = Child1Key + ".Last";
  expvalue = "Presley";
  Parent.SetValue(expKey, expvalue);
  TEST_EQ(Parent.GetValue(expKey), expvalue);

  // Set first value and validate
  std::string Child2Key = "Address";
  expKey = Child2Key + ".Street";
  expvalue = "123 Some St.";
  Parent.SetValue(expKey, expvalue);
  TEST_EQ(Parent.GetValue(expKey), expvalue);

  // Set second value and validate
  expKey = Child2Key + ".State";
  expvalue = "Colorado";
  Parent.SetValue(expKey, expvalue);
  TEST_EQ(Parent.GetValue(expKey), expvalue);

  // Get the data object for the child "employee"
  zData::Data Child1;
  Parent.GetChild(Child1Key, Child1);
  TEST_EQ(Child1.GetKey(), Child1Key);
  zData::Data Child2;
  Parent.GetChild(Child2Key, Child2);
  TEST_EQ(Child2.GetKey(), Child2Key);

  // validate first value
  expKey = "First";
  expvalue = "Elvis";
  TEST_EQ(Child1.GetValue(expKey), expvalue);

  // validate second value
  expKey = "Last";
  expvalue = "Presley";
  TEST_EQ(Child1.GetValue(expKey), expvalue);

  // validate first value
  expKey = "Street";
  expvalue = "123 Some St.";
  TEST_EQ(Child2.GetValue(expKey), expvalue);

  // validate second value
  expKey = "State";
  expvalue = "Colorado";
  TEST_EQ(Child2.GetValue(expKey), expvalue);

  // Return success
  return (0);

}

int
zDataTest_PutChildSingle(void* arg_)
{

  std::string expKey;
  std::string expValue;

  // Create parent data object and validate
  std::string ParentKey = "Employee";
  zData::Data Parent(ParentKey);
  TEST_EQ(Parent.GetKey(), ParentKey);

  // Create child data object and validate
  std::string ChildKey = "Name";
  zData::Data Child(ChildKey);
  TEST_EQ(Child.GetKey(), ChildKey);

  // Set first value and validate
  expKey = "First";
  expValue = "Elvis";
  Child.SetValue(expKey, expValue);
  TEST_EQ(Child.GetValue(expKey), expValue);

  // Set second value and validate
  expKey = "Last";
  expValue = "Presley";
  Child.SetValue(expKey, expValue);
  TEST_EQ(Child.GetValue(expKey), expValue);

  // Set the child and validate
  Parent.PutChild(ChildKey, Child);
  return (0);

  // Get first value and validate
  expKey = ChildKey + ".First";
  expValue = "Elvis";
  TEST_EQ(Parent.GetValue(expKey), expValue);

  // Get second value and validate
  expKey = ChildKey + ".Last";
  expValue = "Presley";
  TEST_EQ(Parent.GetValue(expKey), expValue);

  // Return success
  return (0);

}

int
zDataTest_PutChildMultiple(void* arg_)
{

  std::string expKey;
  std::string expValue;

  // Create parent data object and validate
  std::string ParentKey = "Employee";
  zData::Data Parent(ParentKey);
  TEST_EQ(Parent.GetKey(), ParentKey);

  // Create child data object and validate
  std::string Child1Key = "Name";
  zData::Data Child1(Child1Key);
  TEST_EQ(Child1.GetKey(), Child1Key);

  // Create child data object and validate
  std::string Child2Key("Address");
  zData::Data Child2(Child2Key);
  TEST_EQ(Child2.GetKey(), Child2Key);

  // Set first value and validate
  expKey = "First";
  expValue = "Elvis";
  Child1.SetValue(expKey, expValue);
  TEST_EQ(Child1.GetValue(expKey), expValue);

  // Set second value and validate
  expKey = "Last";
  expValue = "Presley";
  Child1.SetValue(expKey, expValue);
  TEST_EQ(Child1.GetValue(expKey), expValue);

  // Set first value and validate
  expKey = "Street";
  expValue = "123 Some St.";
  Child2.SetValue(expKey, expValue);
  TEST_EQ(Child2.GetValue(expKey), expValue);

  // Set second value and validate
  expKey = "State";
  expValue = "Colorado";
  Child2.SetValue(expKey, expValue);
  TEST_EQ(Child2.GetValue(expKey), expValue);

  // Set the child and validate
  Parent.PutChild(Child1Key, Child1);
  Parent.PutChild(Child2Key, Child2);

  // Get first value and validate
  expKey = Child1Key + ".First";
  expValue = "Elvis";
  TEST_EQ(Parent.GetValue(expKey), expValue);

  // Get second value and validate
  expKey = Child1Key + ".Last";
  expValue = "Presley";
  TEST_EQ(Parent.GetValue(expKey), expValue);

  // Get first value and validate
  expKey = Child2Key + ".Street";
  expValue = "123 Some St.";
  TEST_EQ(Parent.GetValue(expKey), expValue);

  // Get second value and validate
  expKey = Child2Key + ".State";
  expValue = "Colorado";
  TEST_EQ(Parent.GetValue(expKey), expValue);

  // Return success
  return (0);

}

