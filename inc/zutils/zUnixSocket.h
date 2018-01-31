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

namespace zUtils
{
namespace zSocket
{

class UnixSocket;

//**********************************************************************
// Class: zSocket::UnixAddress
//**********************************************************************

class UnixAddress :
    public Address
{

  friend UnixSocket;

public:

  UnixAddress(const std::string &addr_ = "");

  UnixAddress(const Address& addr_);

  UnixAddress(const struct sockaddr_un& sa_);

  virtual
  ~UnixAddress();

  virtual std::string
  GetAddress() const;

  virtual bool
  SetAddress(const std::string &addr_);

  virtual void
  Display() const;

protected:

  struct sockaddr_un sa;

private:

};

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

protected:

  virtual bool
  _bind();

  virtual ssize_t
  _recv();

  virtual ssize_t
  _send(const Address& to_, const Buffer& sb_);

private:

  UnixAddress _sa;

};

}
}

#endif /* __ZUNIXSOCKET_H__ */
