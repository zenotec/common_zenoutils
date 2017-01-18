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
const std::string MessagePath::TypeDataPath("Type");
const std::string MessagePath::IdDataPath("Id");
const std::string MessagePath::DstDataPath("Dst");
const std::string MessagePath::SrcDataPath("Src");
const std::string MessagePath::DataDataPath(zData::DataPath::DataRoot);

MessagePath::MessagePath(const std::string& root_) :
    zData::DataPath(MessagePath::DataRoot)
{
  if (!root_.empty())
  {
    this->_root.push_back(root_);
  }
}

MessagePath::MessagePath(const MessagePath& other_) :
    zData::DataPath(other_)
{
}

MessagePath::MessagePath(const zData::DataPath& path_) :
    zData::DataPath(path_)
{
}

MessagePath::~MessagePath()
{
}

}
}
