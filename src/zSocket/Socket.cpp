//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Handler.cpp
//    Description:
//
//*****************************************************************************

#include <string.h>

#include <zutils/zLog.h>
#include <zutils/zSocket.h>

namespace zUtils
{
namespace zSocket
{

ssize_t
Socket::Receive(Address *addr_, Buffer *sb_)
{

  ssize_t bytes = -1;

  if (!this->Empty())
  {

    // Get first pair off queue and copy to caller's space */
    std::pair<zSocket::Address *, zSocket::Buffer *> q;
    q = this->Front();
    this->Pop();

    // Copy address */
    addr_->SetType(q.first->GetType());
    addr_->SetAddress(q.first->GetAddress());
    delete(q.first);

    // Copy to caller's socket buffer */
    *sb_ = *q.second;
    delete(q.second);

    // Update number of bytes received
    bytes = sb_->Size();
  }

  // Return number of bytes received
  return (bytes);

}

ssize_t
Socket::Receive(Address *addr_, std::string &str_)
{
  ssize_t bytes = 0;
  zSocket::Buffer *sb = new zSocket::Buffer;
  if (!sb)
  {
    std::string errMsg = "Error allocating memory for socket buffer";
    throw errMsg;
  }
  bytes = Receive(addr_, sb);
  if (bytes > 0)
  {
    str_ = sb->Str();
  }
  delete(sb);
  return (bytes);
}

ssize_t
Socket::Send(const Address *addr_, Buffer *sb_)
{
  return (this->_send(addr_, sb_));
}

ssize_t
Socket::Send(const Address *addr_, const std::string &str_)
{
  ssize_t bytes = 0;
  Buffer *sb = new Buffer;
  memcpy(sb->Head(), str_.c_str(), str_.size());
  sb->Put(str_.size());
  bytes = Send(addr_, sb);
  delete(sb);
  return (bytes);
}

ssize_t
Socket::Broadcast(Buffer *sb_)
{
  return (this->_broadcast(sb_));
}

ssize_t
Socket::Broadcast(const std::string &str_)
{
  ssize_t bytes = 0;
  Buffer *sb = new Buffer;
  memcpy(sb->Head(), str_.c_str(), str_.size());
  sb->Put(str_.size());
  bytes = Broadcast(sb);
  delete(sb);
  return (bytes);
}

}
}
