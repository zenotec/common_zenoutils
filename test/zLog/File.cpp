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

#include <fstream>

#include "zLogTest.h"

using namespace zUtils;

int
GetFileSize(std::ifstream &is)
{
  is.seekg(0, is.end);
  int length = is.tellg();
  is.seekg(0, is.beg);
  return (length);
}

int
zLogTest_FileConnector(void* arg_)
{

  zUtils::zLog::Log _zlogger(zLog::Log::MODULE_TEST);

  // Log file names
  const char *logName = "/tmp/fconn-utest.log";

  // Log file streams
  std::ifstream logFile;

  // Clean up from previous runs
  remove(logName);
  logFile.open(logName);
  TEST_FALSE(logFile.is_open());

  // Create file connector and register
  zLog::FileConnector fileConn(logName);
  zLog::Manager::Instance().RegisterConnector(zLog::Log::LEVEL_CRIT, &fileConn);

  // Log message and validate
  logFile.open(logName);
  TEST_TRUE(logFile.is_open());
  int logSize = GetFileSize(logFile);
  TEST_ISNOT_ZERO(logSize);
  ZLOG_CRIT("CRIT");
  usleep(100000);
  TEST_NEQ(logSize, GetFileSize(logFile));
  logFile.close();

  // Cleanup
  zLog::Manager::Instance().UnregisterConnector(zLog::Log::LEVEL_CRIT);

  // Clean up log files from /tmp
  remove(logName);
  logFile.open(logName);
  TEST_FALSE(logFile.is_open());

  // Return success
  return (0);
}

