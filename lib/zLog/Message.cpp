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

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <iomanip>
#include <system_error>

#include <zutils/zLog.h>

namespace zUtils
{
namespace zLog
{

//*****************************************************************************
// Class: Message
//*****************************************************************************

Message::Message(const std::string& module_, const Log::LEVEL level_) :
    _module(module_), _level(level_)
{

  // Latch the process id of the calling process
  pid_t pid = getpid();
  this->_proc = ZLOG_UINT(uint32_t(pid));

  // Latch the thread id of the calling thread
  pthread_t tid = pthread_self();
  this->_thread = ZLOG_HEX(uint32_t(tid));

  // Latch the current time
  struct timespec ts;
  std::stringstream timestamp;
  clock_gettime(CLOCK_REALTIME, &ts);
  timestamp << std::setprecision(20)
      << ((double) ts.tv_sec + (double) ts.tv_nsec / (double) (1000000000));
  this->_time = timestamp.str();

}

Message::~Message()
{
}

const std::string&
Message::GetModule() const
{
  return (this->_module);
}

const Log::LEVEL
Message::GetLevel() const
{
  return (this->_level);
}

const std::string&
Message::GetProcessId() const
{
  return (this->_proc);
}

const std::string&
Message::GetThreadId() const
{
  return (this->_thread);
}

const std::string&
Message::GetTimestamp() const
{
  return (this->_time);
}

const std::string&
Message::GetFile() const
{
  return (this->_file);
}

void
Message::SetFile(const std::string& file_)
{
  this->_file = file_.substr(file_.find_last_of("/") + 1);
}

const std::string&
Message::GetLine() const
{
  return (this->_line);
}

void
Message::SetLine(const unsigned int line_)
{
  this->SetLine(ZLOG_UINT(line_));
}

void
Message::SetLine(const std::string& line_)
{
  this->_line = line_;
}

const std::string&
Message::GetMessage() const
{
  return (this->_message);
}

void
Message::AddMessage(const std::string &message_)
{
  this->_message += message_;
}

//std::string
//Message::GetMessage() const
//{
//  std::string str;
//  str += this->GetTimestamp() + "\t";
//  str += this->GetProcessId() + "\t";
//  str += this->GetThreadId() + "\t";
//  str += std::string(levelStr[this->_level]) + "\t";
//  str += this->_file + "[" + this->_line + "]\t";
//  str += this->_msg + "\n";
//  return (str);
//}

}
}
