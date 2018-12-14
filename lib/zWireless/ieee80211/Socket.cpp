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

  // TODO: Only support notifications for received frames for now
  if (this->GetSubType() != zSocket::Notification::SUBTYPE_PKT_RCVD)
  {
    return;
  }

  if (!this->GetBuffer().get() || !this->GetBuffer()->Length())
  {
    this->SetSubType(zSocket::Notification::SUBTYPE_PKT_ERR);
    ZLOG_ERR("Error parsing socket notification: No data");
    return;
  }

  SHPTR(zSocket::Buffer)sb(this->GetBuffer());
  SHPTR(ieee80211::RadioTap)rthdr(new ieee80211::RadioTap);
  SHPTR(ieee80211::Frame) frame(new ieee80211::Frame(ieee80211::Frame::TYPE_NONE));

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
  if (!frame->Peek(*sb, fcsflag))
  {
    ZLOG_WARN("Cannot decode IEEE80211 frame");
    this->SetSubType(Notification::SUBTYPE_PKT_ERR);
    return;
  }

  // Complete parsing of frame based on type/subtype
  switch (frame->Type())
  {
    case ieee80211::Frame::TYPE_MGMT:
    {
      switch (frame->Subtype())
      {
        case ieee80211::Frame::SUBTYPE_BEACON:
          frame = SHPTR(ieee80211::Frame)(new ieee80211::Beacon);
          break;
        case ieee80211::Frame::SUBTYPE_PROBEREQ:
          frame = SHPTR(ieee80211::Frame)(new ieee80211::ProbeRequest);
          break;
        case ieee80211::Frame::SUBTYPE_PROBERESP:
          frame = SHPTR(ieee80211::Frame)(new ieee80211::ProbeResponse);
          break;
        case ieee80211::Frame::SUBTYPE_ASSREQ:
          frame = SHPTR(ieee80211::Frame)(new ieee80211::AssociationRequest);
          break;
        case ieee80211::Frame::SUBTYPE_ASSRESP:
          frame = SHPTR(ieee80211::Frame)(new ieee80211::AssociationResponse);
          break;
        case ieee80211::Frame::SUBTYPE_RASSREQ:
          frame = SHPTR(ieee80211::Frame)(new ieee80211::ReassociationRequest);
          break;
        case ieee80211::Frame::SUBTYPE_DISASS:
          frame = SHPTR(ieee80211::Frame)(new ieee80211::Disassociation);
          break;
        case ieee80211::Frame::SUBTYPE_AUTH:
          frame = SHPTR(ieee80211::Frame)(new ieee80211::Authentication);
          break;
        case ieee80211::Frame::SUBTYPE_DEAUTH:
          frame = SHPTR(ieee80211::Frame)(new ieee80211::Deauthentication);
          break;
        case ieee80211::Frame::SUBTYPE_ACTION:
          frame = SHPTR(ieee80211::Frame)(new ieee80211::ActionRequest);
          break;
        default:
          break;
      }
      break;
    }
    case ieee80211::Frame::TYPE_CNTL:
    {
      switch (frame->Subtype())
      {
        case ieee80211::Frame::SUBTYPE_RTS:
          // no break
        case ieee80211::Frame::SUBTYPE_CTS:
          // no break
        case ieee80211::Frame::SUBTYPE_ACK:
          // no break
        default:
          frame = SHPTR(ieee80211::Frame)(new ieee80211::ControlFrame);
          break;
      }
      break;
    }
    case ieee80211::Frame::TYPE_DATA:
    {
      switch (frame->Subtype())
      {
        case ieee80211::Frame::SUBTYPE_DATA:
          // no break
        case ieee80211::Frame::SUBTYPE_DATANULL:
          // no break
        case ieee80211::Frame::SUBTYPE_DATAQOS:
          // no break
        case ieee80211::Frame::SUBTYPE_DATAQOSNULL:
          // no break
        default:
          frame = SHPTR(ieee80211::Frame)(new ieee80211::DataFrame);
          break;
      }
      break;
    }
    default:
    {
      break;
    }
  }

  // Disassemble frame
  if (!frame || !frame->Disassemble(*sb, fcsflag) || !this->SetFrame(frame))
  {
    ZLOG_WARN("Cannot decode IEEE80211 frame");
    this->SetSubType(Notification::SUBTYPE_PKT_ERR);
  }

  return;
}

Notification::~Notification()
{
}

SHPTR(ieee80211::RadioTap)
Notification::RadiotapHeader()
{
  return (this->_rtaphdr);
}

bool
Notification::RadiotapHeader(SHPTR(ieee80211::RadioTap)rtaphdr_)
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

SHPTR(zSocket::Notification)
Socket::Recv()
{

  // Receive frame and convert to wireless notification
  SHPTR(Notification) n(new Notification(*this->socket.Recv()));

  if (n.get())
  {
    switch (n->GetSubType())
    {
      case zSocket::Notification::SUBTYPE_PKT_RCVD:
      {
        // Update destination address from actual frame
        SHPTR(zSocket::MacAddress) dst(new zSocket::MacAddress(n->GetFrame()->GetDestination()));
        n->SetDstAddress(dst);
        // Update source address from actual frame
        SHPTR(zSocket::MacAddress) src(new zSocket::MacAddress(n->GetFrame()->GetSource()));
        n->SetSrcAddress(src);
        break;
      }
      default:
        break;
    }
  }

  // Return wireless notification
  return (n);
}

bool
Socket::Send(const zSocket::Address& to_, const zSocket::Buffer& sb_)
{
  return (this->socket.Send(to_, sb_));
}

bool
Socket::Send(ieee80211::RadioTap hdr_, ieee80211::Frame& frame_)
{

  zSocket::Buffer sb;

  if (!hdr_.Assemble(sb))
  {
    ZLOG_ERR("Error assembling radiotap header");
    return (false);
  }

  // Assemble frame (writes buffer)
  if (!frame_.Assemble(sb, false))
  {
    ZLOG_ERR("Error assembling IEEE80211 header");
    return (false);
  }

  zSocket::MacAddress dst(frame_.GetDestination());

  // Restore data pointer to start of frame
  sb.Push(sb.Size());

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
