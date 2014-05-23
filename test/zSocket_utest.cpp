#include "UnitTest.h"
#include "zutils/zSocket.h"

using namespace Test;
using namespace zUtils::zSocket;

#include "zSocket/SocketBuffer_utest.cpp"
#include "zSocket/SocketAddr_utest.cpp"
#include "zSocket/Socket_utest.cpp"

int
zSocket_utest(void)
{
  int ret = 0;
  ret |= SocketBuffer_utest();
  ret |= SocketAddr_utest();
  ret |= Socket_utest();
}

