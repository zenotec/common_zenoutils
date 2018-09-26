/*
 * Copyright (c) 2014-2018 ZenoTec LLC (http://www.zenotec.net)
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

#ifndef __ZUNIXSOCKET_H__
#define __ZUNIXSOCKET_H__

#include <sys/un.h>

#include <string>

#include <zutils/zThread.h>
#include <zutils/zSocket.h>
#include <zutils/zUnixAddress.h>

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// Class: zSocket::UnixSocket
//**********************************************************************

class UnixSocket :
    public Socket
{

public:

  UnixSocket();

  virtual
  ~UnixSocket();

  virtual int
  GetId() const;

  virtual const Address&
  GetAddress() const;

  virtual bool
  Bind(const Address& addr_);

  virtual SHARED_PTR(zSocket::Notification)
  Recv();

  virtual SHARED_PTR(zSocket::Notification)
  Send(const Address& to_, const Buffer& sb_);

protected:

private:

  int _fd;
  UnixAddress _addr;

};

}
}

#endif /* __ZUNIXSOCKET_H__ */
