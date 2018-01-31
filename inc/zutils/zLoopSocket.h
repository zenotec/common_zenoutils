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

#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// Class zSocket::LoopAddress
//**********************************************************************

class LoopAddress : public Address
{

public:

  LoopAddress(const std::string& addr_ = "") :
    Address(SOCKET_TYPE::TYPE_LOOP, addr_)
  {
  }

  virtual
  ~LoopAddress()
  {
  }

protected:

private:

};

//**********************************************************************
// Class: zSocket::LoopSocket
//**********************************************************************

class LoopSocket :
    public Socket
{

public:

  LoopSocket();

  virtual
  ~LoopSocket();

protected:

  virtual bool
  _bind();

  virtual ssize_t
  _recv();

  virtual ssize_t
  _send(const Address& to_, const Buffer& sb_);

private:

  bool _bound;

};

}
}

#endif /* __ZLOOPSOCKET_H__ */
