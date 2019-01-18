/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
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

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <net/if.h>
#include <netinet/in.h>
#include <poll.h>

#include <zutils/zLog.h>
#include <zutils/zSocket.h>
#include <zutils/zLoopSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// Class: zSocket::LoopSocketTx
//**********************************************************************

void
LoopSocketTx::Run(zThread::ThreadArg *arg_)
{

  bool exit = false;
  class LoopSocket* sock = (class LoopSocket*)arg_;

  // Setup for poll loop
  this->RegisterFd(sock->txq.GetFd(), (POLLIN | POLLERR));

  while (!exit)
  {

    std::vector<struct pollfd> fds;

    // Wait on file descriptor set
    int ret = this->Poll(fds);

    FOREACH (auto& fd, fds)
    {
      if (this->IsExit(fd.fd) && (fd.revents == POLLIN))
      {
        exit = true;
        continue;
      }
      else if (this->IsReload(fd.fd) && (fd.revents == POLLIN))
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

//**********************************************************************
// Class: zSocket::LoopSocket
//**********************************************************************

LoopSocket::LoopSocket() :
    Socket(SOCKET_TYPE::TYPE_LOOP),
    _txthread(&_txfunc, this),
    _fd(0)
{
}

LoopSocket::~LoopSocket()
{
  this->_txthread.Stop();
}

bool
LoopSocket::Bind(const Address& addr_)
{
  return (this->SetAddress(LoopAddress(addr_)) && this->_txthread.Start());
}

SHPTR(zSocket::Notification)
LoopSocket::send()
{

  // Initialize send notification
  SHPTR(zSocket::Notification) txn(this->txq.Front());
  this->txq.Pop();
//  fprintf(stderr, "LoopSocket::send(): Pop from TX queue\n");
//  txn->Display("txn\t");

  // Initialize receive notification
  SHPTR(zSocket::Notification) rxn(new zSocket::Notification(*this));
  rxn->SetSubType(Notification::SUBTYPE_PKT_RCVD);
  if (txn->GetSrcAddress().get())
    rxn->SetDstAddress(txn->GetSrcAddress());
  if (txn->GetDstAddress().get())
    rxn->SetSrcAddress(txn->GetDstAddress());
  if (txn->GetBuffer().get())
    rxn->SetBuffer(txn->GetBuffer());

  // Push onto receive queue
  this->rxq.Push(rxn);
//  fprintf(stderr, "LoopSocket::send(): Pushed to RX queue\n");
//  rxn->Display("rxn\t");

  // Return send notification
  return (txn);

}

}
}
