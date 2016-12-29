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

#include <list>
#include <mutex>
#include <memory>

#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zMessage.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// Class: MessagePath
//**********************************************************************

const std::string MessagePath::DataRoot("zMessage");
const std::string MessagePath::DataTypePath("Type");
const std::string MessagePath::DataIdPath("Id");
const std::string MessagePath::DataDstPath("Dst");
const std::string MessagePath::DataSrcPath("Src");
const std::string MessagePath::DataDataPath("Data");

MessagePath::MessagePath(const std::string& root_) :
    zData::DataPath(MessagePath::DataRoot)
{
  if (!root_.empty())
  {
    this->_root.push_back(root_);
  }
}

MessagePath::MessagePath(zData::DataPath& path_) :
    zData::DataPath(path_)
{
}

MessagePath::MessagePath(const zData::DataPath& path_) :
    zData::DataPath(path_)
{
}

MessagePath::~MessagePath()
{
}

zData::DataPath&
MessagePath::GetDataPath()
{
  return (*this);
}

const zData::DataPath&
MessagePath::GetDataPath() const
{
  return (*this);
}

MessagePath&
MessagePath::GetMessagePath()
{
  return (*this);
}

const MessagePath&
MessagePath::GetMessagePath() const
{
  return (*this);
}

zData::DataPath
MessagePath::Id() const
{
  zData::DataPath path(*this);
  path.Append(MessagePath::DataIdPath);
  return(path);
}

zData::DataPath
MessagePath::Type() const
{
  zData::DataPath path(*this);
  path.Append(MessagePath::DataTypePath);
  return(path);
}

zData::DataPath
MessagePath::Dst() const
{
  zData::DataPath path(*this);
  path.Append(MessagePath::DataDstPath);
  return(path);
}

zData::DataPath
MessagePath::Src() const
{
  zData::DataPath path(*this);
  path.Append(MessagePath::DataSrcPath);
  return(path);
}

zData::DataPath
MessagePath::Data() const
{
  zData::DataPath path(*this);
  path.Append(MessagePath::DataDataPath);
  return(path);
}

}
}
