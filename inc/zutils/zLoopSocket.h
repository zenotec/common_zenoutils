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

#ifndef __ZLOOPSOCKET_H__
#define __ZLOOPSOCKET_H__

#include <string>

#include <zutils/zSocket.h>
#include <zutils/zLoopAddress.h>

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// Class: zSocket::LoopSocketTx
//**********************************************************************

class LoopSocketTx :
    public zThread::ThreadFunction
{

public:

protected:

  virtual void
  Run(zThread::ThreadArg *arg_);

private:

};

//**********************************************************************
// Class: LoopSocket
//**********************************************************************

class LoopSocket :
    public Socket,
    public zThread::ThreadArg
{

public:

  friend LoopSocketTx;

  LoopSocket();

  virtual
  ~LoopSocket();

  virtual bool
  Bind(const Address& addr_);

protected:

  // Sends from transmit queue and returns notification
  virtual SHARED_PTR(zSocket::Notification)
  send();

private:

  int _fd;

  zThread::Thread _txthread;
  LoopSocketTx _txfunc;

};

}
}

#endif /* __ZLOOPSOCKET_H__ */
