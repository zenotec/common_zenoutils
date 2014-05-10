#include "UnitTest.h"
#include "zData.h"

using namespace Test;
using namespace zUtils;

static int
UnitTestData_Defaults(int arg_)
{
  std::string key;
  zData *MyData;

  // Create new data object and validate
  key = "";
  MyData = new zData;
  TEST_EQ( MyData->GetKey(), key);
  TEST_EQ( MyData->GetJson(), std::string( "{\n    \"Key\": \"\"\n}\n" ));
  TEST_EQ( MyData->GetXml(), std::string( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key/>" ));
  delete (MyData);

  // Create new data object and validate
  key = "TestKey";
  MyData = new zData(key);
  TEST_EQ( MyData->GetKey(), key);
  TEST_EQ( MyData->GetJson(),
      std::string( "{\n    \"Key\": \"TestKey\",\n    \"TestKey\": \"\"\n}\n" ));
  TEST_EQ( MyData->GetXml(),
      std::string( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>TestKey</Key><TestKey/>" ));
  delete (MyData);

  // Return success
  return (0);

}

static int
UnitTestData_SetValueSingle(int arg_)
{
  std::string key("TestKey");
  std::string val("TestValue");

  // Create new data object and validate
  zData *MyData = new zData(key);
  TEST_EQ( MyData->GetKey(), key);
  TEST_EQ( MyData->GetJson(),
      std::string( "{\n    \"Key\": \"TestKey\",\n    \"TestKey\": \"\"\n}\n" ));
  TEST_EQ( MyData->GetXml(),
      std::string( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>TestKey</Key><TestKey/>" ));

  // Set value and validate
  MyData->SetVal(key, val);
  TEST_EQ( MyData->GetVal( key ), val);

  // Cleanup
  delete (MyData);

  // Return success
  return (0);
}

static int
UnitTestData_SetValueMultiple(int arg_)
{

  std::string key("Employee");
  std::string val("TestValue");

  // Create new data object and validate
  zData *MyData = new zData(key);
  TEST_EQ( MyData->GetKey(), key);
  TEST_EQ( MyData->GetJson(),
      std::string( "{\n    \"Key\": \"Employee\",\n    \"Employee\": \"\"\n}\n" ));
  TEST_EQ( MyData->GetXml(),
      std::string( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>Employee</Key><Employee/>" ));

  // Set first value and validate
  key = "Employee.Name.First";
  val = "Kevin";
  MyData->SetVal(key, val);
  TEST_EQ( MyData->GetVal( key ), val);

  // Set second value and validate
  key = "Employee.Name.Last";
  val = "Mahoney";
  MyData->SetVal(key, val);
  TEST_EQ( MyData->GetVal( key ), val);

  // Cleanup
  delete (MyData);

  // Return success
  return (0);
}

static int
UnitTestData_GetChildSingle(int arg_)
{

  std::string ParentKey("Employee");
  std::string ChildKey("Name");
  std::string key;
  std::string val;

  // Create new data object and validate
  zData Parent(ParentKey);
  TEST_EQ( Parent.GetKey(), ParentKey);
  TEST_EQ( Parent.GetJson(),
      std::string( "{\n    \"Key\": \"Employee\",\n    \"Employee\": \"\"\n}\n" ));
  TEST_EQ( Parent.GetXml(),
      std::string( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>Employee</Key><Employee/>" ));

  // Set first value and validate
  key = ChildKey + ".First";
  val = "Kevin";
  Parent.SetVal(key, val);
  TEST_EQ( Parent.GetVal( key ), val);

  // Set second value and validate
  key = ChildKey + ".Last";
  val = "Mahoney";
  Parent.SetVal(key, val);
  TEST_EQ( Parent.GetVal( key ), val);

  // Get the data object for the child "employee"
  zData Child(ChildKey);
  Parent.GetChild(ChildKey, Child);
  TEST_EQ( Child.GetKey(), ChildKey);
  return (0);

  // Validate first value
  key = "First";
  val = "Kevin";
  TEST_EQ( Child.GetVal( key ), val);

  // Validate second value
  key = "Last";
  val = "Mahoney";
  TEST_EQ( Child.GetVal( key ), val);

  // Return success
  return (0);

}

static int
UnitTestData_GetChildMultiple(int arg_)
{

  std::string ParentKey("Employee");
  std::string Child1Key("Name");
  std::string Child2Key("Address");
  std::string key;
  std::string val;

  // Create new data object and validate
  zData Parent(ParentKey);
  TEST_EQ( Parent.GetKey(), ParentKey);
  TEST_EQ( Parent.GetJson(),
      std::string( "{\n    \"Key\": \"Employee\",\n    \"Employee\": \"\"\n}\n" ));
  TEST_EQ( Parent.GetXml(),
      std::string( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>Employee</Key><Employee/>" ));

  // Create new data object and validate
  zData Child1, Child2;

  // Set first value and validate
  key = Child1Key + ".First";
  val = "Kevin";
  Parent.SetVal(key, val);
  TEST_EQ( Parent.GetVal( key ), val);

  // Set second value and validate
  key = Child1Key + ".Last";
  val = "Mahoney";
  Parent.SetVal(key, val);
  TEST_EQ( Parent.GetVal( key ), val);

  // Set first value and validate
  key = Child2Key + ".Street";
  val = "123 Some St.";
  Parent.SetVal(key, val);
  TEST_EQ( Parent.GetVal( key ), val);

  // Set second value and validate
  key = Child2Key + ".State";
  val = "Colorado";
  Parent.SetVal(key, val);
  TEST_EQ( Parent.GetVal( key ), val);

  // Get the data object for the child "employee"
  Parent.GetChild(Child1Key, Child1);
  TEST_EQ( Child1.GetKey(), Child1Key);
  Parent.GetChild(Child2Key, Child2);
  TEST_EQ( Child2.GetKey(), Child2Key);

  // Validate first value
  key = "First";
  val = "Kevin";
  TEST_EQ( Child1.GetVal( key ), val);

  // Validate second value
  key = "Last";
  val = "Mahoney";
  TEST_EQ( Child1.GetVal( key ), val);

  // Validate first value
  key = "Street";
  val = "123 Some St.";
  TEST_EQ( Child2.GetVal( key ), val);

  // Validate second value
  key = "State";
  val = "Colorado";
  TEST_EQ( Child2.GetVal( key ), val);

  // Return success
  return (0);

}

static int
UnitTestData_PutChildSingle(int arg_)
{

  std::string ParentKey("Employee");
  std::string ChildKey("Name");
  std::string key;
  std::string val;

  // Create parent data object and validate
  zData Parent(ParentKey);
  TEST_EQ( Parent.GetKey(), ParentKey);
  TEST_EQ( Parent.GetJson(),
      std::string( "{\n    \"Key\": \"Employee\",\n    \"Employee\": \"\"\n}\n" ));
  TEST_EQ( Parent.GetXml(),
      std::string( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>Employee</Key><Employee/>" ));

  // Create child data object and validate
  zData Child(ChildKey);
  TEST_EQ( Child.GetKey(), ChildKey);
  TEST_EQ( Child.GetJson(), std::string( "{\n    \"Key\": \"Name\",\n    \"Name\": \"\"\n}\n" ));
  TEST_EQ( Child.GetXml(),
      std::string( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>Name</Key><Name/>" ));

  // Set first value and validate
  key = "First";
  val = "Kevin";
  Child.SetVal(key, val);
  TEST_EQ( Child.GetVal( key ), val);

  // Set second value and validate
  key = "Last";
  val = "Mahoney";
  Child.SetVal(key, val);
  TEST_EQ( Child.GetVal( key ), val);

  // Set the child and validate
  Parent.PutChild(ChildKey, Child);
  return (0);

  // Get first value and validate
  key = ChildKey + ".First";
  val = "Kevin";
  TEST_EQ( Parent.GetVal( key ), val);

  // Get second value and validate
  key = ChildKey + ".Last";
  val = "Mahoney";
  TEST_EQ( Parent.GetVal( key ), val);

  // Return success
  return (0);

}

static int
UnitTestData_PutChildMultiple(int arg_)
{

  std::string ParentKey("Employee");
  std::string Child1Key("Name");
  std::string Child2Key("Address");
  std::string key;
  std::string val;

  // Create parent data object and validate
  zData Parent(ParentKey);
  TEST_EQ( Parent.GetKey(), ParentKey);
  TEST_EQ( Parent.GetJson(),
      std::string( "{\n    \"Key\": \"Employee\",\n    \"Employee\": \"\"\n}\n" ));
  TEST_EQ( Parent.GetXml(),
      std::string( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>Employee</Key><Employee/>" ));

  // Create child data object and validate
  zData Child1(Child1Key);
  TEST_EQ( Child1.GetKey(), Child1Key);
  TEST_EQ( Child1.GetJson(), std::string( "{\n    \"Key\": \"Name\",\n    \"Name\": \"\"\n}\n" ));
  TEST_EQ( Child1.GetXml(),
      std::string( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>Name</Key><Name/>" ));

  // Create child data object and validate
  zData Child2(Child2Key);
  TEST_EQ( Child2.GetKey(), Child2Key);
  TEST_EQ( Child2.GetJson(),
      std::string( "{\n    \"Key\": \"Address\",\n    \"Address\": \"\"\n}\n" ));
  TEST_EQ( Child2.GetXml(),
      std::string( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>Address</Key><Address/>" ));

  // Set first value and validate
  key = "First";
  val = "Kevin";
  Child1.SetVal(key, val);
  TEST_EQ( Child1.GetVal( key ), val);

  // Set second value and validate
  key = "Last";
  val = "Mahoney";
  Child1.SetVal(key, val);
  TEST_EQ( Child1.GetVal( key ), val);

  // Set first value and validate
  key = "Street";
  val = "123 Some St.";
  Child2.SetVal(key, val);
  TEST_EQ( Child2.GetVal( key ), val);

  // Set second value and validate
  key = "State";
  val = "Colorado";
  Child2.SetVal(key, val);
  TEST_EQ( Child2.GetVal( key ), val);

  // Set the child and validate
  Parent.PutChild(Child1Key, Child1);
  Parent.PutChild(Child2Key, Child2);

  // Get first value and validate
  key = Child1Key + ".First";
  val = "Kevin";
  TEST_EQ( Parent.GetVal( key ), val);

  // Get second value and validate
  key = Child1Key + ".Last";
  val = "Mahoney";
  TEST_EQ( Parent.GetVal( key ), val);

  // Get first value and validate
  key = Child2Key + ".Street";
  val = "123 Some St.";
  TEST_EQ( Parent.GetVal( key ), val);

  // Get second value and validate
  key = Child2Key + ".State";
  val = "Colorado";
  TEST_EQ( Parent.GetVal( key ), val);

  // Return success
  return (0);

}

static int
UnitTestData_JsonSimple(int arg_)
{

  std::string key("TestKey");
  std::string name("TestName");
  std::string val("TestValue");

  // Create new data object and validate
  zData MyData1(key);
  TEST_EQ( MyData1.GetKey(), key);
  TEST_EQ( MyData1.GetJson(),
      std::string( "{\n    \"Key\": \"TestKey\",\n    \"TestKey\": \"\"\n}\n" ));
  TEST_EQ( MyData1.GetXml(),
      std::string( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>TestKey</Key><TestKey/>" ));

  // Set value and validate
  MyData1.SetVal(name, val);
  TEST_EQ( MyData1.GetVal( name ), val);

  // Create second empty data object and validate
  zData MyData2;
  TEST_EQ( MyData2.GetKey(), "");
  TEST_EQ( MyData2.GetJson(), std::string( "{\n    \"Key\": \"\"\n}\n" ));
  TEST_EQ( MyData2.GetXml(), std::string( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key/>" ));

  // Set the second data object from the first using only JSON
  MyData2.SetJson(MyData1.GetJson());
  TEST_EQ( MyData2.GetKey(), key);
  TEST_EQ( MyData2.GetVal( name ), val);
  TEST_EQ( MyData2.GetJson(), MyData1.GetJson());
  TEST_EQ( MyData2.GetXml(), MyData1.GetXml());

  // Return success
  return (0);

}

static int
UnitTestData_XmlSimple(int arg_)
{

  std::string key("TestKey");
  std::string name("TestName");
  std::string val("TestValue");

  // Create new data object and validate
  zData MyData1(key);
  TEST_EQ( MyData1.GetKey(), key);
  TEST_EQ( MyData1.GetJson(),
      std::string( "{\n    \"Key\": \"TestKey\",\n    \"TestKey\": \"\"\n}\n" ));
  TEST_EQ( MyData1.GetXml(),
      std::string( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>TestKey</Key><TestKey/>" ));

  // Set value and validate
  MyData1.SetVal(name, val);
  TEST_EQ( MyData1.GetVal( name ), val);

  // Create second empty data object and validate
  zData MyData2;
  TEST_EQ( MyData2.GetKey(), "");
  TEST_EQ( MyData2.GetJson(), std::string( "{\n    \"Key\": \"\"\n}\n" ));
  TEST_EQ( MyData2.GetXml(), std::string( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key/>" ));

  // Set the second data object from the first using only XML
  MyData2.SetXml(MyData1.GetXml());
  TEST_EQ( MyData2.GetKey(), key);
  TEST_EQ( MyData2.GetVal( name ), val);
  TEST_EQ( MyData2.GetJson(), MyData1.GetJson());
  TEST_EQ( MyData2.GetXml(), MyData1.GetXml());

  // Return success
  return (0);

}

int
zData_utest(void)
{
  INIT();
  UTEST( UnitTestData_Defaults, 0);
  UTEST( UnitTestData_SetValueSingle, 0);
  UTEST( UnitTestData_SetValueMultiple, 0);
  UTEST( UnitTestData_GetChildSingle, 0);
  UTEST( UnitTestData_GetChildMultiple, 0);
  UTEST( UnitTestData_PutChildSingle, 0);
  UTEST( UnitTestData_PutChildMultiple, 0);
  UTEST( UnitTestData_JsonSimple, 0);
  UTEST( UnitTestData_XmlSimple, 0);
  FINI();
}

