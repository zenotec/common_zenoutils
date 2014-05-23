

static int
UnitTestSBDefaults(int arg_)
{

  // Create new packet and validate
  SocketBuffer MySB1;
  TEST_ISNOT_NULL(MySB1.Head());
  TEST_EQ(MySB1.Head(), MySB1.Data());
  TEST_IS_ZERO(MySB1.Length());
  TEST_IS_ZERO(MySB1.Size());
  TEST_EQ(MySB1.TotalSize(), 1500);

  // Create new packet of set size and validate
  SocketBuffer MySB2(500);
  TEST_ISNOT_NULL(MySB2.Head());
  TEST_EQ(MySB1.Head(), MySB1.Data());
  TEST_ISNOT_NULL(MySB2.Data());
  TEST_IS_ZERO(MySB2.Length());
  TEST_IS_ZERO(MySB2.Size());
  TEST_EQ(MySB2.TotalSize(), 500);

  // Return success
  return (0);

}

static int
UnitTestSBCompare(int arg_)
{

  // Create new packet and validate
  SocketBuffer MySB1;
  TEST_ISNOT_NULL(MySB1.Head());
  TEST_EQ(MySB1.Head(), MySB1.Data());
  TEST_ISNOT_NULL(MySB1.Data());
  TEST_IS_ZERO(MySB1.Length());
  TEST_IS_ZERO(MySB1.Size());
  TEST_EQ(MySB1.TotalSize(), 1500);

  // Create new packet of set size and validate
  SocketBuffer MySB2;
  TEST_ISNOT_NULL(MySB2.Head());
  TEST_EQ(MySB2.Head(), MySB2.Data());
  TEST_ISNOT_NULL(MySB2.Data());
  TEST_IS_ZERO(MySB2.Length());
  TEST_IS_ZERO(MySB2.Size());
  TEST_EQ(MySB2.TotalSize(), 1500);

  // Validate
  TEST_TRUE((MySB1 == MySB2));

  // Write to first packet and validate
  memset(MySB1.Data(), 0xed, 100);
  MySB1.Put(100);
  TEST_TRUE((MySB1 != MySB2));

  // Write to second packet and validate
  memset(MySB2.Data(), 0xde, 100);
  MySB2.Put(100);
  TEST_TRUE((MySB1 != MySB2));

  // Write to second packet and validate
  memset(MySB2.Data(), 0xed, 100);
  TEST_TRUE((MySB1 == MySB2));

  // Return success
  return (0);

}

int
SocketBuffer_utest(void)
{
  INIT();
  UTEST(UnitTestSBDefaults, 0);
  UTEST(UnitTestSBCompare, 0);
  FINI();
}
