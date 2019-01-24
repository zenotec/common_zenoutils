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
#include <net/if.h>

// libc++ includes

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;
#include <zutils/zSocket.h>
#include <zutils/zRawAddress.h>
#include <zutils/ieee80211/RadioTap.h>
using namespace zSocket;
#include <zutils/nl80211/FrequencyAttribute.h>
#include <zutils/nl80211/RxSignalLevelAttribute.h>
#include <zutils/nl80211/FrameAttribute.h>
#include <zutils/nl80211/FrameSocket.h>

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace nl80211
{

//**********************************************************************
// Class: FrameSocketTx
//**********************************************************************

void
FrameSocketTx::Run(zThread::ThreadArg *arg_)
{

  bool exit = false;
  class FrameSocket* sock = (class FrameSocket*)arg_;

  this->RegisterFd(sock->txq.GetFd(), (POLLIN | POLLERR));

  while (!exit)
  {

    std::vector<struct pollfd> fds;

    // Wait on file descriptor set
    int ret = this->Poll(fds);

    FOREACH (auto& fd, fds)
    {
      if (this->IsExitFd(fd))
      {
        exit = true;
        continue;
      }
      else if (this->IsReloadFd(fd))
      {
        continue;
      }
      else if ((sock->txq.GetFd() == fd.fd) && (fd.revents == POLLIN))
      {
        if (sock->txq.TryWait())
        {
          sock->rxq.Push(sock->send());
        }
      }
    }

  }

  this->UnregisterFd(sock->txq.GetFd());

  return;

}

//*****************************************************************************
// Class: FrameSocket
//*****************************************************************************

FrameSocket::FrameSocket() :
    Socket(SOCKET_TYPE::TYPE_RAW),
    _txthread(&_txfunc, this),
    _fcmd(NULL),
    _fevent(NULL)
{
}

FrameSocket::~FrameSocket()
{

  this->_txthread.Stop();

  if (this->_fcmd)
  {
    delete (this->_fcmd);
    this->_fcmd = NULL;
  }
  if (this->_fevent)
  {
    delete (this->_fevent);
    this->_fevent = NULL;
  }
}

bool
FrameSocket::RegisterFrame(const uint16_t type_, const uint8_t* match_, const size_t len_)
{
  bool status = false;
  if (this->_fevent)
  {
    this->_fevent->FrameType(type_);
    this->_fevent->FrameMatch.Set(match_, len_);
    status = this->_fevent->Exec();
  }
  return (status);
}

bool
FrameSocket::Bind(const zSocket::Address& addr_)
{

  if (this->_fevent)
  {
    ZLOG_ERR(std::string("Socket already bound"));
    return (false);
  }

  // Validate address type
  if (addr_.GetType() != Address::TYPE_RAW)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  // Convert address
  RawAddress raddr(addr_);
  int ifindex = raddr.GetSA().sll_ifindex;

  this->_fcmd = new nl80211::FrameCommand(ifindex);
  if (!this->_fcmd)
  {
    ZLOG_ERR("Error instantiating frame command");
    return (false);
  }

  this->_fevent = new nl80211::FrameEvent(ifindex, *this);
  if (!this->_fevent || !this->_fevent->GetSockFd())
  {
    ZLOG_ERR("Error instantiating frame event");
    return (false);
  }

  this->_txthread.Start();

  return (true);
}

bool
FrameSocket::Listen()
{
  bool status = false;
  if (this->_fevent)
  {
    status = this->_fevent->Listen();
  }
  return (status);
}

SHARED_PTR(zSocket::Notification)
FrameSocket::send()
{

  // Initialize notification
  SHARED_PTR(zSocket::Notification) n(this->txq.Front());
  this->txq.Pop();
  zSocket::Address addr(*n->GetDstAddress());
  zSocket::Buffer sb(*n->GetBuffer());
  n->SetSubType(Notification::SUBTYPE_PKT_ERR);

  if (this->_fcmd && this->_fcmd->Frame.Set(sb.Head(), sb.Size()) && this->_fcmd->Exec())
  {
    ZLOG_DEBUG("(" + ZLOG_INT(this->GetFd()) + ") " + "Sent " + ZLOG_INT(sb.Length()) +
        " bytes to: " + addr.GetAddress());
    n->SetSubType(Notification::SUBTYPE_PKT_SENT);
  }
  else
  {
    ZLOG_ERR(std::string("Cannot send packet: " + std::string(strerror(errno))));
    n->SetSubType(Notification::SUBTYPE_PKT_ERR);
  }

  return (n);
}

int
FrameSocket::valid_cb(struct nl_msg* msg_, void* arg_)
{

//  fprintf(stderr, "FrameSocket::valid_cb()\n");

  GenericMessage msg;
  if (!msg.Disassemble(msg_))
  {
    ZLOG_ERR("Error parsing generic message");
    return (NL_SKIP);
  }
  //  msg.Display();

  nl80211::FrameAttribute frame;
  if (!msg.GetAttribute(&frame))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(frame.GetId()));
    return(NL_SKIP);
  }

  nl80211::FrequencyAttribute freq;
  if (!msg.GetAttribute(&freq))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(freq.GetId()));
    return(NL_SKIP);
  }

  nl80211::RxSignalLevelAttribute rxlevel;
  if (!msg.GetAttribute(&rxlevel))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(rxlevel.GetId()));
    return(NL_SKIP);
  }

  // Parse message and notify
  SHARED_PTR(zSocket::Notification) n(new zSocket::Notification(*this));
  n->SetSubType(Notification::SUBTYPE_PKT_RCVD);

  SHARED_PTR(zSocket::Buffer) sb(new zSocket::Buffer);
  n->SetBuffer(sb);

  // Create radiotap header
  zWireless::ieee80211::RadioTap rt;

  // Set the channel
  zWireless::ieee80211::RadioTapFieldChannel chnl;
  chnl.Channel(freq.GetChannel());
  rt.PutField(chnl);

  // Set signal level
  zWireless::ieee80211::RadioTapFieldSignalLevel rssi;
  rssi(rxlevel());
  rt.PutField(rssi);

  // Write the radiotap header
  if (!rt.Assemble(*sb))
  {
    ZLOG_ERR("Couldn't assemble radiotap hdr");
    return(NL_SKIP);
  }

  // Need to latch the radiotap header length to use later to reset data pointer
  size_t rtlen = sb->Size();

  // Copy 802.11 frame
  size_t len = sb->Tailroom();
  frame.Get(sb->Data(), len);
  sb->Put(len);

  // Reset the data pointer to the start of radiotap header before pushing onto queue
  sb->Push(rtlen);

  // Push to receive queue to allow 'Recv' to return the notification to the socket handler
  this->rxq.Push(n);

  return (NL_OK);

}

int
FrameSocket::ack_cb(struct nl_msg* msg_, void* arg_)
{
  fprintf(stderr, "FrameSocket::ack_cb()\n");
  return (NL_OK);
}

int
FrameSocket::err_cb(struct sockaddr_nl* nla_, struct nlmsgerr* nlerr_, void* arg_)
{
  fprintf(stderr, "FrameSocket::err_cb()\n");
  return(NL_SKIP);
}

}
