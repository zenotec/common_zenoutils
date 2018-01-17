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

#include <iostream>
#include <zutils/zLog.h>

namespace zUtils
{
namespace zLog
{

//*****************************************************************************
// Class: FileConnector
//*****************************************************************************
FileConnector::FileConnector(const std::string& logfile_) :
    _file(logfile_.c_str(), std::ios_base::app)
{
  if (this->_file.is_open())
  {
    this->_file << "************************************************************" << std::endl;
    this->_file << "* Logging started" << std::endl;
    this->_file << "************************************************************" << std::endl;
    this->_file.flush();
  }
}

FileConnector::~FileConnector()
{
  if (this->_file.is_open())
  {
    this->_file << "************************************************************" << std::endl;
    this->_file << "* Logging stopped" << std::endl;
    this->_file << "************************************************************" << std::endl;
    this->_file.flush();
    this->_file.close();
  }
}

void
FileConnector::Logger(std::string msg_)
{
  this->_file << msg_ << std::endl;
  this->_file.flush();
}

//*****************************************************************************
// Class: ConsoleConnector
//*****************************************************************************
ConsoleConnector::ConsoleConnector()
{
  std::cout << "************************************************************" << std::endl;
  std::cout << "* Logging started" << std::endl;
  std::cout << "************************************************************" << std::endl;
  std::cout.flush();
}

ConsoleConnector::~ConsoleConnector()
{
  std::cout << "************************************************************\n" << std::endl;
  std::cout << "* Logging stopped" << std::endl;
  std::cout << "************************************************************\n" << std::endl;
  std::cout.flush();
}

void
ConsoleConnector::Logger(std::string msg_)
{
  std::cout << msg_ << std::endl;
  std::cout.flush();
}

}
}
