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
ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);
#include <zutils/zMonitorInterface.h>
#include <zutils/zMonitorSocket.h>

// local includes

#include "ieee80211/RadioTap.h"
#include "ieee80211/Frame.h"
#include "ieee80211/Beacon.h"
#include "ieee80211/Probe.h"
#include "ieee80211/ControlFrame.h"
#include "ieee80211/DataFrame.h"
#include "ieee80211/Association.h"
#include "ieee80211/Authentication.h"
using namespace ieee80211;


namespace zUtils
{
namespace zWireless
{

//*****************************************************************************
// Class: MonitorSocketNotification
//*****************************************************************************

MonitorSocketNotification::MonitorSocketNotification(MonitorSocket& sock_) :
    Notification(sock_), _id(MonitorSocketNotification::ID_NONE)
{
}

MonitorSocketNotification::~MonitorSocketNotification()
{
}

MonitorSocketNotification::ID
MonitorSocketNotification::Id() const
{
  return(this->_id);
}

bool
MonitorSocketNotification::Id(MonitorSocketNotification::ID id_)
{
  this->_id = id_;
  return(true);
}

SHARED_PTR(ieee80211::RadioTap)
MonitorSocketNotification::RadiotapHeader()
{
  return(this->_rtaphdr);
}

bool
MonitorSocketNotification::RadiotapHeader(SHARED_PTR(ieee80211::RadioTap) rtaphdr_)
{
  this->_rtaphdr = rtaphdr_;
  return(true);
}

SHARED_PTR(ieee80211::Frame)
MonitorSocketNotification::Frame()
{
  return(this->_frame);
}

bool
MonitorSocketNotification::Frame(SHARED_PTR(ieee80211::Frame) frame_)
{
  this->_frame = frame_;
  return(true);
}


//*****************************************************************************
// Class: MonitorSocket
//*****************************************************************************

MonitorSocket::MonitorSocket(const int phy_, const std::string& name_, const bool simulateInputs_) :
    zEvent::Event(TYPE_MONSOCK), _iface(name_), _created(false), _injector(NULL), _simulateInputs(simulateInputs_)
{
  this->_iface.SetPhyIndex(phy_);
  if (_simulateInputs)
  {
    // TODO: If we keep using the injector, put it's addr in config
    _injectorSrcAddress = "127.0.0.9:5400";
    _injector = new MessageInjector("127.0.0.10:5400");
    _handler.RegisterEvent(_injector);
  }
}

MonitorSocket::~MonitorSocket()
{
  this->Close();
  if (_simulateInputs)
  {
    this->_handler.UnregisterEvent(&this->_sock);
    this->_handler.UnregisterObserver(this);
  }
}

bool
MonitorSocket::Open()
{

  std::string ifname = this->_iface.GetIfName();
  if (!this->_iface.GetIfIndex())
  {
    ZLOG_INFO("Monitor interface does not exist, creating: " + ifname);
    this->_created = true;
    if (!this->_iface.Create())
    {
      ZLOG_ERR("Cannot create monitor interface: " + ifname);
      return (false);
    }
  }

  this->_iface.SetAdminState(zInterface::ConfigData::STATE_UP);
  if (!this->_iface.Commit())
  {
    ZLOG_ERR("Failed to commit interface: " + ifname);
    return (false);
  }

  this->_handler.RegisterEvent(&this->_sock);
  this->_handler.RegisterObserver(this);

  if (!this->_sock.Open(zSocket::EthSocket::PROTO_LLC))
  {
    ZLOG_ERR("Cannot open socket for monitor interface: " + ifname);
    return (false);
  }

  if (!this->_sock.Bind(this->_iface))
  {
    ZLOG_ERR("Cannot bind socket for monitor interface: " + ifname);
    return (false);
  }

  return (true);

}

bool
MonitorSocket::Close()
{

  this->_sock.Close();

  if (this->_created)
  {
    if (!this->_iface.Destroy())
    {
      ZLOG_WARN("Cannot destroy monitor interface: " + this->_iface.GetIfName());
    }
    this->_created = false;
  }

  return(true);

}

bool
MonitorSocket::Send(ieee80211::RadioTap hdr_, ieee80211::Frame& frame_)
{

//  std::cout << "MonitorSocket::Send()" << std::endl;
  SHARED_PTR(zSocket::EthAddress) addr(new zSocket::EthAddress(frame_.Address(Frame::ADDRESS_1)));
  SHARED_PTR(zSocket::Buffer) sb(new zSocket::Buffer);
//  addr->Display();

  uint8_t* sbptr = sb->Head();
  size_t sbsize = sb->TotalSize();
//  printf("sbptr: %p - %zd\n", sbptr, sbsize);

  if (!(sbptr = hdr_.Assemble(sbptr, sbsize)))
  {
    ZLOG_ERR("Error assembling radiotap header");
    return (false);
  }

  // Assemble frame (writes buffer)
  if (!(sbptr = frame_.Assemble(sbptr, sbsize)))
  {
    ZLOG_ERR("Error assembling IEEE80211 header");
    return (false);
  }

  if (!sb->Put(sb->TotalSize() - sbsize))
  {
    ZLOG_ERR("Error updating socket buffer size");
    return (false);
  }

  zSocket::AddressBufferPair p(addr, sb);

//  std::cout << "Sending to: " << addr->Address() << std::endl;

  return (this->_sock.Send(p));

  //  return(false);
}

void
MonitorSocket::Display()
{
  this->_iface.Display();
}

bool
MonitorSocket::ObserveEvent(SHARED_PTR(zEvent::Notification) notification_)
{
  bool status = false;

//  std::cout << "MonitorSocket::EventHandler()" << std::endl;

  if (notification_)
  {
    if (notification_->GetType() == zEvent::Event::TYPE_SOCKET)
    {
//      std::cout << "MonitorInterface::EventHandler() - Socket event rcvd" << std::endl;
      status = this->ObserveEvent(STATIC_CAST(zSocket::Notification)(notification_));
    }
    else
    {
//      std::cout << "MonitorInterface::EventHandler() - UNKNOWN event rcvd" << std::endl;
      ZLOG_WARN("Event type unknown: " + ZLOG_INT(notification_->GetType()));
    }
  }

  return status;
}

bool
MonitorSocket::ObserveEvent(SHARED_PTR(zSocket::Notification) notification_)
{
  bool status = false;

  // TODO: This may need to be ifdef'd out for production code...
  if (_simulateInputs and ((notification_->Pkt().first)->GetAddress() != _injectorSrcAddress))
  {
    // SHORT CIRCUIT return if we're using the injector inputs only and this is not from it's address
    return false;
  }

  ZLOG_INFO("Socket notification");

  switch (notification_->Id())
  {
  case zSocket::Notification::ID_PKT_RCVD:
  {
    bool fcsflag = false;
    SHARED_PTR(MonitorSocketNotification) n(new MonitorSocketNotification(*this));
    n->Id(MonitorSocketNotification::ID_PKT_RCVD);
    uint8_t* f = notification_->Pkt().second->Head();
    size_t rem = notification_->Pkt().second->Length();

    // Process radiotap header
    n->RadiotapHeader(SHARED_PTR(RadioTap)(new RadioTap));
    f = n->RadiotapHeader()->Disassemble(f, rem);
    if (f == 0)
    {
      ZLOG_WARN("Cannot decode radiotap header");
      break;
    }
//    n->RadiotapHeader()->Display();

    // Check for presence of FCS
    RadioTapFieldFlags flags;
    n->RadiotapHeader()->GetField(flags);
    fcsflag = flags.FCS();

    // Peek at the 802.11 frame to determine its type/subtype
    Frame ieee80211hdr;
    f = ieee80211hdr.Peek(f, rem, fcsflag);
    if (f == 0)
    {
      ZLOG_WARN("Cannot decode IEEE80211 frame");
      break;
    }

    switch (ieee80211hdr.Type())
    {
    case Frame::TYPE_MGMT:
    {
      if ((ieee80211hdr.Subtype() == Frame::SUBTYPE_BEACON))
      {
        n->Frame(SHARED_PTR(Beacon)(new Beacon));
        f = n->Frame()->Disassemble(f, rem, fcsflag);
        if (f == 0)
        {
          ZLOG_WARN("Cannot decode beacon frame");
          break;
        }
//        n->Frame()->Display();
        this->NotifyHandlers(n);
      }
      else if ((ieee80211hdr.Subtype() == Frame::SUBTYPE_PROBEREQ))
      {
        n->Frame(SHARED_PTR(ProbeRequest)(new ProbeRequest));
        f = n->Frame()->Disassemble(f, rem, fcsflag);
        if (f == 0)
        {
          ZLOG_WARN("Cannot decode probe request frame");
          break;
        }
//        n->Frame()->Display();
        this->NotifyHandlers(n);
      }
      else if ((ieee80211hdr.Subtype() == Frame::SUBTYPE_PROBERESP))
      {
        n->Frame(SHARED_PTR(ProbeResponse)(new ProbeResponse));
        f = n->Frame()->Disassemble(f, rem, fcsflag);
        if (f == 0)
        {
          ZLOG_WARN("Cannot decode probe response frame");
          break;
        }
//        n->Frame()->Display();
        this->NotifyHandlers(n);
      }
      else if ((ieee80211hdr.Subtype() == Frame::SUBTYPE_ASSREQ))
      {
        n->Frame(SHARED_PTR(AssociationRequest)(new AssociationRequest));
        f = n->Frame()->Disassemble(f, rem, fcsflag);
        if (f == 0)
        {
          ZLOG_WARN("Cannot decode association request frame");
          break;
        }
//        n->Frame()->Display();
        this->NotifyHandlers(n);
      }
      else if ((ieee80211hdr.Subtype() == Frame::SUBTYPE_ASSRESP))
      {
        n->Frame(SHARED_PTR(AssociationResponse)(new AssociationResponse));
        f = n->Frame()->Disassemble(f, rem, fcsflag);
        if (f == 0)
        {
          ZLOG_WARN("Cannot decode association response frame");
          break;
        }
//        n->Frame()->Display();
        this->NotifyHandlers(n);
      }
      else if ((ieee80211hdr.Subtype() == Frame::SUBTYPE_AUTHENTICATE))
      {
        n->Frame(SHARED_PTR(Authentication)(new Authentication));
        f = n->Frame()->Disassemble(f, rem, fcsflag);
        if (f == 0)
        {
          ZLOG_WARN("Cannot decode authentication frame");
          break;
        }
//        n->Frame()->Display();
        this->NotifyHandlers(n);
      }
      status = true;
      break;
    }
    case Frame::TYPE_CNTL:
    {
      n->Frame(SHARED_PTR(ControlFrame)(new ControlFrame));
      f = n->Frame()->Disassemble(f, rem, fcsflag);
      if (f == 0)
      {
        ZLOG_WARN("Cannot decode control frame");
        break;
      }
//      n->Frame()->Display();
      this->NotifyHandlers(n);
      break;
    }
    case Frame::TYPE_DATA:
    {
      n->Frame(SHARED_PTR(DataFrame)(new DataFrame));
      f = n->Frame()->Disassemble(f, rem, fcsflag);
      if (f == 0)
      {
        ZLOG_WARN("Cannot decode data frame");
        break;
      }
//      n->Frame()->Display();
      this->NotifyHandlers(n);
      break;
    }
    default:
    {
      ZLOG_WARN("Unsupported frame type / subtype: " +
          zLog::HexStr(ieee80211hdr.Type()) + " / "
              + zLog::HexStr(ieee80211hdr.Subtype()));
      ieee80211hdr.Display();
      break;
    }
    } // end ieee80211hdr.Type()

    break;
  }
  case zSocket::Notification::ID_PKT_SENT:
  {
    status = true;
    break;
  }
  default:
    status = false;
    break;
  }
  return (status);
}

}
