/*
 * zSocketTest.h
 *
 *  Created on: Jan 10, 2015
 *      Author: kmahoney
 */

#ifndef _ZSOCKETTEST_H_
#define _ZSOCKETTEST_H_

#include <unistd.h>

#include <zutils/zSocket.h>
#include <zutils/zQueue.h>

#include "UnitTest.h"

int
zSocketTest_SocketBufferDefaults(void* arg_);
int
zSocketTest_SocketAddrDefaults(void* arg_);
int
zSocketTest_SocketDefaults(void* arg_);
int
zSocketTest_ListenerDefaults(void* arg_);
int
zSocketTest_SocketBufferCompare(void* arg_);
int
zSocketTest_SocketAddrGet(void* arg_);
int
zSocketTest_SocketAddrSet(void* arg_);
int
zSocketTest_SocketAddrCompare(void* arg_);
int
zSocketTest_SocketListener(void* arg_);

using namespace Test;
using namespace zUtils;

class SocketTestListener : public zSocket::SocketListener
{
public:

  virtual bool
  SocketRecv(const zSocket::SocketAddr &addr_, zSocket::SocketBuffer *sb_)
  {
    this->_sq.Push(std::make_pair(addr_, sb_));
    return (true);
  }

  zSocket::SocketBuffer *
  WaitForPacket(int ms_)
  {
    std::pair<zSocket::SocketAddr, zSocket::SocketBuffer *> q;
    zSocket::SocketBuffer *sb = 0;
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
  zQueue<std::pair<zSocket::SocketAddr, zSocket::SocketBuffer *> > _sq;
};

#endif /* _ZSOCKETTEST_H_ */
