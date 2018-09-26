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

#ifndef __ZINETSOCKET_H__
#define __ZINETSOCKET_H__

#include <netinet/in.h>

#include <string>

#include <zutils/zThread.h>
#include <zutils/zSocket.h>
#include <zutils/zIpAddress.h>

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// Class: zSocket::InetSocket
//**********************************************************************

class UdpSocket :
    public Socket
{

public:

  UdpSocket();

  virtual
  ~UdpSocket();

  virtual int
  GetId() const;

  virtual bool
  Getopt(Socket::OPTIONS opt_);

  virtual bool
  Setopt(Socket::OPTIONS opt_);

  virtual bool
  Bind(const Address& addr_);

  virtual SHARED_PTR(zSocket::Notification)
  Recv();

  virtual SHARED_PTR(zSocket::Notification)
  Send(const Address& to_, const Buffer& sb_);

protected:

private:

  int _fd;

};

}
}

#endif /* __ZINETSOCKET_H__ */
