#include "zutils/zData.h"
#include "UnitTest.h"
#include "zDataTest.h"

using namespace zUtils;

int
zDataTest_SetValueSingle(void* arg_)
{
  std::string expKey = "TestKey";
  std::string expVal = "TestValue";
  std::string expJson = "{\n    \"Key\": \"TestKey\",\n    \"TestKey\": \"\"\n}\n";
  std::string expXml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>TestKey</Key><TestKey/>";

  // Create new data object and validate
  zData::Data *MyData = new zData::Data(expKey);
  TEST_EQ(MyData->GetKey(), expKey);
  TEST_EQ(MyData->GetJson(), expJson);
  TEST_EQ(MyData->GetXml(), expXml);

  // Set value and validate
  MyData->SetValue(expKey, expVal);
  TEST_EQ(MyData->GetValue(expKey), expVal);

  // Cleanup
  delete (MyData);

  // Return success
  return (0);
}

int
zDataTest_SetValueMultiple(void* arg_)
{

  std::string expKey = "TestKey";
  std::string expVal = "TestValue";
  std::string expJson = "{\n    \"Key\": \"TestKey\",\n    \"TestKey\": \"\"\n}\n";
  std::string expXml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>TestKey</Key><TestKey/>";

  // Create new data object and validate
  zData::Data *MyData = new zData::Data(expKey);
  TEST_EQ(MyData->GetKey(), expKey);
  TEST_EQ(MyData->GetJson(), expJson);
  TEST_EQ(MyData->GetXml(), expXml);

  // Set first value and validate
  expKey = "Employee.Name.First";
  expVal = "Elvis";
  MyData->SetValue(expKey, expVal);
  TEST_EQ(MyData->GetValue(expKey), expVal);

  // Set second value and validate
  expKey = "Employee.Name.Last";
  expVal = "Presley";
  MyData->SetValue(expKey, expVal);
  TEST_EQ(MyData->GetValue(expKey), expVal);

  // Cleanup
  delete (MyData);

  // Return success
  return (0);
}
