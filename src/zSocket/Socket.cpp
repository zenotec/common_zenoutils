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
Socket::RecvBuffer(Address &addr_, Buffer &sb_)
{
  ssize_t bytes = -1;
  if (!this->Empty())
  {
    std::pair<zSocket::Address, zSocket::Buffer *> q;
    q = this->Front();
    this->Pop();
    addr_ = q.first;
    sb_ = *q.second;
    bytes = sb_.Size();
    delete (q.second);
  }
  return (bytes);
}

ssize_t
Socket::RecvString(Address &addr_, std::string &str_)
{
  ssize_t bytes = 0;
  zSocket::Buffer *sb = new zSocket::Buffer;
  if (!sb)
  {
    std::string errMsg = "Error allocating memory for socket buffer";
    throw errMsg;
  }
  bytes = RecvBuffer(addr_, *sb);
  if (bytes > 0)
  {
    str_ = sb->Str();
  }
  delete (sb);
  return (bytes);
}

ssize_t
Socket::SendBuffer(const Address &addr_, Buffer &sb_)
{
  return (this->_send(addr_, sb_));
}

ssize_t
Socket::SendString(const Address &addr_, const std::string &str_)
{
  ssize_t bytes = 0;
  Buffer sb;
  memcpy(sb.Head(), str_.c_str(), str_.size());
  sb.Put(str_.size());
  bytes = SendBuffer(addr_, sb);
  return (bytes);
}

}
}
