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
  this->Frame(SHARED_PTR(ieee8023::Frame)(new ieee8023::Frame));
}

Notification::Notification(const zSocket::Notification& noti_) :
    zSocket::Notification(noti_)
{
  this->Frame(SHARED_PTR(ieee8023::Frame)(new ieee8023::Frame));
  uint8_t* f = this->GetBuffer().Data();
  size_t rem = this->GetBuffer().Length();

  // Peek at the 802.3 frame to determine its type & protocol
  f = this->Frame()->Peek(f, rem, false);
  if (f == NULL)
  {
    ZLOG_WARN("Cannot decode IEEE8023 frame");
    this->SetSubType(Notification::SUBTYPE_PKT_ERR);
    return;
  }

  // Complete parsing of frame based on type
  switch (this->Frame()->GetSubtype())
  {
  case Frame::SUBTYPE_ETHER:
  {
    this->Frame(SHARED_PTR(ieee8023::EtherFrame)(new ieee8023::EtherFrame));
    f = this->Frame()->Disassemble(f, rem, false);
    if (f == 0)
    {
      ZLOG_WARN("Cannot decode ether frame");
      this->SetSubType(Notification::SUBTYPE_PKT_ERR);
    }
    break;
  }
  case Frame::SUBTYPE_LLC:
  {
    this->Frame(SHARED_PTR(ieee8023::LlcFrame)(new ieee8023::LlcFrame));
    f = this->Frame()->Disassemble(f, rem, false);
    if (f == 0)
    {
      ZLOG_WARN("Cannot decode LLC frame");
      this->SetSubType(Notification::SUBTYPE_PKT_ERR);
    }
    break;
  }
  case Frame::SUBTYPE_ETHER2:
  {
    this->Frame(SHARED_PTR(ieee8023::Ether2Frame)(new ieee8023::Ether2Frame));
    f = this->Frame()->Disassemble(f, rem, false);
    if (f == 0)
    {
      ZLOG_WARN("Cannot decode ether2 frame");
      this->SetSubType(Notification::SUBTYPE_PKT_ERR);
    }
    break;
  }
  case Frame::SUBTYPE_VLAN:
  {
    this->Frame(SHARED_PTR(ieee8023::VlanFrame)(new ieee8023::VlanFrame));
    f = this->Frame()->Disassemble(f, rem, false);
    if (f == 0)
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

SHARED_PTR(ieee8023::Frame)
Notification::Frame()
{
  return(this->_frame);
}

bool
Notification::Frame(SHARED_PTR(ieee8023::Frame)frame_)
{
  this->_frame = frame_;
  return(true);
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
  zSocket::Buffer sb;
  uint8_t* sbptr = sb.Head();
  size_t sbsize = sb.TotalSize();

  // Assemble frame (writes buffer)
  if (!(sbptr = frame_.Assemble(sbptr, sbsize)))
  {
    ZLOG_ERR("Error assembling IEEE80211 header");
    SHARED_PTR(Notification) n(new Notification(*this));
    n->SetSubType(zSocket::Notification::SUBTYPE_PKT_ERR);
    return (n);
  }

  if (!sb.Put(sb.TotalSize() - sbsize))
  {
    ZLOG_ERR("Error updating socket buffer size");
    SHARED_PTR(Notification) n(new Notification(*this));
    n->SetSubType(zSocket::Notification::SUBTYPE_PKT_ERR);
    return (n);
  }

  return (this->Send(zSocket::EthAddress(frame_.GetDestination()), sb));
}

void
Socket::Display()
{
  return;
}

}
}
}
