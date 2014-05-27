#include "UnitTest.h"
#include "zutils/zSocket.h"

using namespace Test;
using namespace zUtils::zSocket;

int
SocketBuffer_utest(void);
int
SocketAddr_utest(void);
int
Socket_utest(void);

int
zSocket_utest(void)
{
  int ret = 0;
  ret |= SocketBuffer_utest();
  ret |= SocketAddr_utest();
  ret |= Socket_utest();
}

