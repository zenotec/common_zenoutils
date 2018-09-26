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
#include <zutils/zMacAddress.h>
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
#include <zutils/ieee80211/Socket.h>

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: ieee80211::Notification
//*****************************************************************************

Notification::Notification(Socket& sock_) :
    zSocket::Notification(sock_)
{
}

Notification::Notification(const zSocket::Notification& n_) :
    zSocket::Notification(n_)
{

  if (!this->GetBuffer().get() || !this->GetBuffer()->Length())
  {
    this->SetSubType(zSocket::Notification::SUBTYPE_PKT_ERR);
    ZLOG_ERR("Error parsing socket notification: No data");
    return;
  }

  SHARED_PTR(zSocket::Buffer) sb(this->GetBuffer());
  SHARED_PTR(ieee80211::RadioTap) rthdr(new ieee80211::RadioTap);
  ieee80211::Frame frame(ieee80211::Frame::TYPE_NONE);

  // Process radiotap header
  if (!rthdr || !rthdr->Disassemble(*(sb)))
  {
    ZLOG_WARN("Cannot decode radiotap header");
    this->SetSubType(Notification::SUBTYPE_PKT_ERR);
    return;
  }

  // Update notification with radiotap header
  this->RadiotapHeader(rthdr);

  // Check for presence of FCS
  ieee80211::RadioTapFieldFlags flags;
  rthdr->GetField(flags);
  bool fcsflag = flags.FCS();

  // Check for bad FCS
  if (flags.BadFCS())
  {
    ZLOG_INFO("Bad FCS");
    this->SetSubType(Notification::SUBTYPE_PKT_ERR);
    return;
  }

  // Check for TX flags indicating this was a frame we transmitted then continue with disassembly
  ieee80211::RadioTapFieldTxFlags txflags;
  if (this->RadiotapHeader()->GetField(txflags))
  {
    this->SetSubType(zSocket::Notification::SUBTYPE_PKT_SENT);
  }

  // Peek at the 802.11 frame to determine its type/subtype
  if (!frame.Peek(*sb))
  {
    ZLOG_WARN("Cannot decode IEEE80211 frame");
    this->SetSubType(Notification::SUBTYPE_PKT_ERR);
    return;
  }

  // Complete parsing of frame based on type/subtype
  switch (frame.Type())
  {
  case ieee80211::Frame::TYPE_MGMT:
  {
    if ((frame.Subtype() == ieee80211::Frame::SUBTYPE_BEACON))
    {

      this->SetFrame(SHARED_PTR(ieee80211::Beacon)(new ieee80211::Beacon));
      if (!this->GetFrame()->Disassemble(*sb))
      {
        ZLOG_WARN("Cannot decode beacon frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((frame.Subtype() == ieee80211::Frame::SUBTYPE_PROBEREQ))
    {
      this->SetFrame(SHARED_PTR(ieee80211::ProbeRequest)(new ieee80211::ProbeRequest));
      if (!this->GetFrame()->Disassemble(*sb))
      {
        ZLOG_WARN("Cannot decode probe request frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((frame.Subtype() == ieee80211::Frame::SUBTYPE_PROBERESP))
    {
      this->SetFrame(SHARED_PTR(ieee80211::ProbeResponse)(new ieee80211::ProbeResponse));
      if (!this->GetFrame()->Disassemble(*sb))
      {
        ZLOG_WARN("Cannot decode probe response frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((frame.Subtype() == ieee80211::Frame::SUBTYPE_ASSREQ))
    {
      this->SetFrame(SHARED_PTR(ieee80211::AssociationRequest)(new ieee80211::AssociationRequest));
      if (!this->GetFrame()->Disassemble(*sb))
      {
        ZLOG_WARN("Cannot decode association request frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((frame.Subtype() == ieee80211::Frame::SUBTYPE_RASSREQ))
    {
      this->SetFrame(SHARED_PTR(ieee80211::ReassociationRequest)(new ieee80211::ReassociationRequest));
      if (!this->GetFrame()->Disassemble(*sb))
      {
        ZLOG_WARN("Cannot decode reassociation request frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((frame.Subtype() == ieee80211::Frame::SUBTYPE_DISASS))
    {
      this->SetFrame(SHARED_PTR(ieee80211::Disassociation)(new ieee80211::Disassociation));
      if (!this->GetFrame()->Disassemble(*sb))
      {
        ZLOG_WARN("Cannot decode disassociation frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((frame.Subtype() == ieee80211::Frame::SUBTYPE_ASSRESP))
    {
      this->SetFrame(SHARED_PTR(ieee80211::AssociationResponse)(new ieee80211::AssociationResponse));
      if (!this->GetFrame()->Disassemble(*sb))
      {
        ZLOG_WARN("Cannot decode association response frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((frame.Subtype() == ieee80211::Frame::SUBTYPE_AUTHENTICATE))
    {
      this->SetFrame(SHARED_PTR(ieee80211::Authentication)(new ieee80211::Authentication));
      if (!this->GetFrame()->Disassemble(*sb))
      {
        ZLOG_WARN("Cannot decode authentication frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((frame.Subtype() == ieee80211::Frame::SUBTYPE_DEAUTH))
    {
      this->SetFrame(SHARED_PTR(ieee80211::Deauthentication)(new ieee80211::Deauthentication));
      if (!this->GetFrame()->Disassemble(*sb))
      {
        ZLOG_WARN("Cannot decode deauthentication frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    else if ((frame.Subtype() == ieee80211::Frame::SUBTYPE_ACTION))
    {
      this->SetFrame(SHARED_PTR(ieee80211::ActionRequest)(new ieee80211::ActionRequest));
      if (!this->GetFrame()->Disassemble(*sb))
      {
//        ZLOG_WARN("Cannot decode action request frame");
        this->SetSubType(Notification::SUBTYPE_PKT_ERR);
      }
    }
    break;
  }
  case ieee80211::Frame::TYPE_CNTL:
  {
    this->SetFrame(SHARED_PTR(ieee80211::ControlFrame)(new ieee80211::ControlFrame));
    if (!this->GetFrame()->Disassemble(*sb))
    {
      ZLOG_WARN("Cannot decode control frame");
      this->SetSubType(Notification::SUBTYPE_PKT_ERR);
    }
    break;
  }
  case ieee80211::Frame::TYPE_DATA:
  {
    this->SetFrame(SHARED_PTR(ieee80211::DataFrame)(new ieee80211::DataFrame));
    if (!this->GetFrame()->Disassemble(*sb))
    {
      ZLOG_WARN("Cannot decode data frame");
      this->SetSubType(Notification::SUBTYPE_PKT_ERR);
    }
    break;
  }
  default:
  {
      ZLOG_WARN("Unsupported frame type / subtype: " + zLog::HexStr(frame.Type()) + " / "
          + zLog::HexStr(frame.Subtype()));
      frame.Display();
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

//*****************************************************************************
// Class: ieee80211::Socket
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

  if (n.get() && (n->GetSubType() == zSocket::Notification::SUBTYPE_PKT_RCVD))
  {
    std::string addr;
    // Update destination address from actual frame
    SHARED_PTR(zSocket::MacAddress) dst(new zSocket::MacAddress(n->GetFrame()->GetDestination()));
    n->SetDstAddress(dst);

    // Update source address from actual frame
    SHARED_PTR(zSocket::MacAddress) src(new zSocket::MacAddress(n->GetFrame()->GetSource()));
    n->SetSrcAddress(src);
  }
  else
  {
    fprintf(stderr, "BUG: Unknown frame: %d\n", n->GetSubType());
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

  if (!hdr_.Assemble(sb))
  {
    ZLOG_ERR("Error assembling radiotap header");
    SHARED_PTR(Notification) n(new Notification(*this));
    n->SetSubType(zSocket::Notification::SUBTYPE_PKT_ERR);
    return (n);
  }

  // Assemble frame (writes buffer)
  if (!frame_.Assemble(sb))
  {
    ZLOG_ERR("Error assembling IEEE80211 header");
    SHARED_PTR(Notification) n(new Notification(*this));
    n->SetSubType(zSocket::Notification::SUBTYPE_PKT_ERR);
    return (n);
  }

  zSocket::MacAddress dst(frame_.GetAddress(ieee80211::Frame::ADDRESS_1));
  return (this->Send(dst, sb));
}

void
Socket::Display()
{
  return;
}

}
}
}
