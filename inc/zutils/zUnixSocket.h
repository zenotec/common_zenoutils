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
// Class: zSocket::UnixSocketRx
//**********************************************************************

class UnixSocketRx :
    public zThread::ThreadFunction
{

public:

protected:

  virtual void
  Run(zThread::ThreadArg *arg_);

private:

};

//**********************************************************************
// Class: zSocket::UnixSocketTx
//**********************************************************************

class UnixSocketTx :
    public zThread::ThreadFunction
{

public:

protected:

  virtual void
  Run(zThread::ThreadArg *arg_);

private:

};

//**********************************************************************
// Class: zSocket::UnixSocket
//**********************************************************************

class UnixSocket :
    public Socket,
    public zThread::ThreadArg
{

public:

  friend UnixSocketRx;
  friend UnixSocketTx;

  UnixSocket();

  virtual
  ~UnixSocket();

  virtual bool
  Open();

  virtual bool
  Bind(const Address& addr_);

  virtual bool
  Close();

protected:

  int fd;

  // Receives and returns notification
  virtual SHPTR(zSocket::Notification)
  recv();

  // Sends from transmit queue and returns notification
  virtual SHPTR(zSocket::Notification)
  send(SHPTR(zSocket::Notification) n_);

private:

  zThread::Thread _rxthread;
  UnixSocketRx _rxfunc;
  zThread::Thread _txthread;
  UnixSocketTx _txfunc;

};

}
}

#endif /* __ZUNIXSOCKET_H__ */
