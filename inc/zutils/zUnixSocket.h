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

#ifndef __ZUNIXSOCKET_H__
#define __ZUNIXSOCKET_H__

#include <string>

#include <zutils/zThread.h>
#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// UnixAddress Class
//**********************************************************************

class UnixAddress : public Address
{

public:

  UnixAddress(const std::string &addr_ = std::string(""));

  UnixAddress(Address &addr_);

  UnixAddress(const Address &addr_);

  virtual
  ~UnixAddress();

protected:

  virtual bool
  verify(const SocketType type_, const std::string &addr_);

private:

};

//**********************************************************************
// zSocket::UnixSocketRecv Class
//**********************************************************************

class UnixSocketRecv : public zThread::ThreadFunction
{
public:

  UnixSocketRecv()
  {
  }

  virtual
  ~UnixSocketRecv()
  {
  }

  virtual void
  Run(zThread::ThreadArg *arg_);

protected:

private:

};

//**********************************************************************
// zSocket::UnixSocketSend Class
//**********************************************************************

class UnixSocketSend : public zThread::ThreadFunction
{
public:

  UnixSocketSend()
  {
  }

  virtual
  ~UnixSocketSend()
  {

  }

  virtual void
  Run(zThread::ThreadArg *arg_);

protected:

private:

};

//**********************************************************************
// zSocket::UnixSocket Class
//**********************************************************************

class UnixSocket : public Socket, public zThread::ThreadArg
{

  friend UnixSocketRecv;
  friend UnixSocketSend;

public:

  UnixSocket();

  virtual
  ~UnixSocket();

  virtual bool
  Open();

  virtual void
  Close();

protected:

  int _sock;

  virtual bool
  _bind();

  virtual ssize_t
  _recv(zSocket::UnixAddress &src_, zSocket::Buffer &sb_);

  virtual ssize_t
  _send(const zSocket::UnixAddress &dst_, zSocket::Buffer &sb_);

private:

  zThread::Thread _rx_thread;
  UnixSocketRecv _rx_func;
  zThread::Thread _tx_thread;
  UnixSocketSend _tx_func;

};

}
}

#endif /* __ZUNIXSOCKET_H__ */
