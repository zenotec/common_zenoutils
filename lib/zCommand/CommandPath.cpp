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

#include <string>
#include <mutex>
#include <list>
#include <map>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zEvent.h>
#include <zutils/zData.h>
#include <zutils/zQueue.h>
#include <zutils/zSocket.h>
#include <zutils/zMessage.h>

#include <zutils/zCommand.h>

namespace zUtils
{
namespace zCommand
{

//**********************************************************************
// Class: CommandPath
//**********************************************************************

const std::string CommandPath::DataRoot("zCommand");
const std::string CommandPath::NameDataPath("Name");
const std::string CommandPath::OptionDataPath(CommandOptionPath::DataRoot);
const std::string CommandPath::OutputDataPath("Output");

CommandPath::CommandPath(const std::string& root_) :
    zData::DataPath(DataRoot)
{
  if (!root_.empty())
  {
    this->_root.push_back(root_);
  }
}

CommandPath::CommandPath(const CommandPath& other_) :
    zData::DataPath(other_)
{
}

CommandPath::CommandPath(const zData::DataPath& path_) :
    zData::DataPath(path_)
{
}

CommandPath::~CommandPath()
{
}

}
}
