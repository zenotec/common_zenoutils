/*
 * Copyright (c) 2017 Cable Television Laboratories, Inc. ("CableLabs")
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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netlink/netlink.h>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;

// local includes
#include "Attribute.h"

#include "Message.h"

namespace netlink
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: Message
//*****************************************************************************

Message::Message(struct nl_msg* msg_) :
    _msg(msg_)
{
  if (msg_)
  {
    nlmsg_get(msg_);
  }
  else
  {
    this->_msg = nlmsg_alloc();
  }
  memset((void*)this->_attrs, 0, sizeof(this->_attrs));
}

Message::~Message()
{
  if (this->_msg)
  {
    nlmsg_free(this->_msg);
    this->_msg = NULL;
  }
  memset((void*)this->_attrs, 0, sizeof(this->_attrs));
}

struct nl_msg*
Message::operator()() const
{
  return(this->_msg);
}

uint16_t
Message::Type() const
{
  uint16_t type = 0;
  if (this->Header())
  {
    type = this->Header()->nlmsg_type;
  }
  return(type);
}

bool
Message::Type(const uint16_t type_)
{
  bool status = false;
  if (this->Header())
  {
    this->Header()->nlmsg_type = type_;
    status = true;
  }
  return(status);
}

bool
Message::GetAttribute(const int attr_, uint8_t& value_)
{
  bool status = false;
  if (this->_attrs[attr_])
  {
    value_ = nla_get_u8(this->_attrs[attr_]);
    status = true;
  }
  return(status);
}

bool
Message::GetAttribute(const int attr_, uint32_t& value_)
{
  bool status = false;
  if (this->_attrs[attr_])
  {
    value_ = nla_get_u32(this->_attrs[attr_]);
    status = true;
  }
  return(status);
}

bool
Message::GetAttribute(const int attr_, std::string& value_)
{
  bool status = false;
  if (this->_attrs[attr_])
  {
    value_ = std::string(nla_get_string(this->_attrs[attr_]));
    status = true;
  }
  return(status);
}

bool
Message::GetAttribute(const int attr_, AttributeBuffer buf_)
{
  bool status = false;
  std::cout << "Message::GetAttribute(AttributeBuffer)" << std::endl;
  if (this->_attrs[attr_] && buf_.first && buf_.second)
  {
    char* m = (char*)nla_data(this->_attrs[attr_]);
    printf("MAC[0]: %p -> %02x\n", m, m[0]);
    memcpy(buf_.first, nla_data(this->_attrs[attr_]), buf_.second);
    status = true;
  }
  return(status);
}

bool
Message::PutAttribute(const int attr_, const uint8_t value_)
{
  int ret = nla_put_u8(this->_msg, attr_, value_);
  if (ret < 0)
  {
    ZLOG_ERR("Error putting message attributes");
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
    return (false);
  }
  return(true);
}

bool
Message::PutAttribute(const int attr_, const uint32_t value_)
{
  int ret = nla_put_u32(this->_msg, attr_, value_);
  if (ret < 0)
  {
    ZLOG_ERR("Error putting message attributes");
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
    return (false);
  }
  return(true);
}

bool
Message::PutAttribute(const int attr_, const std::string& value_)
{
  int ret = nla_put_string(this->_msg, attr_, value_.c_str());
  if (ret < 0)
  {
    ZLOG_ERR("Error putting message attributes");
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
    return (false);
  }
  return(true);
}

bool
Message::PutAttribute(const int attr_, AttributeBuffer buf_)
{
  if (!buf_.first || !buf_.second)
  {
    return(false);
  }

  int ret = nla_put(this->_msg, attr_, buf_.second, buf_.first);
  if (ret < 0)
  {
    ZLOG_ERR("Error putting message attributes");
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
    return (false);
  }
  return(true);
}

struct nlmsghdr*
Message::Header() const
{
  return (nlmsg_hdr(this->_msg));
}

void*
Message::Data() const
{
  return(nlmsg_data(this->Header()));
}

size_t
Message::Length() const
{
  return(nlmsg_datalen(this->Header()));
}

void
Message::Display() const
{
  nl_msg_dump(this->_msg, stdout);
}

}

