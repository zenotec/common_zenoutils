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

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// InetAddress Class
//**********************************************************************

class InetAddress : public SocketAddress
{

public:

  InetAddress(const std::string &addr_ = std::string("0.0.0.0:0"));

  InetAddress(SocketAddress &addr_);

  InetAddress(const SocketAddress &addr_);

  virtual
  ~InetAddress();

protected:

private:

  virtual bool
  verify(const SocketType type_, const std::string &addr_);

};

//**********************************************************************
// zSocket::InetSocketRecv Class
//**********************************************************************

class InetSocketRecv : public zThread::ThreadFunction
{

public:

  InetSocketRecv()
  {
  }

  virtual
  ~InetSocketRecv()
  {
  }

  virtual void
  Run(zThread::ThreadArg *arg_);

protected:

private:

};

//**********************************************************************
// zSocket::InetSocketSend Class
//**********************************************************************

class InetSocketSend : public zThread::ThreadFunction
{

public:

  InetSocketSend()
  {
  }

  virtual
  ~InetSocketSend()
  {
  }

  virtual void
  Run(zThread::ThreadArg *arg_);

protected:

private:

};

//**********************************************************************
// zSocket::InetSocket Class
//**********************************************************************

class InetSocket : public Socket, public zThread::ThreadArg
{

  friend InetSocketRecv;
  friend InetSocketSend;

public:

  InetSocket();

  virtual
  ~InetSocket();

  virtual bool
  Open();

  virtual void
  Close();

  virtual bool
  Bind();

  virtual bool
  Connect(const SocketAddress* addr_);

protected:

  int _sock;

  virtual ssize_t
  _recv(zSocket::InetAddress &src_, zSocket::SocketBuffer &sb_);

  virtual ssize_t
  _send(const zSocket::InetAddress &dst_, zSocket::SocketBuffer &sb_);

private:

  zThread::Thread _rx_thread;
  InetSocketRecv _rx_func;
  zThread::Thread _tx_thread;
  InetSocketSend _tx_func;

};

}
}

#endif /* __ZINETSOCKET_H__ */
