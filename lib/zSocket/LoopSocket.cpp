/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <zutils/zLog.h>
#include <zutils/zSocket.h>
#include <zutils/zLoopSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// Class: zSocket::LoopSocket
//**********************************************************************

LoopSocket::LoopSocket() :
    Socket(SOCKET_TYPE::TYPE_LOOP), _sock(0), _bound(false)
{
  // Create a AF_INET socket
  this->_sock = socket( AF_INET, (SOCK_DGRAM | SOCK_NONBLOCK), IPPROTO_UDP);
  if (this->_sock > 0)
  {
    ZLOG_INFO("Socket created: " + ZLOG_INT(this->_sock));
  }
  else
  {
    this->_sock = 0;
    ZLOG_ERR("Cannot create socket: " + std::string(strerror(errno)));
  }
}

LoopSocket::~LoopSocket()
{
  // Make sure the socket is unregistered from all handlers
  if (!this->_handler_list.empty())
  {
    fprintf(stderr, "BUG: Socket registered with handler, not closing FD\n");
  }
  else
  {
    // Close socket
    ZLOG_INFO("Closing socket: " + ZLOG_INT(this->_sock));
    if (this->_sock)
    {
      close(this->_sock);
      this->_sock = 0;
    } // end if
  }
}

int
LoopSocket::_get_fd()
{
  return (this->_sock);
}

bool
LoopSocket::_bind()
{
  this->_bound = true;
  return (this->_bound);
}

ssize_t
LoopSocket::_recv()
{
  return (this->_bound);
}

ssize_t
LoopSocket::_send(const Address& to_, const Buffer& sb_)
{
  ssize_t nbytes = -1;
  if (this->_bound && (to_ == this->GetAddress()) && this->txNotify(to_, sb_))
  {
    if (this->rxNotify(to_, sb_))
    {
      nbytes = sb_.Size();
    }
  }
  return (nbytes);
}

}
}
