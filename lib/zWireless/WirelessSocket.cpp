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
#include <net/if.h>

// libc++ includes
#include <iostream>

// libzutils includes

#include <zutils/zLog.h>
#include <zutils/ieee80211/Frame.h>
#include <zutils/ieee80211/RadioTap.h>
#include <zutils/ieee80211/Beacon.h>
#include <zutils/ieee80211/Probe.h>
#include <zutils/ieee80211/ControlFrame.h>
#include <zutils/ieee80211/DataFrame.h>
#include <zutils/ieee80211/Association.h>
#include <zutils/ieee80211/Disassociation.h>
#include <zutils/ieee80211/Reassociation.h>
#include <zutils/ieee80211/Authentication.h>
#include <zutils/ieee80211/Deauthentication.h>
#include <zutils/ieee80211/Action.h>
#include <zutils/zWirelessSocket.h>

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{

//*****************************************************************************
// Class: zWireless::Notification
//*****************************************************************************

Notification::Notification(Socket& sock_) :
    zSocket::Notification(sock_)
{
  this->RadiotapHeader(SHARED_PTR(ieee80211::RadioTap)(new ieee80211::RadioTap));
  this->Frame(SHARED_PTR(ieee80211::Frame)(new ieee80211::Frame));
}

Notification::Notification(const zSocket::Notification& noti_) :
    zSocket::Notification(noti_)
{

  this->RadiotapHeader(SHARED_PTR(ieee80211::RadioTap)(new ieee80211::RadioTap));
  this->Frame(SHARED_PTR(ieee80211::Frame)(new ieee80211::Frame));
  uint8_t* f = this->GetBuffer().Data();
  size_t rem = this->GetBuffer().Length();

  // Process radiotap header
  f = this->RadiotapHeader()->Disassemble(f, rem);
  if (f == 0)
  {
    ZLOG_WARN("Cannot decode radiotap header");
    this->SetSubType(Notification::SUBTYPE_PKT_ERR);
    return;
  }

  // Check for presence of FCS
  ieee80211::RadioTapFieldFlags flags;
  this->RadiotapHeader()->GetField(flags);
  bool fcsflag = flags.FCS();

  // Check for bad FCS
  if (flags.BadFCS())
  {
    ZLOG_INFO("Bad FCS");
    this->SetSubType(Notification::SUBTYPE_PKT_ERR);
    return;
  }

  // Check for TX flags indicating this was a frame we transmitted
  ieee80211::RadioTapFieldTxFlags txflags;
  if (this->RadiotapHeader()->GetField(txflags))
  {
    this->SetSubType(zSocket::Notification::SUBTYPE_PKT_SENT);
  }

  // Peek at the 802.11 frame to determine its type/subtype
  f = this->Frame()->Peek(f, rem, fcsflag);
  if (f == 0)
  {
    ZLOG_WARN("Cannot decode IEEE80211 frame");
    this->SetSubType(Notification::SUBTYPE_PKT_ERR);
    return;
  }

  // Complete parsing of frame based on type/subtype
  switch (this->Frame()->Type())
  {
  case ieee80211::Frame::TYPE_MGMT:
  {
    if ((this->Frame()->Subtype() == ieee80211::Frame::SUBTYPE_BEACON))
    {

      this->Frame(SHARED_PTR(ieee80211::Beacon)(new ieee80211::Beacon));
      f = this->Frame()->Disassemble(f, rem, fcsflag);
      if (f == 0)
      {
        ZLOG_WARN("Cannot decode beacon frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((this->Frame()->Subtype() == ieee80211::Frame::SUBTYPE_PROBEREQ))
    {
      this->Frame(SHARED_PTR(ieee80211::ProbeRequest)(new ieee80211::ProbeRequest));
      f = this->Frame()->Disassemble(f, rem, fcsflag);
      if (f == 0)
      {
        ZLOG_WARN("Cannot decode probe request frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((this->Frame()->Subtype() == ieee80211::Frame::SUBTYPE_PROBERESP))
    {
      this->Frame(SHARED_PTR(ieee80211::ProbeResponse)(new ieee80211::ProbeResponse));
      f = this->Frame()->Disassemble(f, rem, fcsflag);
      if (f == 0)
      {
        ZLOG_WARN("Cannot decode probe response frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((this->Frame()->Subtype() == ieee80211::Frame::SUBTYPE_ASSREQ))
    {
      this->Frame(SHARED_PTR(ieee80211::AssociationRequest)(new ieee80211::AssociationRequest));
      f = this->Frame()->Disassemble(f, rem, fcsflag);
      if (f == 0)
      {
        ZLOG_WARN("Cannot decode association request frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((this->Frame()->Subtype() == ieee80211::Frame::SUBTYPE_RASSREQ))
    {
      this->Frame(SHARED_PTR(ieee80211::ReassociationRequest)(new ieee80211::ReassociationRequest));
      f = this->Frame()->Disassemble(f, rem, fcsflag);
      if (f == 0)
      {
        ZLOG_WARN("Cannot decode reassociation request frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((this->Frame()->Subtype() == ieee80211::Frame::SUBTYPE_DISASS))
    {
      this->Frame(SHARED_PTR(ieee80211::Disassociation)(new ieee80211::Disassociation));
      f = this->Frame()->Disassemble(f, rem, fcsflag);
      if (f == 0)
      {
        ZLOG_WARN("Cannot decode disassociation frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((this->Frame()->Subtype() == ieee80211::Frame::SUBTYPE_ASSRESP))
    {
      this->Frame(SHARED_PTR(ieee80211::AssociationResponse)(new ieee80211::AssociationResponse));
      f = this->Frame()->Disassemble(f, rem, fcsflag);
      if (f == 0)
      {
        ZLOG_WARN("Cannot decode association response frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((this->Frame()->Subtype() == ieee80211::Frame::SUBTYPE_AUTHENTICATE))
    {
      this->Frame(SHARED_PTR(ieee80211::Authentication)(new ieee80211::Authentication));
      f = this->Frame()->Disassemble(f, rem, fcsflag);
      if (f == 0)
      {
        ZLOG_WARN("Cannot decode authentication frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((this->Frame()->Subtype() == ieee80211::Frame::SUBTYPE_DEAUTH))
    {
      this->Frame(SHARED_PTR(ieee80211::Deauthentication)(new ieee80211::Deauthentication));
      f = this->Frame()->Disassemble(f, rem, fcsflag);
      if (f == 0)
      {
        ZLOG_WARN("Cannot decode deauthentication frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((this->Frame()->Subtype() == ieee80211::Frame::SUBTYPE_ACTION))
    {
      this->Frame(SHARED_PTR(ieee80211::ActionRequest)(new ieee80211::ActionRequest));
      f = this->Frame()->Disassemble(f, rem, fcsflag);
      if (f == 0)
      {
//        ZLOG_WARN("Cannot decode action request frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    break;
  }
  case ieee80211::Frame::TYPE_CNTL:
  {
    this->Frame(SHARED_PTR(ieee80211::ControlFrame)(new ieee80211::ControlFrame));
    f = this->Frame()->Disassemble(f, rem, fcsflag);
    if (f == 0)
    {
      ZLOG_WARN("Cannot decode control frame");
      this->SetSubType(Notification::SUBTYPE_PKT_ERR);
    }
    break;
  }
  case ieee80211::Frame::TYPE_DATA:
  {
    this->Frame(SHARED_PTR(ieee80211::DataFrame)(new ieee80211::DataFrame));
    f = this->Frame()->Disassemble(f, rem, fcsflag);
    if (f == 0)
    {
      ZLOG_WARN("Cannot decode data frame");
      this->SetSubType(Notification::SUBTYPE_PKT_ERR);
    }
    break;
  }
  default:
  {
      ZLOG_WARN("Unsupported frame type / subtype: " + zLog::HexStr(this->Frame()->Type()) + " / "
          + zLog::HexStr(this->Frame()->Subtype()));
      this->Frame()->Display();
    this->SetSubType(Notification::SUBTYPE_PKT_ERR);
    break;
  }
  } // end ieee80211hdr.Type()

  return;
}

Notification::~Notification()
{
}

SHARED_PTR(ieee80211::RadioTap)
Notification::RadiotapHeader()
{
  return (this->_rtaphdr);
}

bool
Notification::RadiotapHeader(SHARED_PTR(ieee80211::RadioTap)rtaphdr_)
{
  this->_rtaphdr = rtaphdr_;
  return (true);
}

SHARED_PTR(ieee80211::Frame)
Notification::Frame()
{
  return(this->_frame);
}

bool
Notification::Frame(SHARED_PTR(ieee80211::Frame)frame_)
{
  this->_frame = frame_;
  return(true);
}

//*****************************************************************************
// Class: zWireless::Socket
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

  // Update destination address from actual frame
  zSocket::Address daddr(zSocket::Socket::GetType(), n->Frame()->Address(ieee80211::Frame::ADDRESS_1));
  n->SetDstAddress(daddr);

  // Update source address from actual frame
  zSocket::Address saddr(zSocket::Socket::GetType(), n->Frame()->Address(ieee80211::Frame::ADDRESS_2));
  n->SetSrcAddress(saddr);

  if ((n->Frame()->Type() == ieee80211::Frame::TYPE_CNTL) &&
      (n->Frame()->Subtype() == ieee80211::Frame::SUBTYPE_RTS) &&
      (n->Frame()->Address(ieee80211::Frame::ADDRESS_1) == this->socket.GetAddress().GetAddress()))
  {
    n->Frame()->Display();
  }

  // Return wireless notification
  return (n);
}

SHARED_PTR(zSocket::Notification)
Socket::Send(const zSocket::Address& to_, const zSocket::Buffer& sb_)
{
  return (this->socket.Send(to_, sb_));
}

SHARED_PTR(zSocket::Notification)
Socket::Send(ieee80211::RadioTap hdr_, ieee80211::Frame& frame_)
{

  zSocket::Buffer sb;
  uint8_t* sbptr = sb.Head();
  size_t sbsize = sb.TotalSize();

  if (!(sbptr = hdr_.Assemble(sbptr, sbsize)))
  {
    ZLOG_ERR("Error assembling radiotap header");
    SHARED_PTR(Notification) n(new Notification(*this));
    n->SetSubType(zSocket::Notification::SUBTYPE_PKT_ERR);
    return (n);
  }

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

  zSocket::Address daddr(zSocket::Socket::GetType(), frame_.Address(ieee80211::Frame::ADDRESS_1));
  return (this->Send(daddr, sb));
}

void
Socket::Display()
{
  return;
}

}
}
