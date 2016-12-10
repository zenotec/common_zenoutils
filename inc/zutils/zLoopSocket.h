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

#ifndef __ZLOOPSOCKET_H__
#define __ZLOOPSOCKET_H__

#include <string>

#include <zutils/zThread.h>
#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// LoopAddress Class
//**********************************************************************

class LoopAddress : public SocketAddress
{

public:

  LoopAddress(const std::string &name_ = std::string(""));

  virtual
  ~LoopAddress();

protected:

  virtual bool
  verify(const SocketType type_, const std::string &addr_);

private:

};

//**********************************************************************
// zSocket::LoopSocket Class
//**********************************************************************

class LoopSocket : public Socket, public zThread::ThreadFunction
{

public:

  LoopSocket();

  virtual
  ~LoopSocket();

  virtual bool
  Open();

  virtual void
  Close();

  virtual bool
  Bind();

  virtual bool
  Connect(const SocketAddress* addr_);

protected:

  virtual void
  Run(zThread::ThreadArg *arg_);

private:

  zThread::Thread _thread;
  bool _opened;
  bool _bound;
  bool _connected;

};

}
}

#endif /* __ZLOOPSOCKET_H__ */
