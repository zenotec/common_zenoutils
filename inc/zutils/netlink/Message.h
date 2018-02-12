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

#ifndef __NETLINK_MESSAGE_H__
#define __NETLINK_MESSAGE_H__

#include <stdint.h>

#include <netlink/netlink.h>
#include <netlink/msg.h>
#include <netlink/attr.h>

#include <string>

#include <zutils/netlink/Attribute.h>

namespace netlink
{

//*****************************************************************************
// Class: Message
//*****************************************************************************

class Message
{
public:

  Message(struct nl_msg* msg_ = NULL);

  virtual
  ~Message();

  struct nl_msg*
  operator()() const;

  uint16_t
  Type() const;

  bool
  Type(const uint16_t type_);

  virtual bool
  Parse() = 0;

  template<typename T>
    bool
    GetAttribute(Attribute<T>* attr_)
    {
      bool status = false;
      if (attr_ && this->GetAttribute(attr_->Id(), attr_->operator ()()))
      {
        attr_->SetValid();
        status = true;
      }
      return (status);
    }

  bool
  GetAttribute(const int attr_, uint8_t& value_);

  bool
  GetAttribute(const int attr_, uint32_t& value_);

  bool
  GetAttribute(const int attr_, int32_t& value_);

  bool
  GetAttribute(const int attr_, std::string& value_);

  bool
  GetAttribute(const int attr_, AttributeBuffer buf_);

  template<typename T>
    bool
    PutAttribute(Attribute<T>* attr_)
    {
      bool status = false;
      if (attr_ && attr_->IsValid())
      {
        status = this->PutAttribute(attr_->Id(), attr_->GetValue());
      }
      return (status);
    }

  bool
  PutAttribute(const int attr_, const uint8_t value_);

  bool
  PutAttribute(const int attr_, const uint32_t value_);

  bool
  PutAttribute(const int attr_, const int32_t value_);

  bool
  PutAttribute(const int attr_, const std::string& value_);

  bool
  PutAttribute(const int attr_, AttributeBuffer buf_);

  struct nlmsghdr*
  Header() const;

  void*
  Data() const;

  size_t
  Length() const;

  void
  Display() const;

protected:

  struct nlattr* _attrs[256]; // Worse case number of attributes returned by a single message

private:

  struct nl_msg* _msg;

};

}

#endif /* __NETLINK_MESSAGE_H__ */
