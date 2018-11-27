/*
 * Copyright (c) 2018 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
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

// libc includes

#include <string.h>

// libc++ includes


// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;
#include <zutils/zSocket.h>

// local includes

ZLOG_MODULE_INIT(zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

//*****************************************************************************
// Class: zSocket::Frame
//*****************************************************************************

Frame::Frame(const Frame::TYPE type_) :
    _type(type_)
{
}

Frame::~Frame()
{
}

Frame::TYPE
Frame::GetType() const
{
  return (this->_type);
}

bool
Frame::SetType(const Frame::TYPE type_)
{
  bool status = false;
  switch (type_)
  {
  case Frame::TYPE_NONE ... Frame::TYPE_LAST:
    this->_type = type_;
    status = true;
    break;
  default:
    break;
  }
  return (true);
}

std::string
Frame::GetDestination() const
{
  return (this->_dst);
}

bool
Frame::SetDestination(const std::string& dst_)
{
  this->_dst = dst_;
  return (true);
}

std::string
Frame::GetSource() const
{
  return (this->_src);
}

bool
Frame::SetSource(const std::string& src_)
{
  this->_src = src_;
  return (true);
}

size_t
Frame::GetPayload(uint8_t* buf_, const size_t len_) const
{
  size_t cnt = std::min(this->_payload.size(), len_);
  memcpy(buf_, this->_payload.data(), cnt);
  return(cnt);
}

size_t
Frame::GetPayloadLength() const
{
  return(this->_payload.size());
}

bool
Frame::PutPayload(const uint8_t* buf_, const size_t len_)
{
  bool status = false;

  if (!len_)
  {
    ZLOG_WARN("Zero length payload");
  }

  if (buf_ && (len_ < 7952))
  {
    this->_payload.clear();
    this->_payload.resize(len_);
    status = (memcpy(this->_payload.data(), buf_, len_) == this->_payload.data());
  }
  return (status);
}

void
Frame::Display(const std::string& prefix_) const
{
}

}
}
