#include <unistd.h>
#include <string>

#include "UnitTest.h"

#include "zutils/zSocket.h"
#include "zutils/zQueue.h"

using namespace Test;
using namespace zUtils::zSocket;

class SocketTestListener : public SocketListener
{
public:

  virtual bool
  SocketRecv(const SocketAddr &addr_, SocketBuffer *sb_)
  {
    this->_sq.Push(std::make_pair(addr_, sb_));
    return (true);
  }

  SocketBuffer *
  WaitForPacket(int ms_)
  {
    std::pair<SocketAddr, SocketBuffer *> q;
    SocketBuffer *sb = 0;
    for (int i = 0; i < ms_; i++)
    {
      if (!this->_sq.Empty())
      {
        q = this->_sq.Front();
        this->_sq.Pop();
        sb = q.second;
        break;
      } // end if
      usleep(1000);
    } // end for
    return (sb);
  }

protected:

private:
  zUtils::zQueue<std::pair<SocketAddr, SocketBuffer *> > _sq;
};

static int
UnitTest_SocketDefaults(int arg_)
{
  SocketTestListener listener;

  // Create Socket address using string notation
  SocketAddr MySockAddr("127.0.0.2:56789");
  TEST_EQ(MySockAddr.GetIpAddrStr(), std::string("127.0.0.2"));
  TEST_EQ(MySockAddr.GetPortStr(), std::string("56789"));

  // Create socket
  Socket MySock(MySockAddr);
  MySock.Listen();
  MySock.Register(&listener);

  // Cleanup
  MySock.Unregister(&listener);

  // Return success
  return (0);

}

static int
UnitTest_SocketListener(int arg_)
{
  SocketTestListener listener;

  // Create listener socket address using string notation
  SocketAddr ListenerAddr("127.0.0.1:56789");
  TEST_EQ(ListenerAddr.GetIpAddrStr(), std::string("127.0.0.1"));
  TEST_EQ(ListenerAddr.GetPortStr(), std::string("56789"));

  // Create listener socket
  Socket ListenerSock(ListenerAddr);
  ListenerSock.Listen();
  ListenerSock.Register(&listener);

  // Create sender socket address using string notation
  SocketAddr SenderAddr("127.0.0.2:56789");
  TEST_EQ(SenderAddr.GetIpAddrStr(), std::string("127.0.0.2"));
  TEST_EQ(SenderAddr.GetPortStr(), std::string("56789"));

  // Create sender socket
  Socket SenderSock(SenderAddr);

  // Create packet for sending
  SocketBuffer sb_exp(88), *sb_obs;
  memset(sb_exp.Data(), 0xed, 88);
  sb_exp.Put(88);

  // Send packet to listener
  SenderSock.Send(ListenerAddr, &sb_exp);

  // Wait for packet to be received and validated
  sb_obs = listener.WaitForPacket(1000);
  TEST_ISNOT_NULL(sb_obs);
  TEST_TRUE(sb_exp == *sb_obs);
  delete(sb_obs);

  // Cleanup
  ListenerSock.Unregister(&listener);

  // Return success
  return (0);

}

int
Socket_utest(void)
{
  INIT();
  UTEST(UnitTest_SocketDefaults, 0);
  UTEST(UnitTest_SocketListener, 0);
  FINI();
}

