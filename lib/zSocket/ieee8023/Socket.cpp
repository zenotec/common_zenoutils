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
#include <endian.h>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
#include <zutils/zSocket.h>
#include <zutils/zRawSocket.h>
using namespace zUtils;
#include <zutils/ieee8023/Frame.h>
#include <zutils/ieee8023/EtherFrame.h>
#include <zutils/ieee8023/LlcFrame.h>
#include <zutils/ieee8023/Ether2Frame.h>
#include <zutils/ieee8023/VlanFrame.h>
#include <zutils/ieee8023/Socket.h>

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{
namespace ieee8023
{

//*****************************************************************************
// Class: ieee8023::Notification
//*****************************************************************************

Notification::Notification(Socket& sock_) :
    zSocket::Notification(sock_)
{
  this->SetFrame(SHARED_PTR(ieee8023::Frame)(new ieee8023::Frame));
}

Notification::Notification(const zSocket::Notification& n_) :
    zSocket::Notification(n_)
{

  SHARED_PTR(ieee8023::Frame)frame(new ieee8023::Frame);

  // Peek at the 802.3 frame to determine its type & protocol
  if (!frame->Peek(*this->GetBuffer()))
  {
    ZLOG_WARN("Cannot decode IEEE8023 frame");
    this->SetSubType(Notification::SUBTYPE_PKT_ERR);
    return;
  }

  // Complete parsing of frame based on type
  switch (frame->GetSubtype())
  {
    case Frame::SUBTYPE_ETHER:
    {
      this->SetFrame(SHARED_PTR(ieee8023::EtherFrame)(new ieee8023::EtherFrame));
      if (!this->GetFrame()->Disassemble(*this->GetBuffer()))
      {
        ZLOG_WARN("Cannot decode ether frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
      break;
    }
    case Frame::SUBTYPE_LLC:
    {
      this->SetFrame(SHARED_PTR(ieee8023::LlcFrame)(new ieee8023::LlcFrame));
      if (!this->GetFrame()->Disassemble(*this->GetBuffer()))
      {
        ZLOG_WARN("Cannot decode LLC frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
      break;
    }
    case Frame::SUBTYPE_ETHER2:
    {
      this->SetFrame(SHARED_PTR(ieee8023::Ether2Frame)(new ieee8023::Ether2Frame));
      if (!this->GetFrame()->Disassemble(*this->GetBuffer()))
      {
        ZLOG_WARN("Cannot decode ether2 frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
      break;
    }
    case Frame::SUBTYPE_VLAN:
    {
      this->SetFrame(SHARED_PTR(ieee8023::VlanFrame)(new ieee8023::VlanFrame));
      if (!this->GetFrame()->Disassemble(*this->GetBuffer()))
      {
        ZLOG_WARN("Cannot decode VLAN frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
      break;
    }
    default:
      break;
  }

}

Notification::~Notification()
{
}

//*****************************************************************************
// Class: ieee8023::Socket
//*****************************************************************************


Socket::Socket(zSocket::Socket& sock_) :
    zSocket::Adapter(sock_)
{
}

Socket::~Socket()
{
}

SHARED_PTR(zSocket::Notification)
Socket::Recv()
{

  // Receive frame and convert to wireless notification
  SHARED_PTR(Notification) n(new Notification(*this->socket.Recv()));

  // Return wireless notification
  return (n);
}

SHARED_PTR(zSocket::Notification)
Socket::Send(const zSocket::Address& to_, const zSocket::Buffer& sb_)
{
  return (this->socket.Send(to_, sb_));
}

SHARED_PTR(zSocket::Notification)
Socket::Send(Frame& frame_)
{

  SHARED_PTR(Notification) n(new Notification(*this));
  zSocket::Buffer sb;

  // Assemble frame (writes buffer)
  if (!frame_.Assemble(sb))
  {
    ZLOG_ERR("Error assembling IEEE8023 frame");
    n->SetSubType(zSocket::Notification::SUBTYPE_PKT_ERR);
    return (n);
  }

  n = STATIC_CAST(Notification)(this->Send(zSocket::MacAddress(frame_.GetDestination()), sb));
  n->SetFrame(SHARED_PTR(Frame)(new Frame(frame_)));

  return (n);
}

void
Socket::Display()
{
  return;
}

}
}
}
