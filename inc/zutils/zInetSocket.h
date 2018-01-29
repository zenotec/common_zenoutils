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

#include <string>

#include <zutils/zThread.h>
#include <zutils/zSocket.h>

struct sockaddr_in;

namespace zUtils
{
namespace zSocket
{

class InetSocket;

//**********************************************************************
// Class: zSocket::InetAddress
//**********************************************************************

class InetAddress : public Address
{

  friend InetSocket;

public:

  InetAddress(const std::string &addr_ = std::string("0.0.0.0:0"));

  InetAddress(const Address& addr_);

  InetAddress(const struct sockaddr_in& sa_);

  virtual
  ~InetAddress();

  virtual std::string
  GetAddress() const;

  virtual bool
  SetAddress(const std::string &addr_);

protected:

  struct sockaddr_in sa;

private:

};

//**********************************************************************
// zSocket::InetSocket Class
//**********************************************************************

class InetSocket : public Socket, public zThread::ThreadArg
{

public:

  InetSocket();

  virtual
  ~InetSocket();

  virtual bool
  Getopt(Socket::OPTIONS opt_);

  virtual bool
  Setopt(Socket::OPTIONS opt_);

protected:

  virtual int
  _get_fd();

  virtual bool
  _bind();

  virtual ssize_t
  _recv();

  virtual ssize_t
  _send(const Address& to_, const Buffer& sb_);

private:

  int _sock;
  InetAddress _sa;

};

}
}

#endif /* __ZINETSOCKET_H__ */
