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

#ifndef __ZUDPSOCKET_H__
#define __ZUDPSOCKET_H__

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
// Class: zSocket::UdpSocketRx
//**********************************************************************

class UdpSocketRx :
    public zThread::ThreadFunction
{

public:

protected:

  virtual void
  Run(zThread::ThreadArg *arg_);

private:

};

//**********************************************************************
// Class: zSocket::UdpSocketTx
//**********************************************************************

class UdpSocketTx :
    public zThread::ThreadFunction
{

public:

protected:

  virtual void
  Run(zThread::ThreadArg *arg_);

private:

};

//**********************************************************************
// Class: zSocket::UdpSocket
//**********************************************************************

class UdpSocket :
    public Socket,
    public zThread::ThreadArg
{

public:

  friend UdpSocketRx;
  friend UdpSocketTx;

  UdpSocket();

  virtual
  ~UdpSocket();

  virtual bool
  Getopt(Socket::OPTIONS opt_);

  virtual bool
  Setopt(Socket::OPTIONS opt_);

  virtual bool
  Bind(const Address& addr_);

protected:

  int fd;

  // Receives and returns notification
  virtual SHARED_PTR(zSocket::Notification)
  recv();

  // Sends from transmit queue and returns notification
  virtual SHARED_PTR(zSocket::Notification)
  send(SHARED_PTR(zSocket::Notification) n_);

private:

  zThread::Thread _rxthread;
  UdpSocketRx _rxfunc;
  zThread::Thread _txthread;
  UdpSocketTx _txfunc;

};

}
}

#endif /* __ZUDPSOCKET_H__ */
