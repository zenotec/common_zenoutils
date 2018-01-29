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

#ifndef __WTP_MONITORSOCKET_H__
#define __WTP_MONITORSOCKET_H__

// libc includes

// libc++ includes
#include <string>

// libzutils includes
#include <zutils/zCompatibility.h>
#include <zutils/zEvent.h>
#include <zutils/zSocket.h>
#include <zutils/zEthSocket.h>
#include <zutils/zMonitorInterface.h>
using namespace zUtils;

// local includes

#include "ieee80211/RadioTap.h"
#include "ieee80211/Frame.h"

namespace zUtils
{
namespace zWireless
{

//*****************************************************************************
// Class: MonitorSocket
//*****************************************************************************

class MonitorSocket :
    public zEvent::Observer,
    public zEvent::Event
{

public:

  MonitorSocket(const int phy_, const std::string& name_);

  virtual
  ~MonitorSocket();

  bool
  Open();

  bool
  Close();

  bool
  Send(ieee80211::RadioTap hdr_, ieee80211::Frame& frame_);

  void
  Display();

protected:

  virtual bool
  ObserveEvent(SHARED_PTR(zEvent::Notification) notification_);

private:

  MessageInjector* _injector;

  bool _created;
  zWireless::MonitorInterface _iface;
  zSocket::EthAddress _addr;
  zSocket::EthSocket _sock;
  zEvent::Handler _handler;

  bool _simulateInputs;
  string _injectorSrcAddress;

  bool
  ObserveEvent(SHARED_PTR(zSocket::Notification) notification_);

};

//*****************************************************************************
// Class: MonitorSocketNotification
//*****************************************************************************

class MonitorSocketNotification :
    public zEvent::Notification
{

public:

  enum ID
  {
    ID_ERR = -1,
    ID_NONE = 0,
    ID_PKT_RCVD = 1,
    ID_PKT_SENT = 2,
    ID_PKT_ERR = 3,
    ID_LAST
  };

  MonitorSocketNotification(MonitorSocket& sock_);

  virtual
  ~MonitorSocketNotification();

  MonitorSocketNotification::ID
  Id() const;

  bool
  Id(MonitorSocketNotification::ID);

  SHARED_PTR(ieee80211::RadioTap)
  RadiotapHeader();

  bool
  RadiotapHeader(SHARED_PTR(ieee80211::RadioTap) rtaphdr_);

  SHARED_PTR(ieee80211::Frame)
  Frame();

  bool
  Frame(SHARED_PTR(ieee80211::Frame) frame_);

protected:

private:

  MonitorSocketNotification::ID _id;
  SHARED_PTR(ieee80211::RadioTap) _rtaphdr;
  SHARED_PTR(ieee80211::Frame) _frame;

};

}

#endif /* __WTP_MONITORSOCKET_H__ */
