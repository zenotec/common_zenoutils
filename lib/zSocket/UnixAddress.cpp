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

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <poll.h>

#include <zutils/zLog.h>
#include <zutils/zSocket.h>
#include <zutils/zUnixSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

static bool
_addr2sa(const std::string &addr_, struct sockaddr_un &sa_)
{
  bool status = false;
  if (!addr_.empty())
  {
    size_t len = (sizeof(sa_.sun_path) - 1); // subtract one for null terminator
    sa_.sun_family = AF_UNIX;
    status = (sa_.sun_path == strncpy(sa_.sun_path, addr_.c_str(), len));
  }
  return (status);
}

static bool
_sa2addr(const struct sockaddr_un &sa_, std::string &addr_)
{
  addr_ = std::string(sa_.sun_path);
  return (!addr_.empty());
}

//**********************************************************************
// Class: zSocket::UnixAddress
//**********************************************************************

UnixAddress::UnixAddress(const std::string &addr_) :
    Address(Address::TYPE_UNIX, addr_), _sa { 0 }
{
  _addr2sa(addr_, this->_sa);
}

UnixAddress::UnixAddress(const Address &addr_) :
    Address(Address::TYPE_UNIX), _sa { 0 }
{
  if (addr_.GetType() == this->GetType())
  {
    this->SetAddress(addr_.GetAddress());
  }
}

UnixAddress::UnixAddress(const struct sockaddr_un& sa_) :
        Address(Address::TYPE_UNIX), _sa (sa_)
{
  this->SetAddress(this->GetAddress());
}

UnixAddress::~UnixAddress()
{
}

std::string
UnixAddress::GetAddress() const
{
  std::string addr;
  _sa2addr(this->_sa, addr);
  return (addr);
}

bool
UnixAddress::SetAddress(const std::string& addr_)
{
  bool status = false;
  if (_addr2sa(addr_, this->_sa))
  {
    status = Address::SetAddress(addr_);
  }
  return (status);
}

struct sockaddr_un
UnixAddress::GetSA() const
{
  return (this->_sa);
}

bool
UnixAddress::SetSA(const struct sockaddr_un& sa_)
{
  bool status = false;
  if (sa_.sun_family == AF_UNIX)
  {
    this->_sa = sa_;
    status = true;
  }
  return (status);
}

void
UnixAddress::Display() const
{
  Address::Display();
}

}
}
